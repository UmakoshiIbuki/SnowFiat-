#include"SnowBall.h"
#include"Application/main.h"
#include"../../Component/ModelComponent.h"
#include"../Particle.h"
#include"../Scene.h"

void SnowBall::Deserialize(const json11::Json& jsonObj)
{
	if (jsonObj.is_null()) { return; }

	GameObject::Deserialize(jsonObj);

	if (jsonObj["Speed"].is_null() == false)
	{
		m_speed = jsonObj["Speed"].number_value();
	}
	//煙テクスチャ
	m_trailSmoke.SetTexture(ResFac.GetTexture("Data/Texture/smokeline2.png"));

	for (UINT i = 0; i < 200; i++)
	{
		particleSnow[i] = std::make_shared< Particle>();
	}
}

void SnowBall::Update()
{
	if (m_alive == false) { return; }

	if (--m_lifeSpan <= 0)
	{
		Destroy();
	}	

	frame++;
	 {
		static const std::string filename = "Data/SnowCrystal.png";
		particleSnow[count]->SetTextureFile(filename);
		particleSnow[count]->SetShowTime(20);
		particleSnow[count]->SetSize(0.1f);
		particleSnow[count]->SetMove(0.05f, 0.05f, 0.03f, 0.05f);
		particleSnow[count]->Deserialize(ResFac.GetJSON("Data/Scene/Particle.json"));
		particleSnow[count]->SetMatrix(m_mWorld);

		Scene::GetInstance().AddObject(particleSnow[count]);
		count++;
	}
	
	Vec3 move = m_mWorld.GetAxisZ();
	move.Normalize();
	m_speed *= 0.98;
	move *= m_speed;
	m_gravity += 0.005f;
	move.y -= m_gravity;
	// 移動前の座標を覚える
	m_prevPos = m_mWorld.GetTranslation();

	m_mWorld.Move(move);
	
	UpdateCollision();

	//軌跡の更新
	//UpdateTrail();
}

#include"Human.h"
#include"Enemy.h"
void SnowBall::UpdateCollision()
{
	m_colRadius = 0.1f;
	//発射した主人のshared_ptr取得
	auto spOwner = m_wpOwner.lock();

	SphereInfo Info;
	Info.m_pos = m_mWorld.GetTranslation();				//ひとつ前の場所から
	Info.m_radius = m_colRadius;				//動いた方向に向かって

	//レイ判定情報を作成
	RayInfo rInfo;
	rInfo.m_pos = m_prevPos;				//ひとつ前の場所から
	rInfo.m_dir = m_mWorld.GetTranslation() - m_prevPos;
	rInfo.m_maxRange = rInfo.m_dir.Length();
	rInfo.m_dir.Normalize();

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		//自分自身は無視
		if (obj.get() == this) { continue; }
		//発射したヤツも無視
		if (obj.get() == spOwner.get()) { continue; }

		bool hit = false;

		//球判定
		if (obj->GetTag() & TAG_Character)
		{
			if (obj->HitCheckBySphere(Info))
			{
				hit = true;
			}
		}

		if (hit)
		{
			//std::dinamic_pointer_cast=基底クラス型をダウンキャストするときに使う。失敗するとnullptrが帰る
			std::shared_ptr<Human> human = std::dynamic_pointer_cast<Human>(obj);
			if (human)
			{
				human->Damage(10);
				Scene::GetInstance().SetWhiteIn(20);
			}
			std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj);
			if (enemy)
			{
				enemy->Damage(10);
			}
		}

		//ステージとはレイ判定
		if (obj->GetTag() & TAG_StageObject)
		{
			KdRayResult rResult;
			hit = obj->HitCheckByRay(rInfo, rResult);
		}

		if (hit)
		{
			
			ParticleEffect();
			Destroy();
		}
	}
}

#include"../../Game/AnimationEffect.h"
void SnowBall::ParticleEffect()
{
	static const std::string filename = "Data/White.png";
	std::shared_ptr<Particle> particle = std::make_shared< Particle>();
	particle->SetTextureFile(filename);
	particle->SetShowTime(30);
	particle->SetSize(0.5f);
	particle->SetMove(0.3f, 0.3f,0.2f,0.2f);
	particle->Deserialize(ResFac.GetJSON("Data/Scene/Particle.json"));
	particle->SetMatrix(m_mWorld);

	Scene::GetInstance().AddObject(particle);
}

void SnowBall::UpdateTrail()
{
	//軌跡の座標を先頭に追加
	m_trailSmoke.AddPoint(m_mWorld);

	//軌跡の数制限(100以前の軌跡を消去する
	if (m_trailSmoke.GetNumPonints() > 10) {
		m_trailSmoke.DelPoint_Back();
	}
}

void SnowBall::DrawEffect()
{
	if (!m_alive) { return; }

	SHADER.m_effectShader.SetWorldMatrix(Matrix());
	SHADER.m_effectShader.WriteToCB();

	m_trailSmoke.DrawBillboard(0.5f);
}

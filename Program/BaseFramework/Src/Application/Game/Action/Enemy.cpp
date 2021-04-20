#include"Enemy.h"
#include"SnowBall.h"
#include"Wall.h"
#include"Human.h"
#include"../Scene.h"
#include"../../Component//CameraComponent.h"
#include"../../Component//ModelComponent.h"
#include"../../Component//InputComponent.h"
#include"../AnimationEffect.h"

const float Enemy::s_allowToStepHeight=0.8f;
const float Enemy::s_landingHeight=0.1f;

void Enemy::Deserialize(const json11::Json& jsonObj)
{	
	GameObject::Deserialize(jsonObj);

	m_spInputComponent = std::make_shared<EnemyInputComponent>(*this);
	m_EnemyMat.resize(9);
	m_pos = m_mWorld.GetTranslation();
	for (UINT i = 0; i < m_EnemyMat.size(); i++)
	{
		if (pX > 2)
		{
			pX = 0;
			pY++;
		}
		m_EnemyMat[i].CreateTranslation(pX * 20.0f, 0, pY * 20.0f);
		pX++;
	}
	m_sphuman=Scene::GetInstance().FindObjectWithName("PlayerHuman");
	
	m_spBikkurieffect = std::make_shared< AinmationEffect>();
	Scene::GetInstance().AddObject(m_spBikkurieffect);

	SetAnimation("Walk");
}

void Enemy::Update()
{
	/*SphereInfo sinfo;
	sinfo.m_pos = m_sphuman->GetMatrix().GetTranslation();
	sinfo.m_radius = 15;
	if (!HitCheckBySphere(sinfo)) { return; }*/

	if (m_RespawnTime >0)
	{
		this->GetModelComponent()->SetDissolveThreshold(m_RespawnTime);
		m_RespawnTime--;
		return;
	}

	if (m_hp < 0)
	{
		m_spActionState = std::make_shared<ActionCrash>();
	}
	else {
		m_spActionState.reset();
		if (!m_LetsMove)
		{
			m_LetsMove = true;
			m_mWorld.CreateTranslation(0,5,0);
			m_pos = m_mWorld.GetTranslation();
			return;
		}
	}
	if (m_mWorld.GetTranslation().y < -10) { m_hp -= 100; }

	frame++;

	if(m_spInputComponent)
	{
		m_spInputComponent->Update();
	}

	if (m_spActionState)
	{
		m_spActionState->Update(*this);
	}
	
	//移動前の座標を覚える
	m_prevPos = m_pos;

	Reload();

	UpdateShoot();

	CheckBump();

	if (m_canTrace)
	{
		Trace();
		m_pos = m_mWorld.GetTranslation();
	}
	else
	{
		
		m_BikkurieffectMat.SetTranslation(m_mWorld.GetTranslation().x, m_mWorld.GetTranslation().y + 1, m_mWorld.GetTranslation().z);
		m_spBikkurieffect->SetAnimationInfo(ResFac.GetTexture("Data/Texture/Hatena.png"), 0.4f, 1, 1, 0, 0, 0);
		m_spBikkurieffect->SetMatrix(m_BikkurieffectMat);
		

		//重力をキャラクターのYの移動力に加える
		m_force.y -= m_gravity;

		UpdateRotate(m_force);
		m_mWorld.CreateRotationX(m_rot.x);
		m_mWorld.RotateY(m_rot.y);
		m_pos.Move(m_force);
		m_mWorld.Scale(m_scale.x, m_scale.y, m_scale.z);
		m_mWorld.Move(m_pos);
		if (frame %120==0){
			i = rand() % 9; 
			m_force.y += 0.1f;
		}

		if (m_scale.Length() < 1)
		{
			SetAnimation("Walk");

			m_hitRange = 5;
			Shot = 0;
			if (i % 2 == 0)
			{
				m_force.z = 0;
				if (m_mWorld.GetTranslation().x < m_EnemyMat[i].GetTranslation().x) { m_force.x = m_Movespeed; }
				if (m_mWorld.GetTranslation().x > m_EnemyMat[i].GetTranslation().x) { m_force.x = -m_Movespeed; }
				m_force.z = 0.0f;
			}
			else
			{
				m_force.x = 0;
				if (m_mWorld.GetTranslation().z > m_EnemyMat[i].GetTranslation().z) { m_force.z = -m_Movespeed; }
				if (m_mWorld.GetTranslation().z < m_EnemyMat[i].GetTranslation().z) { m_force.z = m_Movespeed; }
				m_force.x = 0.0f;
			}
		}
		else
		{
			m_hitRange = 9;
			Shot = 360;
			m_IsHitRange = true;
		}
	}

	UpdateCollision();
	m_animator.AdvanceTime(m_spModelComponent->GetChangeableNodes());

}

void Enemy::SetAnimation(const char* pAnimName)
{
	if (m_spModelComponent)
	{
		std::shared_ptr<KdAnimationData> animData = m_spModelComponent->GetAnimation(pAnimName);
		m_animator.SetAnimation(animData);
	}
}

void Enemy::Damage(int damage)
{
	m_hp -= damage;
	m_force.y = 0.1f;
}

bool Enemy::CanShoot()
{
	if (m_charge == 0) { return false; }
	return true;
}

void Enemy::Trace()
{
	m_spBikkurieffect->SetAnimationInfo(ResFac.GetTexture("Data/Texture/Bikkuri.png"), 0.4f, 1, 1, 0, 0, 0);
	m_BikkurieffectMat.SetTranslation(m_mWorld.GetTranslation().x, m_mWorld.GetTranslation().y + 1, m_mWorld.GetTranslation().z);
	m_spBikkurieffect->SetMatrix(m_BikkurieffectMat);

	Vec3 Dir = m_sphuman->GetMatrix().GetTranslation() - m_mWorld.GetTranslation();

	Dir.Normalize();
	UpdateRotate(Dir);

	Matrix copyMat = m_mWorld;
	m_mWorld.CreateRotationX(m_rot.x);
	m_mWorld.RotateY(m_rot.y);
	m_mWorld.Scale(m_scale.x, m_scale.y, m_scale.z);

	m_mWorld.SetTranslation(copyMat.GetTranslation());
	m_pos = Dir * 0.02f;
	m_pos.y -= m_gravity;
	if (m_sphuman->GetMatrix().GetTranslation().y > m_mWorld.GetTranslation().y) { m_pos.y = 0; }

	if (m_IsHitRange) { return; }

	m_mWorld.Move(m_pos);
}

void Enemy::UpdateShoot()
{
	if (!m_canTrace) { return; }
	if (m_spInputComponent->GetButton(Input::Buttons::A))
	{
		if (m_canShoot)
		{
			if (m_charge <= 0) { return; }
			m_charge--;

			std::shared_ptr<SnowBall> spSnowBall = std::make_shared< SnowBall>();
			if (spSnowBall)
			{
				spSnowBall->Deserialize(ResFac.GetJSON("Data/Scene/SnowBall.json"));

				Matrix mLaunch;
				mLaunch.CreateTranslation(0.0f, 5.0f, 5.0f);

				if (m_scale.Length() > 1)
				{
					spSnowBall->SetGravity(0);
					mLaunch.CreateTranslation(0.0f, 3.0f, 5.0f);

				}
				mLaunch.RotateX(((360 - Shot) * KdToRadians) );
				mLaunch.Scale(0.2f, 0.2f, 0.2f);
				mLaunch *= m_mWorld;
				
				spSnowBall->SetMatrix(mLaunch);

				spSnowBall->SetOwner(shared_from_this());

				Scene::GetInstance().AddObject(spSnowBall);

				float minDistance = FLT_MAX;
			}
			m_canShoot = false;
		}
	}
	else
	{
		m_canShoot = true;
	}
}

void Enemy::Reload()
{
	if (m_spInputComponent->GetButton(Input::Buttons::B))
	{
		m_charge += 0.1f;
	}
}

//rMoveDir:移動方向
void Enemy::UpdateRotate(const Vec3& rMoveDir)
{
	//移動していなければ帰る
	if (rMoveDir.LengthSquared() == 0.0f) { return; }
	//今のキャラクターの方向ベクトル
	Vec3 nowDir = m_mWorld.GetAxisZ();
	nowDir.Normalize();

	//キャラクターの今向いている方向の角度を求める(ラジアン角)
	float nowRadian = atan2(nowDir.x, nowDir.z);

	//移動方向へのベクトルの角度を求める(ラジアン角)
	float targetRadian = atan2(rMoveDir.x, rMoveDir.z);
	float rotateRadian = targetRadian - nowRadian;

	if (rotateRadian > M_PI)
	{
		rotateRadian -= 2 * float(M_PI);
	}
	else if (rotateRadian < -M_PI)
	{
		rotateRadian += 2 * float(M_PI);
	}

	//1回の回転速度をm_m_rotateAngle度内に収める(クランプ)
	rotateRadian = std::clamp(rotateRadian, -m_rotateAngle * KdToRadians, m_rotateAngle * KdToRadians);

	m_rot.y += rotateRadian;
}

void Enemy::UpdateCollision()
{
	float distanceFromGround = FLT_MAX;

	SphereInfo sInfo;
	sInfo.m_pos = m_mWorld.GetTranslation();
	sInfo.m_radius = 15.0f; //索敵範囲

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		if (obj.get() == this) { continue; }

		if (obj->GetTag() & TAG_Player)
		{
			if (obj->HitCheckBySphere(sInfo)) 
			{				
				m_canTrace =true;
				if (m_scale.Length() > 1) { return; }
			}
			else {
				m_canTrace = false;
			}

			sInfo.m_radius = m_hitRange; //攻撃範囲
			if (obj->HitCheckBySphere(sInfo))
			{
				m_IsHitRange = true;
			}
			else
			{
				m_IsHitRange = false;
			}
		}
	}

	//下方向への判定を行い、着地した
	if (CheckGround(distanceFromGround,TAG_StageObject,m_pos))
	{
		//地面の上にy座標を移動
		m_pos.y += s_allowToStepHeight - distanceFromGround;
		//地面があるので、y方向への移動量は0に
		m_force.y = 0.0f;
		return;
	}
}

void Enemy::CheckBump()
{
	SphereInfo info;

	info.m_pos = m_mWorld.GetTranslation();		//中心点キャラクターの位置
	info.m_pos.y += 0.8f;	//キャラクターのぶつかり判定をするので、ちょっと上に持ち上げる
	info.m_radius = 0.4f;	//キャラクターの大きさに合わせて半径サイズもいい感じに設定する

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		if (obj.get() == this) { continue; }

		SphereResult Result;

		if (obj->HitCheckBySphereVsMesh(info, Result))
		{
			m_pos.Move(Result.m_push);
			m_canTrace = false;
		}
	}
}

bool Enemy::CheckGround(float& rDstDistance,UINT m_tag,Vec3 Pos)
{
	RayInfo rayInfo;
	rayInfo.m_pos = Pos;

	//キャラの足元からレイを発射すると地面と当たらないので少し持ち上げる(乗り越えられる段差の高さ分だけ)
	rayInfo.m_pos.y += s_allowToStepHeight;

	//落下中かもしれないので、1フレーム前の座標分も持ち上げる
	rayInfo.m_pos.y += m_prevPos.y - Pos.y;

	//地面方向へのレイ
	rayInfo.m_dir = { 0.0f,-1.0f,0.0f };
	rayInfo.m_maxRange = FLT_MAX;

	KdRayResult finalRayResult;

	//全員とレイ判定
	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		if (obj.get() == this) { continue; }

		//もらってきたタグと判定
		if (!(obj->GetTag() & (m_tag))) { continue; }
		KdRayResult rayResult;

		if (obj->HitCheckByRay(rayInfo, rayResult))
		{
			//最も当たったところまでの距離が近いものを保持する
			if (rayResult.m_distance < finalRayResult.m_distance)
			{
				finalRayResult = rayResult;
			}
		}
	}

	//補正分の長さを結果に反映＆着地判定

	float distanceFromGround = FLT_MAX;

	//足元にステージオブジェクトがあった
	if (finalRayResult.m_hit)
	{
		//地面との距離を算出
		distanceFromGround = finalRayResult.m_distance - (m_prevPos.y - Pos.y);
	}

	//上方向に力がかかっていた場合
	if (m_force.y > 0.0f)
	{
		//着地禁止
		m_isGround = false;
	}
	else
	{
		//地面からの距離が(歩いて乗り越えられる高さ＋地面から足が離れていても着地判定する高さ)未満であれば着地とみなす
		m_isGround = (distanceFromGround < (s_allowToStepHeight + s_landingHeight));
	}

	//地面との距離を格納
	rDstDistance = distanceFromGround;

	//着地したかどうかを返す
	return m_isGround;
}

#include"Crystal.h"
void Enemy::ActionCrash::Update(Enemy& owner)
{
	owner.SetRespawnTime(60);
	owner.SetHp(20);
	owner.m_LetsMove = false;

	std::string s = "json";
	 
	std::shared_ptr<Crystal> spCrystal = std::make_shared<Crystal>();
	spCrystal->Deserialize(ResFac.GetJSON("Data/Scene/Crystal."+s));
	spCrystal->SetMatrix(owner.GetMatrix());
	Scene::GetInstance().AddObject(spCrystal);
}

void Enemy::ActionEscape::Update(Enemy& owner)
{
	m_sphuman = Scene::GetInstance().FindObjectWithName("PlayerHuman");
	Vec3 pos;
	pos.x -= 0.2f;
	Matrix a=owner.GetMatrix();
	a.Move(pos);
	owner.SetMatrix(a);
}

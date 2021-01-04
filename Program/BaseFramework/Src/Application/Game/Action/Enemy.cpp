#include"Enemy.h"
#include"SnowBall.h"
#include"Wall.h"
#include"Human.h"
#include"../Scene.h"
#include"../../Component//CameraComponent.h"
#include"../../Component//ModelComponent.h"
#include"../../Component//InputComponent.h"

const float Enemy::s_allowToStepHeight=0.8f;
const float Enemy::s_landingHeight=0.5f;

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
	
	SetAnimation("Walk");
}

void Enemy::Update()
{
	if (m_RespawnTime >0)
	{
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

	frame++;

	if(m_spInputComponent)
	{
		m_spInputComponent->Update();
	}

	//カーソル固定解除
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		m_spInputComponent->m_base=false;
	}

	if (m_spActionState)
	{
		m_spActionState->Update(*this);
	}
	
	//移動前の座標を覚える
	m_prevPos = m_pos;

	Reload();
	MakeWall();
	//SetTarget(m_sphuman->shared_from_this());

	UpdateShoot();
 
	CheckBump();

	if (m_canTrace)
	{
		m_force = m_mWorld.GetTranslation();
		Trace();
	}
	else
	{
		//重力をキャラクターのYの移動力に加える
		m_force.y -= m_gravity;

		UpdateRotate(m_force);
		m_mWorld.CreateRotationX(m_rot.x);
		m_mWorld.RotateY(m_rot.y);
		m_pos.Move(m_force);
		m_mWorld.Scale(m_scale.x, m_scale.y, m_scale.z);
		m_mWorld.Move(m_pos);
		if (frame %30==0){i = rand() % 9;}

		if (i % 2 == 0)
		{
			m_force.z = 0;
			if (m_mWorld.GetTranslation().x < m_EnemyMat[i].GetTranslation().x) { m_force.x = 0.02f; }
			if (m_mWorld.GetTranslation().x > m_EnemyMat[i].GetTranslation().x) { m_force.x = -0.02f; }
			m_force.z = 0.0f;
		}
		else
		{
			m_force.x = 0;
			if (m_mWorld.GetTranslation().z > m_EnemyMat[i].GetTranslation().z) { m_force.z = -0.02f; }
			if (m_mWorld.GetTranslation().z < m_EnemyMat[i].GetTranslation().z) { m_force.z = 0.02f; }
			m_force.x = 0.0f;
		}
	}

	UpdateCollision();

	m_animator.AdvanceTime(m_spModelComponent->GetChangeableNodes());

}

void Enemy::SetAnimation(const char* pAnimName)
{
	/*if (m_spModelComponent)
	{
		std::shared_ptr<KdAnimationData> animData = m_spModelComponent->GetAnimation(pAnimName);
		m_animator.SetAnimation(animData);
	}*/
}

void Enemy::Damage(int damage)
{
	m_hp -= damage;
	m_force.y = 0.1f;

	//if (m_hp < 10){	m_spActionState = std::make_shared<ActionEscape>();}
}

bool Enemy::CanShoot()
{
	if (m_charge == 0) { return false; }
	return true;
}

bool Enemy::CanMakeWall()
{
	m_canWall -= 0.01f;
	if (m_canWall <= 0) { return false; }
	return true;
}

void Enemy::Trace()
{
	Vec3 Dir = m_sphuman->GetMatrix().GetTranslation() - m_mWorld.GetTranslation();

	Dir.Normalize();
	UpdateRotate(Dir);

	Matrix copyMat = m_mWorld;
	m_mWorld.CreateRotationX(m_rot.x);
	m_mWorld.RotateY(m_rot.y);
	m_mWorld.Scale(m_scale.x, m_scale.y, m_scale.z);
	m_mWorld.SetTranslation(copyMat.GetTranslation());
	m_pos = Dir * 0.02f;
	if(m_pos.y<0){m_pos.y = 0;}
	m_mWorld.Move(m_pos);
	//m_mWorld._42 = -0.7f;
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
				mLaunch.RotateX((360 * KdToRadians) + Shot);
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

void Enemy::MakeWall()
{
	if (m_spInputComponent->GetButton(Input::Buttons::B))
	{
		if (m_makeWall)
		{
			//m_gather += 2;
			m_notMove = true;

			std::shared_ptr<Wall> spWall = std::make_shared<Wall>();
			if (spWall)
			{
				if (m_force.LengthSquared() > 0.0f) { return; }

				spWall->Deserialize(ResFac.GetJSON("Data/Scene/Wall.json"));

				Matrix mLaunch;
				//Matrix mScale;
				mLaunch.CreateTranslation(0.0f, -0.5f, 5.0f);
				mLaunch.Scale(2.0f, 1.5, 0.5f);

				mLaunch *= m_mWorld;

				spWall->SetMatrix(mLaunch);

				spWall->SetOwner(shared_from_this());

				Scene::GetInstance().AddObject(spWall);

				//m_snow -= 2;
			}
			m_makeWall = false;
		}
	}
	else {
		m_makeWall = true;
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
	sInfo.m_radius = 5.0f;

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		if (obj.get() == this) { continue; }

		if (obj->GetTag() & TAG_Player)
		{
			if (obj->HitCheckBySphere(sInfo))
			{
				//M = obj->GetMatrix();
				m_canTrace=true;
			}
			else {
				m_canTrace = false;
			}
		}
	}

	//Scene::GetInstance().AddDebugSphereLine(m_mWorld.GetTranslation(), 5.0f, { 1.0f,0.0f,0.0f,1.0f });

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
	info.m_pos.y += 0.4f;	//キャラクターのぶつかり判定をするので、ちょっと上に持ち上げる
	info.m_radius = 0.3f;	//キャラクターの大きさに合わせて半径サイズもいい感じに設定する

	Scene::GetInstance().AddDebugSphereLine(info.m_pos, info.m_radius, { 1.0f,1.0f,1.0f,1.0f });

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

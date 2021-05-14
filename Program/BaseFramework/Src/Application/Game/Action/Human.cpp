#include"Human.h"
#include"SnowBall.h"
#include"Wall.h"
#include"Lift.h"
#include"SnowManager.h"
#include"../Scene.h"
#include"../../Component//CameraComponent.h"
#include"../../Component//InputComponent.h"
#include"../../Component//ModelComponent.h"
#include"SnowManager.h"
#include"../DebugLine.h"

const float Human::s_allowToStepHeight=0.8f;
const float Human::s_landingHeight=0.1f;

void Human::Deserialize(const json11::Json& jsonObj)
{

	ShowCursor(false);

	GameObject::Deserialize(jsonObj);
	
	if (m_spCameraComponent)
	{
		m_spCameraComponent->OffsetMatrix().CreateTranslation(0.0f, 0.8f, 0.2f);
		m_spCameraComponent->OffsetMatrix().RotateX(5.0f * ToRadians);
		m_spCameraComponent->OffsetMatrix().RotateY(270.0f * ToRadians);
	}

	if ((GetTag() & TAG_Player) != 0)
	{
		Scene::GetInstance().SteTargetCamera(m_spCameraComponent);
		m_spInputComponent = std::make_shared<ActionPlayerInputComponent>(*this);
	}

	SetAnimation("Walk");
	const std::vector<json11::Json>& rRot = jsonObj["Rot"].array_items();
	m_power = jsonObj["Power"].int_value();

	m_CamMat.RotateZ((-5 * ToRadians));
	m_CamMat.RotateY((-140 * ToRadians));
	m_spCameraComponent->SetCameraMatrix(m_CamMat);

	m_pos = m_mWorld.GetTranslation();
	m_gravity = 0.008f;

	m_spReticleTex = ResFac.GetInstance().GetTexture("Data/Texture/UITexture/UI_Reticle.png");
}

void Human::Update()
{

	if (m_spInputComponent) { m_spInputComponent->Update(); }

	m_spInputComponent->SetBase(m_base);
	frame++;

	//移動前の座標を覚える
	m_prevPos = m_pos;

	UpdaetCamera();

	UpdateShoot();
	MakeWall();

	//重力をキャラクターのYの移動力に加える
	m_force.y -= m_gravity;
	
	UpdateCollision();

	DrainSnow();
	UpdateMove();
	Crouch();

	//移動力をキャラクターの座標に足しこむ
	m_pos.Move(m_force);
		
	m_mWorld.CreateScalling(m_scale.x, m_scale.y, m_scale.z);

	m_mWorld.Move(m_pos);

	Matrix drawMat;
	drawMat = m_mWorld;
	m_mWorld.SetBillboard(SHADER.m_cb7_Camera.GetWork().mV);		//カメラ行列の取得

	//座標は自分のものを使う
	m_mWorld.SetTranslation(drawMat.GetTranslation());

	if(m_hit||m_movespeed<0.05f)
	{
		m_movespeed += 0.001f;
		m_hit = false;
	}

	m_animator.AdvanceTime(m_spModelComponent->GetChangeableNodes());

}

void Human::SetAnimation(const char* pAnimName)
{
	if (m_spModelComponent)
	{
		std::shared_ptr<KdAnimationData> animData = m_spModelComponent->GetAnimation(pAnimName);
		m_animator.SetAnimation(animData);
	}
}


void Human::Damage(float damage)
{
	m_hitCntOne++;
	if (m_hitCntOne < 9){
		m_hitCntTen++;
	}

	m_hit = true;
	m_movespeed *=  0.70f;

	m_hp -= damage;
	m_hpScroll += 14;	//HP表示拡縮に合わせて場所をずらす(調整)
	
	m_notMove = true;
	//m_force.z = m_spCameraComponent->GetCameraMatrix().GetAxisZ() * -0.1;
	//m_force.y = 0.2f;

	if (m_hp <= 0)
	{
		m_spInputComponent->m_base = false;
		ShowCursor(true);
		Scene::GetInstance().RequestChangeScene("Data/Scene/Result.json");
		//m_spActionState = std::make_shared<ActionDie>();
	}
}

bool Human::CanAction()
{
	if(!m_canmGather) { return false; }		//雪溜め中それしかできない
	
	return true;
}

void Human::DrainSnow()
{
	if (m_spInputComponent->GetButton(Input::Buttons::X))
	{
		if (!m_CanDrain) { return; }
		m_CrystalDrain += 0.001f;
		if (m_SnowBallNum >= 5) { return; }
		m_snow += 0.1f;
		m_gather += 0.1f;
		m_CanShoot = false;
		m_canmGather = false;
	
		if (m_snow >= 3)
		{ 
			m_SnowBallNum++;
			m_snow   = 0; 
			m_gather = -3.0f;
		}
	}
	else {
		m_canmGather = true;
		m_CanDrain = false;
	}
}

void Human::UpdateShoot()
{
	if (m_spInputComponent == nullptr) { return; }

	if (!CanAction()) { return; }		//溜めているとき
	
	if (m_spInputComponent->GetButton(Input::Buttons::A))
	{
		m_CanShoot = true;
	}
	else
	{
		if (m_CanShoot)
		{

			if (m_SnowBallNum <= 0.0f) { return; }
			m_SnowBallNum--;
			std::shared_ptr<SnowBall> spSnowBall = std::make_shared< SnowBall>();
			if (spSnowBall)
			{
				spSnowBall->Deserialize(ResFac.GetJSON("Data/Scene/SnowBall.json"));

				Matrix mLaunch;
				mLaunch.CreateTranslation(0.0f, 7.0f, 3.0f);
				mLaunch.RotateX(350 * ToRadians);
				mLaunch.Scale(0.2f, 0.2f, 0.2f);
				mLaunch *= m_mWorld;

				spSnowBall->SetMatrix(mLaunch);
				spSnowBall->SetPower(3);

				spSnowBall->SetOwner(shared_from_this());

				Scene::GetInstance().AddObject(spSnowBall);
				Scene::GetInstance().SetShotCnt(1);

				float minDistance = FLT_MAX;
			}
		}
		m_CanShoot = false;
	}
}

void Human::MakeWall()
{
	if (!CanAction()) { return; }		//溜めているとき
	if (m_snow <= 2) { return; }		//雪が足りなかったら帰る

	if (m_spInputComponent->GetButton(Input::Buttons::B))
	{
		if (m_makeWall)
		{
			m_gather -= 2;

			std::shared_ptr<Wall> spWall = std::make_shared<Wall>();
			if (spWall)
			{
				if (m_force.LengthSquared() > 0.0f) { return; }

				spWall->Deserialize(ResFac.GetJSON("Data/Scene/Wall.json"));

				Matrix mLaunch;
				mLaunch.Scale(2.0f, 1.5, 0.5f);
				mLaunch.CreateTranslation(0.0f, -0.5f, 2.0f);

				mLaunch *= m_mWorld;

				spWall->SetMatrix(mLaunch);
				
				spWall->SetOwner(shared_from_this());

				Scene::GetInstance().AddObject(spWall);

				m_snow -= 2;
			}
			m_makeWall = false;
		}
	}
	else {
		m_makeWall = true;
	}
}

void Human::UpdateMove()
{

	if (!m_spCameraComponent) { return; }

	const Math::Vector2& inputMove = m_spInputComponent->GetAxis(Input::Axes::L);
	Vec3 moveSide = m_spCameraComponent->GetCameraMatrix().GetAxisX() * inputMove.x;
	Vec3 moveForward = m_spCameraComponent->GetCameraMatrix().GetAxisZ() * inputMove.y;

	//上下の移動成分はカット
	moveForward.y = 0.0f;

	Vec3 moveVec = moveSide + moveForward;

	moveVec.Normalize();

	//移動速度補正
	moveVec *= m_movespeed;

	m_force.x = moveVec.x;
	m_force.z = moveVec.z;

	m_crouch = 0.05f+sin((float)frame*0.3f) * 0.03f;
	if (m_spInputComponent->GetButton(Input::Buttons::Y))
	{
		if (!m_jumpflg||!m_isGround) { return; }
		m_force.y=m_jumpPow;
		m_jumpflg = false;
	}
	else
	{
		m_jumpflg = true;
	}
}

void Human::Crouch()
{
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_force.z *= 0.5f;
		m_force.x *= 0.5f;
	}
	if (m_spInputComponent->GetButton(Input::Buttons::SHIFT))
	{
		if (m_crouch < -0.5f) { m_crouchSpeed = 0; return; }
		m_crouchSpeed -= 0.01f;
		m_crouch += m_crouchSpeed;
	}
	else
	{
		if (m_crouch > 0.0f) { m_crouchSpeed = 0; return; }
		m_crouchSpeed += 0.01f;
		m_crouch += m_crouchSpeed;
	}

}

void Human::UpdaetCamera()
{
	if (!m_spCameraComponent) { return; }
	if (m_notMove) { return; }

	const Math::Vector2& inputRotate = m_spInputComponent->GetAxis(Input::R);

	Vec3 moveVec = { -inputRotate.x,0.0f,-inputRotate.y };
	moveVec *= m_movespeed;

	float DeltaX = moveVec.x;
	float DeltaY = moveVec.z;

	Matrix mRotY, mRotX;
	mRotY.RotateY(DeltaX * ToRadians);
	m_CamMat *= mRotY;

	Vec3 AxisX = m_CamMat.GetAxisZ();
	AxisX.Normalize();
	mRotX = DirectX::XMMatrixRotationAxis(AxisX, DeltaY * ToRadians);
	m_CamMat *= mRotX;

	m_CamMat.SetTranslation(m_pos.x, m_pos.y + m_crouch, m_pos.z);
	m_spCameraComponent->SetCameraMatrix(m_CamMat);
}

void Human::UpdateCollision()
{
	float distanceFromGround = FLT_MAX;

	CheckBump();

	//下方向への判定を行い、着地した
	if (CheckGround(distanceFromGround, TAG_StageObject, m_pos))
	{
		m_force *= 0.99;
		//地面の上にy座標を移動
		m_pos.y += s_allowToStepHeight - distanceFromGround;
		//地面があるので、y方向への移動量は0に
		m_force.y = 0.0f;
		m_notMove = false;
		return;
	}

}
void Human::CheckBump()
{
	SphereInfo info;
	
	info.m_pos = m_pos;		//中心点キャラクターの位置
	info.m_pos.y += 0.4f;	//キャラクターのぶつかり判定をするので、ちょっと上に持ち上げる
	info.m_radius = 0.2f;	//キャラクターの大きさに合わせて半径サイズもいい感じに設定する

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		if (obj.get() == this) { continue; }

		SphereResult Result;
		if (obj->GetTag() == TAG_Spring)
		{
			if (obj->HitCheckBySphereVsMesh(info, Result))
			{
				m_force.y += 0.4;
			}
		}
		else if (obj->HitCheckBySphereVsMesh(info, Result))
		{
			m_pos.Move(Result.m_push);
		}
	}


}

bool Human::CheckGround(float& rDstDistance, UINT m_tag, Vec3 Pos)
{

	RayInfo rayInfo;
	rayInfo.m_pos = Pos;

	//キャラの足元からレイを発射すると地面と当たらないので少し持ち上げる(乗り越えられる段差の高さ分だけ)
	rayInfo.m_pos.y += s_allowToStepHeight;

	//落下中かもしれないので、1フレーム前の座標分も持ち上げる
	rayInfo.m_pos.y += m_prevPos.y - Pos.y;

	//地面方向へのレイ
	rayInfo.m_dir = { 0.0f,-1.0f,0.0f };
	rayInfo.m_maxRange = 15;

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

void Human::Draw2D()
{
	SnowManager snow;
	snow.Draw2DHP(m_hp, m_hpScroll);
	snow.Draw2DTex(m_snow, m_gather);

	Vec3 pos;
	pos.SetPos(110, -330, 0);
	snow.DrawTex("UI_SnowBallGage", pos);

	for (int i = 0; m_SnowBallNum > i; i++)
	{
		snow.Draw2DRemaining(i);
	}

	m_ReticleMat.CreateScalling(m_Scale, m_Scale, m_Scale);
	SHADER.m_spriteShader.SetMatrix(m_ReticleMat);
	SHADER.m_spriteShader.DrawTex(m_spReticleTex.get(), 0, 0);

}
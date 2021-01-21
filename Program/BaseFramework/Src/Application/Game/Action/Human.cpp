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

const float Human::s_allowToStepHeight=0.8f;
const float Human::s_landingHeight=0.1f;

void Human::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	
	if (m_spCameraComponent)
	{
		m_spCameraComponent->OffsetMatrix().CreateTranslation(0.0f, 1.0f, 0.0f);
		m_spCameraComponent->OffsetMatrix().RotateX(5.0f * KdToRadians);
		m_spCameraComponent->OffsetMatrix().RotateY(270.0f * KdToRadians);
	}

	if ((GetTag() & TAG_Player) != 0)
	{
		Scene::GetInstance().SteTargetCamera(m_spCameraComponent);
		m_spInputComponent = std::make_shared<ActionPlayerInputComponent>(*this);
	}

	SetAnimation("Stand");
	
	m_spActionState = std::make_shared<ActionWait>();
	m_pos = m_mWorld.GetTranslation();
	m_gravity = 0.008;
}

void Human::Update()
{

	if(m_spInputComponent){m_spInputComponent->Update();}

	//カーソル固定解除
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000){m_spInputComponent->m_base=false;}
	
	frame++;

	//移動前の座標を覚える
	m_prevPos = m_pos;

	UpdaetCamera();

	UpdateShoot();
	MakeWall();

	//重力をキャラクターのYの移動力に加える
	m_force.y -= m_gravity;
	
	UpdateCollision();

	ChargeSnow();

	//移動力をキャラクターの座標に足しこむ
	m_pos.Move(m_force);

	//m_mWorld.CreateRotationX(m_rot.x);
	//m_mWorld.RotateX(350 * KdToRadians);
		
	m_mWorld.CreateScalling(m_scale.x, m_scale.y, m_scale.z);

	m_mWorld.Move(m_pos);
		
	if (m_spActionState)
	{
		m_spActionState->Update(*this);
	}

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
	/*if (m_spModelComponent)
	{
		std::shared_ptr<KdAnimationData> animData = m_spModelComponent->GetAnimation(pAnimName);
		m_animator.SetAnimation(animData);
	}*/
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
		m_spActionState = std::make_shared<ActionDie>();
	}
}

bool Human::CanAction()
{
	if (m_snow <= 0) { return false; }
	return true;
}

void Human::ChargeSnow()
{
	if (m_spInputComponent->GetButton(Input::Buttons::X))
	{
		//m_notMove = true;
		m_snow   += 0.1f;
		m_gather -= 0.1f;

		
		if (m_snow >= 3)
		{ 
			m_snow   = 3; 
			m_gather = 0.0f;
		}
	}
}

void Human::UpdateShoot()
{
	if (m_spInputComponent == nullptr) { return; }

	if (!CanAction()) { return; }

	if (m_spInputComponent->GetButton(Input::Buttons::A))
	{
		m_canShoot = true;
	}
	else
	{
		if (m_canShoot)
		{
			m_gather++;
			m_snow--;
			std::shared_ptr<SnowBall> spSnowBall = std::make_shared< SnowBall>();
			if (spSnowBall)
			{
				spSnowBall->Deserialize(ResFac.GetJSON("Data/Scene/SnowBall.json"));

				Matrix mLaunch;
				mLaunch.CreateTranslation(5.0f, 10.0f, 0.3f);
				mLaunch.RotateX(340 * KdToRadians);
				mLaunch.Scale(0.2f, 0.2f, 0.2f);
				mLaunch *= m_mWorld;

				spSnowBall->SetMatrix(mLaunch);

				spSnowBall->SetOwner(shared_from_this());

				Scene::GetInstance().AddObject(spSnowBall);

				float minDistance = FLT_MAX;
			}
		}
		m_canShoot = false;
	}
}

void Human::MakeWall()
{
	if (!CanAction()) { return; }

	if (m_spInputComponent->GetButton(Input::Buttons::B))
	{
		if (m_makeWall)
		{
			m_gather += 2;
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
	if (m_notMove) { return; }

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
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_force.z *= 2.5f;
		m_force.x *= 2.5f;
	}
}

void Human::UpdaetCamera()
{
	if (!m_spCameraComponent) { return; }
	if (m_notMove) { return; }

	const Math::Vector2& inputRotate = m_spInputComponent->GetAxis(Input::R);

	//m_spCameraComponent->OffsetMatrix().RotateY(inputRotate.x * m_camRotSpeed * KdToRadians);

	Vec3 moveVec = { inputRotate.x,0.0f,inputRotate.y };
	moveVec *= m_movespeed;

	float DeltaX = moveVec.x;
	float DeltaY = moveVec.z;

	Matrix mRotY, mRotX;
	mRotY.RotateY(DeltaX * KdToRadians);
	m_CamMat *= mRotY;

	Vec3 AxisX = m_CamMat.GetAxisZ();
	AxisX.Normalize();
	mRotX = DirectX::XMMatrixRotationAxis(AxisX, DeltaY * KdToRadians);
	m_CamMat *= mRotX;

	m_CamMat.SetTranslation(m_pos);
	m_spCameraComponent->SetCameraMatrix(m_CamMat);

	/*Matrix m_CamMat=m_spCameraComponent->GetCameraMatrix();
	if (!m_spCameraComponent) { return; }
	if (m_notMove) { return; }

	const Math::Vector2& inputRotate = m_spInputComponent->GetAxis(Input::R);
	*/
}

void Human::UpdateCollision()
{
	float distanceFromGround = FLT_MAX;

	CheckBump();

	//下方向への判定を行い、着地した
	if (CheckGround(distanceFromGround, TAG_StageObject, m_pos))
	{
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
	info.m_pos.y += 0.8f;	//キャラクターのぶつかり判定をするので、ちょっと上に持ち上げる
	info.m_radius = 0.4f;	//キャラクターの大きさに合わせて半径サイズもいい感じに設定する

	Scene::GetInstance().AddDebugSphereLine(info.m_pos, info.m_radius);

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		if (obj.get() == this) { continue; }

		SphereResult Result;
		if (obj->GetTag() == TAG_Spring)
		{
			if (obj->HitCheckBySphereVsMesh(info, Result))
			{
				m_force.y += 0.3;
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

void Human::Draw2D()
{
	SnowManager snow;
	snow.Draw2DHP(m_hp, m_hpScroll);
	snow.Draw2DTex(m_snow, m_gather);
}

bool Human::IsChangeMove()
{
	if (!m_spInputComponent) { return false; }
	const Math::Vector2& inputMove = m_spInputComponent->GetAxis(Input::Axes::L);
	// 移動していたら
	if (inputMove.LengthSquared() != 0.0f) { return true; }
	return false;
}

bool Human::IsChangeJump()
{
	if (!m_spInputComponent) { return false; }
	// ジャンプできる？
	//if(CanJump())
	//{ 
		// ジャンプボタン押された？
	if (m_spInputComponent->GetButton(Input::Buttons::Y) & m_spInputComponent->ENTER && m_isGround)
	{
		return true;
	}
	//} 
	return false;
}

void Human::ChangeWait()
{
	m_force.x = 0.0f;
	m_force.z = 0.0f;
	// 待機アクションへ遷移
	m_spActionState = std::make_shared<ActionWait>();
}

void Human::ChangeMove()
{
	// 移動アクションへ遷移
	m_spActionState = std::make_shared<ActionWalk>();
}

void Human::ChangeJump()
{
	m_force.y = m_jumpPow;

	//ジャンプアクションへ遷移
	m_spActionState = std::make_shared<ActionJump>();
}

void Human::ActionJump::Update(Human& rOwner)
{
	rOwner.UpdateMove();

	if (rOwner.IsGround())
	{
		rOwner.ChangeWait();// 停止していたら

		return;
	}

}

void Human::ActionWalk::Update(Human& rOwner)
{
	rOwner.UpdateMove();

	// 待機する？
	if (!rOwner.IsChangeMove())
	{
		rOwner.ChangeWait();

		//待機アニメに変更
		rOwner.SetAnimation("Stand");

		return;
	}
	// ジャンプする？
	if (rOwner.IsChangeJump())
	{
		rOwner.ChangeJump();
		return;
	}
}

void Human::ActionWait::Update(Human& rOwner)
{

	// 移動する？
	if (rOwner.IsChangeMove())
	{
		rOwner.ChangeMove();

		//歩きアニメに変更
		rOwner.SetAnimation("Walk");

		return;
	}
	// ジャンプする？
	if (rOwner.IsChangeJump())
	{
		rOwner.ChangeJump();
		return;
	}
}
#include"Enemy.h"
void Human::ActionDie::Update(Human& owner)
{
	std::shared_ptr<Lift> spLift = std::make_shared<Lift>();

	spLift->Deserialize(ResFac.GetJSON("Data/Scene/Lift.json"));
	Scene::GetInstance().AddObject(spLift);

	owner.Destroy();
}
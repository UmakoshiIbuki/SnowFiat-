#include "HumanMini.h"
#include "Human.h"
#include "../Scene.h"
#include"../../Component//InputComponent.h"
#include"../../Component//CameraComponent.h"

void HumanMini::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);	

	if (m_spCameraComponent)
	{
		m_spCameraComponent->OffsetMatrix().CreateTranslation(0.3f, 2.0f, 0.0f);
		m_spCameraComponent->OffsetMatrix().RotateX(10.0f * ToRadians);
		m_spCameraComponent->OffsetMatrix().RotateY(250.0f * ToRadians);
	}

	if ((GetTag() & TAG_Mini) != 0)
	{
		m_spInputComponent = std::make_shared<ActionPlayerInputComponent>(*this);
		m_spInputComponent->m_base = false;
	}
}

void HumanMini::Update()
{

	if(m_spInputComponent)
	{
		m_spInputComponent->Update();
	}

	
	//移動前の座標を覚える
	m_prevPos = m_pos;

	UpdaetCamera();

	UpdateMove();

	//重力をキャラクターのYの移動力に加える

	//移動力をキャラクターの座標に足しこむ
	m_pos.Move(m_force);

	m_mWorld.CreateRotationX(m_rot.x);
	m_mWorld.RotateY(m_rot.y);
	m_mWorld.RotateZ(m_rot.z);
	m_mWorld.Move(m_pos);

}

void HumanMini::UpdateMove()
{
	if (!m_spCameraComponent) { return; }

	const Math::Vector2& inputMove = m_spInputComponent->GetAxis(Input::Axes::L);
	Vec3 moveSide = m_spCameraComponent->GetCameraMatrix().GetAxisX() * inputMove.x;
	Vec3 moveForward = m_spCameraComponent->GetCameraMatrix().GetAxisZ() * inputMove.y;

	//上下の移動成分はカット
	moveForward.y = 0.0f;
	Vec3 moveVec = moveSide + moveForward;

	moveVec.Normalize();

	//キャラクターの回転処理
	UpdateRotate(moveVec);

	moveVec *= m_movespeed;

	m_force.x = moveVec.x;
	m_force.z = moveVec.z;
}

void HumanMini::UpdaetCamera()
{
	if (!m_spCameraComponent) { return; }

	const Math::Vector2& inputRotate = m_spInputComponent->GetAxis(Input::R);

	m_spCameraComponent->OffsetMatrix().RotateY(inputRotate.x * m_camRotSpeed * ToRadians);
}

//rMoveDir:移動方向
void HumanMini::UpdateRotate(const Vec3& rMoveDir)
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
	rotateRadian = std::clamp(rotateRadian, -m_rotateAngle * ToRadians, m_rotateAngle * ToRadians);

	m_rot.y += rotateRadian;
}
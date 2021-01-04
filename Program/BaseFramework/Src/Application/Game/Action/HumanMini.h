#pragma once
#include"../GameObject.h"

class HumanMini :public GameObject
{
public:

	virtual void Deserialize(const json11::Json& jsonObj) override;
	virtual void Update()override;



private:

	void UpdateMove();					    //移動更新処理
	float		m_movespeed = 0.2f;		    //移動スピード

	void UpdaetCamera();
	float m_camRotSpeed = 0.2f;
	Vec3		m_pos;

	void UpdateRotate(const Vec3& rMoveDir);			//操作やキャラクターの行動による回転計算

	float		m_rotateAngle = 10.0f;					//キャラクターの回転速度

	Vec3		m_rot;								    //ワールド行列の回転角度 
	Vec3		m_force;								//キャラクターにかかる力
	Vec3		m_prevPos;
	
	UINT		m_tag = OBJECT_TAG::TAG_None;

	float		m_speed = 0.2f;							//移動スピード

};
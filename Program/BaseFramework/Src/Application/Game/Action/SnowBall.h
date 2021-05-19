#pragma once
#include"../GameObject.h"

class Particle;

class SnowBall : public GameObject
{
public:

	void Deserialize(const json11::Json& jsonObj)override;	//デシリアライズ(初期化)
	void Update();		//更新

	inline void SetTarget(const std::shared_ptr<GameObject>& spTarget) { m_wpTarget = spTarget; }

	void UpdateCollision();//当たり判定処理

	inline void SetOwner(const std::shared_ptr<GameObject>& spOwner) { m_wpOwner = spOwner; }

	void ParticleEffect();

	void DrawEffect()override;	//透明物の描画
	void SetSpeed(float speed) { m_speed = speed; }
	void SetGravity(float gravity) { m_gravity = gravity; }
	void SetPower(float power) { m_power = power; }
private:

	Vec3  m_prevPos;		//1フレーム前の座標

	std::weak_ptr<GameObject>m_wpOwner;		//発射したオーナーオブジェクト
	int   m_shotCnt = 0;
	int   m_hitCnt = 0;
	int	  m_lifeSpan = 100;
	float m_speed = 0.0f;
	float m_power = 0.0f;

	std::weak_ptr<GameObject>m_wpTarget;

	float m_trailRotate = 0.0f;
	int frame = 0;

	static const int	      particleNum = 200;
	std::shared_ptr<Particle> particleSnow[particleNum];

	int	  count = 0;
	int   AttackCnt;


};
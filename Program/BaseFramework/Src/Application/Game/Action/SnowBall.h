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

	void UpdateTrail();			//軌跡の更新
	void DrawEffect()override;	//透明物の描画
private:

	Vec3  m_prevPos;		//1フレーム前の座標

	std::weak_ptr<GameObject>m_wpOwner;		//発射したオーナーオブジェクト
	float m_speed = 0.0f;
	//float gravity = 0.0f;
	int	  m_lifeSpan = 50;
	std::weak_ptr<GameObject>m_wpTarget;

	TrailPolygon m_trailSmoke;
	float m_trailRotate = 0.0f;
	int frame = 0;

	std::shared_ptr<Particle> particleSnow[200];

	int count = 0;

	int AttackCnt;

};
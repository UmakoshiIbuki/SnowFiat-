#pragma once
#include"../GameObject.h"

class Lift:public GameObject
{
public:
	void Deserialize(const json11::Json& jsonObj)override;
	void Update()override;

private:
	Matrix m_mStart;
	Vec3 m_vGoal = {};
	float m_speed = 0.0f;
	float m_progress = 0.0f;//ゴールに向かっている割合(0-1)
	bool  m_goTo = true;	//ゴール地点に向かっている(true)、折り返している(false)
	std::shared_ptr<GameObject> m_sphuman;
	float m_posY;
};
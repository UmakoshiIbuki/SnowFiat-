#pragma once
#include "../GameObject.h"

class Crystal:public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void UpdateCollision();

	void Update();	

private:
	bool m_CanDrain = false;
	float m_dissolveThreshold;
	float m_rot;
	Matrix m_rotMat;

	Vec3 pos;
};
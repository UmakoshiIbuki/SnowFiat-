#pragma once
#include "../GameObject.h"

class Crystal:public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void UpdateCollision();

	void Update();	

private:
	float m_rot;
	Matrix m_rotMat;

	Vec3 pos;
};
#pragma once
#include "../GameObject.h"

class Crystal:public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void UpdateCollision();

	void Update();	
	void Draw2D()override;

private:

	int		m_frame;
	bool	m_CanDrain = false;
	float	m_dissolveThreshold;
	float	m_rot;
	float	m_LightPower;
	Vec3	m_pos;
	Matrix	m_rotMat;

	std::shared_ptr<KdTexture> m_spTex;
	std::shared_ptr<KdTexture> m_spTex1;

};
#pragma once
#include"../GameObject.h"

class Title :public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;

	void Update()override;

	void Draw2D()override;

	virtual void ImguiUpdate()override;

private:

	int frame=0;

	std::shared_ptr<KdTexture> m_spTitleTex;
	Matrix					   m_TitleMat;

	std::shared_ptr<KdTexture> m_spSelectTex;
	Matrix					   m_SelectMat;
	float m_scale = 1;

	bool m_canChange = false;
	float m_black = 1.0f;
	bool flg=false;
	Vec3 MousePos;
};
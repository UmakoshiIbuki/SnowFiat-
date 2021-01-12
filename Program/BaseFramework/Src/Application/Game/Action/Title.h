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
	Vec3					   m_SelectPos;
	float					   m_scale = 0.7f;

	std::shared_ptr<KdTexture> m_spSelect001Tex;
	Matrix					   m_Select001Mat;
	Vec3					   m_Select001Pos;
	float					   m_scale001 = 0.7f;

	std::shared_ptr<KdTexture> m_spSelect002Tex;
	Matrix					   m_Select002Mat;
	Vec3					   m_Select002Pos;
	float					   m_scale002 = 0.7f;

	bool					   m_canChange = false;
	float                      m_black = 1.0f;
	bool                       flg=false;
	Vec3                       MousePos;
};
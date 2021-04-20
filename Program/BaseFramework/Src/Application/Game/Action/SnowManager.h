#pragma once
#include"../GameObject.h"

class SnowManager :public GameObject
{
public:

	virtual void Deserialize(const json11::Json& jsonObj) override;
	void Draw2DTex(float f1,float f2);
	void Draw2DHP(float f1,int f2);
	void Draw2D();
	void Draw2DRemaining(int snow);
	void DrawTex(std::shared_ptr<KdTexture> TexFile, Vec3	Pos);
	void Update()override;
	
private:
	void ChangeTex(float m_snow);
	std::shared_ptr<KdTexture> m_spSnowRemainingTex;	//雪玉の残弾数
	Matrix					   m_SnowRemainingMat;		//描画位置

	std::shared_ptr<KdTexture> m_spBackTex;
	Matrix					   m_spBackMat;
	
	std::shared_ptr<KdTexture> m_spSnowGageTex;
	Matrix					   m_SnowGageMat; 
	std::shared_ptr<KdTexture> m_spSnowGageTex2;
	Matrix					   m_SnowGageMat2;

	std::shared_ptr<KdTexture> m_spHpTex;
	Matrix					   m_HpMat;

	std::shared_ptr<KdTexture> m_spHpBerTex;

	std::shared_ptr<KdTexture> m_spParticleTex;

};

#define RND (rand()/(double)RAND_MAX)
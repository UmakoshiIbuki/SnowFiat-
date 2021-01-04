#pragma once
#include"../GameObject.h"

class SnowManager :public GameObject
{
public:

	virtual void Deserialize(const json11::Json& jsonObj) override;
	void Draw2DTex(float f1,float f2);
	void Draw2DHP(float f1,int f2);
	void Draw2D();
	void Draw2DRemaining(float snow);
	void Update()override;
	
	void EmitParticle(struct PARTICLE* p, float aPosX, float aPosY, float aMoveX, float aMoveY, int size, int LifeSpan);
	void UpdateParticle(struct PARTICLE* p);
	void DrawParticle(struct PARTICLE* p);

private:
	void ChangeTex(float m_snow);
	std::shared_ptr<KdTexture> m_spSnowRemainingTex;	//雪玉の残弾数
	Matrix					   m_SnowRemainingMat;		//描画位置

	std::shared_ptr<KdTexture> m_spBackTex;
	Matrix					   m_spBackMat;
	
	std::shared_ptr<KdTexture> m_spSnowGageTex;
	Matrix					   m_spSnowGageMat; 
	std::shared_ptr<KdTexture> m_spSnowGageTex2;
	Matrix					   m_spSnowGageMat2;

	std::shared_ptr<KdTexture> m_spHpTex;
	Matrix					   m_HpMat;

	std::shared_ptr<KdTexture> m_spHpBerTex;

	std::shared_ptr<KdTexture> m_spParticleTex;

};

#define RND (rand()/(double)RAND_MAX)
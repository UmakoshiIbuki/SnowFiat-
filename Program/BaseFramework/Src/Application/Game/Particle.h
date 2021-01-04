#pragma once
#include"GameObject.h"

struct PARTICLE
{
	float mPosX, mPosY,mPosZ;
	float mMoveX, mMoveY;
	float mSize;
	int   mLifeSpan;
	Matrix mMat;
};

class Particle:public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void Update()override;

	void EmitParticle(struct PARTICLE* p, float aPosX, float aPosY,float aPosZ, float aMoveX, float aMoveY, int size, int LifeSpan);
	void UpdateParticle(struct PARTICLE* p);
	void DrawParticle(struct PARTICLE* p);
	void DrawEffect()override;

private:
	static const int particleNum = 25;//粒子の個数
	struct PARTICLE smoke[particleNum];//煙用

	std::shared_ptr<KdTexture> m_spParticleTex;
	int showtime = 30;
	SquarePolygon m_poly;

	float m_g=0.0f;

};
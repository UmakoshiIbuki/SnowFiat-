#pragma once
#include"../GameObject.h"

class AinmationEffect;

class Result :public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void Update()override;
	void Draw()override;
	void CrystalCount(int one, int ten);
	void DamageCount(int one, int ten);
	void HitCount(int one);
	void ShotCount(int one);
private:

	std::shared_ptr<KdTexture> m_spResultTex;
	Matrix					   m_spResultMat;

	std::shared_ptr<KdTexture> m_spCrystalCntOneTex;
	Matrix					   m_spCrystalCntOneMat;


	std::shared_ptr<KdTexture> m_spCrystalCntTenTex;
	Matrix					   m_spCrystalCntTenMat;

	std::shared_ptr<KdTexture> m_spCrystalCntTex;
	Matrix					   m_CrystalCnt[99];
	Vec3					   pos = { 200,90,0 };

	std::shared_ptr<KdTexture> m_spDamageCntOneTex;
	Matrix					   m_spDamageCntOneMat;

	std::shared_ptr<KdTexture> m_spDamageCntTenTex;
	Matrix					   m_spDamageCntTenMat;

	std::shared_ptr<KdTexture> m_spShotCntTex;
	Matrix					   m_spShotCntMat;

	std::shared_ptr<KdTexture> m_spHitCntTex;
	Matrix					   m_spHitCntMat;

	Vec3					   m_GoTitlePos;
	Matrix					   m_GoTitleMat;
	std::shared_ptr<KdTexture> m_spGoTitleTex;

	Vec3					   m_ReTryPos;
	Matrix					   m_ReTryMat;
	std::shared_ptr<KdTexture> m_spReTryTex;

	std::string s;

	int						   m_Ten = 0;
	int						   m_One = 0;

	int						   m_CrystalCntOne = 0;
	int						   m_CrystalCntTen = 0;
	int						   Crystals = 0;

	int						   m_HitCntOne = 0;
	int						   m_HitCntTen = 0;

	int						   m_AttackCnt = 0;
	int						   m_HitCnt = 0;

	int						   Weight = 1;
	int						   i = 0;

	float					   m_scale0 = 0.7f;
	float					   m_scale1 = 0.7f;

	bool					   m_CanChange = false;
	Matrix					   m_CamMat;

	std::shared_ptr<AinmationEffect> fallSnowTex[100];
	Matrix							 falleffectMat[100];
	float							 falleffectposY;
	Vec3							 m_FalleffectPos[100];
};
#pragma once
#include"../GameObject.h"

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

	std::string s;

	int m_Ten = 0;
	int m_One = 0;

	int m_CrystalCntOne = 0;
	int m_CrystalCntTen = 0;
	int Crystals = 0;

	int m_HitCntOne = 0;
	int m_HitCntTen = 0;

	int m_AttackCnt = 0;
	int m_HitCnt = 0;

	int Weight = 1;
	int i = 0;

	bool m_canChange = false;

};
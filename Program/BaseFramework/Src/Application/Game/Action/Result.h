#pragma once
#include"../GameObject.h"

class Result :public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void Update()override;
	void Draw()override;
	void CrystalCount(int one, int ten);
	void HitCount(int one, int ten);
	void AttackCount(int one);
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

	std::shared_ptr<KdTexture> m_spHitCntOneTex;
	Matrix					   m_spHitCntOneMat;

	std::shared_ptr<KdTexture> m_spHitCntTenTex;
	Matrix					   m_spHitCntTenMat;

	std::shared_ptr<KdTexture> m_spAttackCntTex;
	Matrix					   m_spAttackCntMat;

	std::string s;

	int m_Ten = 0;
	int m_One = 0;

	int m_CrystalCntOne = 0;
	int m_CrystalCntTen = 0;
	int Crystals = 0;

	int m_HitCntOne = 0;
	int m_HitCntTen = 0;

	int m_AttackCnt = 0;

	int Weight = 1;
	int i = 0;

	bool m_canChange = false;

};
#pragma once
#include"../GameProcess.h"

class AinmationEffect;

class ActionGameProcess :public GameProcess
{
public:

	void Deserialize(const json11::Json& jsonObj);

	void Draw2D();

	void Update() override;

	void Time(int seconds, int minutes);

	void CrystaCount(int one , int ten);
	int GetTen() { return m_CrystalsTenthPlace; }
	int GetOne() { return m_CrystalsOncePlace; }

private:

	std::shared_ptr<KdTexture> m_spScreenRT = nullptr;
	std::shared_ptr<KdTexture> m_spScreenZ = nullptr;

	int m_limit = 0;
	std::shared_ptr<GameObject> m_sphuman;

	std::shared_ptr<KdTexture> m_spCrystalsOncePlace;
	Matrix m_CrystalsOncePlaceMat;

	std::shared_ptr<KdTexture> m_spCrystalsTenthPlace;
	Matrix m_spCrystalsTenthPlaceMat;

	std::string s;

	std::shared_ptr<KdTexture> m_spFrame;
	Matrix m_FrameMat;

	std::shared_ptr<KdTexture> m_spFrame_1;
	Matrix m_FrameMat_1;

	std::shared_ptr<KdTexture> m_spTimeSeconds;
	Matrix m_TimeSecondsMat;

	std::shared_ptr<KdTexture> m_spTimeMinutes;
	Matrix m_TimeMinutesMat;

	std::shared_ptr<KdTexture> m_spSnow;

	static const int SnouNum=50;
	Matrix m_SnowMat[SnouNum];
	Vec3 pos[SnouNum] ;
	Vec3 move;
	float scale = 1;
	int m_TimeMinutes = 1;
	int m_TimeSeconds = 9;

	int time = 0;

	std::shared_ptr<AinmationEffect> falleffect[10] ;
	Matrix falleffectMat[10];
	float   falleffectposY[10];

	int m_CrystalsTenthPlace = 0;
	int m_CrystalsOncePlace = 0;

	bool m_canChange = false;

	int AttackCnt = 0;

	Math::Vector3 copy;
};
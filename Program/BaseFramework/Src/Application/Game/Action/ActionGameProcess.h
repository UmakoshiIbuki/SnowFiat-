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
	int GetOne() { return m_CrystalsOncePlace ; }

private:

	//時間系///////////////////////////////////////////////////////////////

	int m_TimeMinutes = 0;
	int m_TimeSeconds = 0;

	int time = 0;								//制限時間設定用　例：60を入れると制限時間60秒となる

	std::shared_ptr<KdTexture> m_spFrameTex;		//枠用テクスチャ
	std::shared_ptr<KdTexture> m_spMeterTex;		//制限時間表示メーター
	std::shared_ptr<KdTexture> m_spInfinityTex;		//制限時間無しのテクスチャ
	std::shared_ptr<KdTexture> m_spTimeSecondsTex;	//秒数テクスチャ
	std::shared_ptr<KdTexture> m_spTimeMinutesTex;	//秒数テクスチャ

	////////////////////////////////////////////////////////////////////////


	Matrix m_TexMat;	//テクスチャの行列
	Vec3   m_TexPos;	//テクスチャの座標

	std::shared_ptr<KdTexture> m_spMissionTex;		//進め方表示テクスチャ
	std::shared_ptr<KdTexture> m_spExpTex;			//操作説明画面テクスチャ

	float m_rotate;
	float m_angle = 0;
	float scale = 1;


	std::shared_ptr<GameObject> m_sphuman;
	std::shared_ptr<GameObject> m_sptank;

	std::shared_ptr<KdTexture> m_spCrystalsOncePlaceTex;

	std::shared_ptr<KdTexture> m_spCrystalsTenthPlaceTex;

	std::string m_number;

	std::shared_ptr<KdTexture> m_spSnowTex;

	static const int SnowNum=50;
	Matrix m_SnowMat[SnowNum];
	Vec3 pos[SnowNum];
	Vec3 move;
	
	std::shared_ptr<AinmationEffect> fallSnowTex[10] ;
	Matrix  falleffectMat[10];
	float   falleffectposY[10];

	bool m_canChange = false;
	std::shared_ptr<KdTexture> m_spPauseTex;
	bool					   m_GoPause = false;

	Matrix					   m_ReplayMat;
	Vec3					   m_ReplayPos;

	Matrix					   m_GoTitleMat;
	Vec3					   m_GoTitlePos;

	Matrix					   m_PauseMat;

	Vec3                       MousePos;




	Math::Vector3 copy;

	///////////////////////////////////////////////////////////////////////////////////////
	int  m_CrystalsTenthPlace = 0;
	int  m_CrystalsOncePlace = 0;
	int  AttackCnt = 0;
	//////////////////////////////////////////////////////////////////////////////////////
};
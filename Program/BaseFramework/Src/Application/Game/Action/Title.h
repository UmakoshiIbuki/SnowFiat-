﻿#pragma once
#include"../GameObject.h"

class CharacterUpdate;
class AinmationEffect;

class Title :public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;

	void Update()override;

	void Draw2D()override;
	void Draw()override;

	virtual void ImguiUpdate()override;

	bool							 m_isTitleScene = true;

private:

	int frame=0;

	std::shared_ptr<KdTexture> m_spTitleTex;
	Matrix					   m_TitleMat;

	std::shared_ptr<KdTexture> m_spSelectTex;
	Matrix					   m_SelectMat;
	Vec3					   m_SelectPos;
	float					   m_scale = 0.7f;
	Matrix m_mStart;
	Vec3 m_vGoal = {};

	std::shared_ptr<KdTexture> m_spSelect001Tex;
	Matrix					   m_Select001Mat;
	Vec3					   m_Select001Pos;
	float					   m_scale001 = 0.7f;
	Matrix m_mStart001;
	Vec3 m_vGoal001 = {};

	std::shared_ptr<KdTexture> m_spSelect002Tex;
	Matrix					   m_Select002Mat;
	Vec3					   m_Select002Pos;
	float					   m_scale002 = 0.7f;

	bool					   m_canChange = false;
	float                      m_black = 0.0f;
	bool                       flg=false;
	Vec3                       MousePos;
	Matrix m_mStart002;
	Vec3 m_vGoal002 = {};

	int						   flgs = 1;
	std::shared_ptr<KdTexture> m_spStageWindowTex;
	Matrix					   m_StageWindowMat;

	std::shared_ptr<KdTexture> m_spArrowTex;
	Matrix					   m_ArrowMat;
	Vec3					   m_ArrowPos;
	float					   m_Arrowscale = 0.7f;
	float                      m_ScrollSpeed = 0.00f;
	bool                       m_CanScroll=false;

	std::shared_ptr<KdTexture> m_spArrow001Tex;
	Matrix					   m_Arrow001Mat;
	Vec3					   m_Arrow001Pos;
	float					   m_Arrow001scale = 0.7f;
	float                      m_ScrollSpeed001 = 0.01f;
	bool                       m_CanScroll001 = false;

	std::shared_ptr<KdTexture> m_spClickToStartTex;
	Matrix					   m_ClickToStartMat;
	float                      m_CposY=0;
	float                      m_CspeedY;

	int						   m_stageclearcnt=0;

	std::string				   m_stagename;

	std::shared_ptr<CharacterUpdate> m_spPoworUp;
	Matrix m_CamMat;
	std::string string = "Data/StageMap/StageMap.gltf";

	std::shared_ptr<AinmationEffect> fallSnowTex[100];
	Matrix  falleffectMat[100];
	float   falleffectposY;
	Vec3   m_FalleffectPos[100];
};
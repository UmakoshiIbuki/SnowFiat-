#include "Result.h"
#include"../Scene.h"
#include"../Bridge.h"

void Result::Deserialize(const json11::Json& jsonObj)
{
	m_spResultTex = ResFac.GetTexture("Data/Result/Result.png");

	m_CrystalCntOne =  Scene::GetInstance().GetCrystal();

	m_HitCntOne =  Scene::GetInstance().GetHitCntOne();

	m_spCrystalCntOneTex= ResFac.GetTexture("Data/Texture/UITexture/UI_ONE_0.png");

	m_spHitCntOneTex= ResFac.GetTexture("Data/Texture/UITexture/UI_ONE_0.png");

	m_spCrystalCntTex = ResFac.GetTexture("Data/Result/Crystal.png");
	//Crystals =m_HitCntTen * 10+ m_HitCntOne;
	Crystals = 20;
}

void Result::Update()
{

	Weight++;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_canChange)
		{
			Scene::GetInstance().RequestChangeScene("Data/Scene/Title.json");
			m_canChange = false;
		}
	}
	else
	{
		m_canChange = true;
	}

	CrystalCount(m_CrystalCntOne, m_CrystalCntTen);

	HitCount(m_HitCntOne, m_HitCntTen);
}

void Result::Draw()
{
	m_spResultMat.SetTranslation(Vec3(0,0,0));
	SHADER.m_spriteShader.SetMatrix(m_spResultMat);
	SHADER.m_spriteShader.DrawTex(m_spResultTex.get(), 0, 0);

	//m_spCrystalCntOneMat.CreateScalling(0.23f, 0.15f, 1);
	//m_spCrystalCntOneMat.SetTranslation(Vec3(250, 90, 0));
	//SHADER.m_spriteShader.SetMatrix(m_spCrystalCntOneMat);
	//SHADER.m_spriteShader.DrawTex(m_spCrystalCntOneTex.get(), 0, 0);

	//m_spCrystalCntTenMat.CreateScalling(0.23f, 0.15f, 1);
	//m_spCrystalCntTenMat.SetTranslation(Vec3(200, 90, 0));
	//SHADER.m_spriteShader.SetMatrix(m_spCrystalCntTenMat);
	//SHADER.m_spriteShader.DrawTex(m_spCrystalCntTenTex.get(), 0, 0);

	m_spHitCntOneMat.CreateScalling(0.23f, 0.15f, 1);
	m_spHitCntOneMat.SetTranslation(Vec3(250, -120, 0));
	SHADER.m_spriteShader.SetMatrix(m_spHitCntOneMat);
	SHADER.m_spriteShader.DrawTex(m_spHitCntOneTex.get(), 0, 0);

	m_spHitCntTenMat.CreateScalling(0.23f, 0.15f, 1);
	m_spHitCntTenMat.SetTranslation(Vec3(200, -120, 0));
	SHADER.m_spriteShader.SetMatrix(m_spHitCntTenMat);
	SHADER.m_spriteShader.DrawTex(m_spHitCntTenTex.get(), 0, 0);

	if (i < Crystals)
	{
		if (i < 10)
		{
			if (Weight % 5 == 0) { i++; }
			m_CrystalCnt[i].SetTranslation(pos.x + (10 * i), pos.y, 0);
			SHADER.m_spriteShader.SetMatrix(m_CrystalCnt[i]);
			SHADER.m_spriteShader.DrawTex(m_spCrystalCntTex.get(), 0, 0);
		}
		else
		{
			if (Weight % 5 == 0) { i++; }
			m_CrystalCnt[i].SetTranslation(100 + (10 * i), pos.y - 20, 0);
			SHADER.m_spriteShader.SetMatrix(m_CrystalCnt[i]);
			SHADER.m_spriteShader.DrawTex(m_spCrystalCntTex.get(), 0, 0);
		}
	}
	else
	{
		for (UINT i = 0; i < Crystals; i++)
		{
			if (i < 10)
			{
				m_CrystalCnt[i].SetTranslation(pos.x + (10 * i), pos.y, 0);
				SHADER.m_spriteShader.SetMatrix(m_CrystalCnt[i]);
				SHADER.m_spriteShader.DrawTex(m_spCrystalCntTex.get(), 0, 0);
			}
			else
			{
				m_CrystalCnt[i].SetTranslation(100 + (10 * i), pos.y - 20, 0);
				SHADER.m_spriteShader.SetMatrix(m_CrystalCnt[i]);
				SHADER.m_spriteShader.DrawTex(m_spCrystalCntTex.get(), 0, 0);
			}
		}
	}
}

void Result::CrystalCount(int one, int ten)
{
	switch (one)
	{
	case 0:
		s = "0";
		m_spCrystalCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 1:
		s = "1";
		m_spCrystalCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 2:
		s = "2";
		m_spCrystalCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 3:
		s = "3";
		m_spCrystalCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 4:
		s = "4";
		m_spCrystalCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 5:
		s = "5";
		m_spCrystalCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 6:
		s = "6";
		m_spCrystalCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 7:
		s = "7";
		m_spCrystalCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 8:
		s = "8";
		m_spCrystalCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 9:
		s = "9";
		m_spCrystalCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	}

	switch (ten)
	{
	case 0:
		s = "0";
		m_spCrystalCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 1:
		s = "1";
		m_spCrystalCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 2:
		s = "2";
		m_spCrystalCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 3:
		s = "3";
		m_spCrystalCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 4:
		s = "4";
		m_spCrystalCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 5:
		s = "5";
		m_spCrystalCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 6:
		s = "6";
		m_spCrystalCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 7:
		s = "7";
		m_spCrystalCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 8:
		s = "8";
		m_spCrystalCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 9:
		s = "9";
		m_spCrystalCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	}
}



void Result::HitCount(int one, int ten)
{
	switch (one)
	{
	case 0:
		s = "0";
		m_spHitCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 1:
		s = "1";
		m_spHitCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 2:
		s = "2";
		m_spHitCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 3:
		s = "3";
		m_spHitCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 4:
		s = "4";
		m_spHitCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 5:
		s = "5";
		m_spHitCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 6:
		s = "6";
		m_spHitCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 7:
		s = "7";
		m_spHitCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 8:
		s = "8";
		m_spHitCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 9:
		s = "9";
		m_spHitCntOneTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	}

	switch (ten)
	{
	case 0:
		s = "0";
		m_spHitCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 1:
		s = "1";
		m_spHitCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 2:
		s = "2";
		m_spHitCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 3:
		s = "3";
		m_spHitCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 4:
		s = "4";
		m_spHitCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 5:
		s = "5";
		m_spHitCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 6:
		s = "6";
		m_spHitCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 7:
		s = "7";
		m_spHitCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 8:
		s = "8";
		m_spHitCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 9:
		s = "9";
		m_spHitCntTenTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	}
}

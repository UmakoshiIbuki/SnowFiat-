#include "ActionGameProcess.h"
#include"../Scene.h"
#include"../AnimationEffect.h"
#include"Human.h"
#include"Tank.h"

void ActionGameProcess::Deserialize(const json11::Json& jsonObj)
{
	m_spScreenRT = std::make_shared<KdTexture>();
	m_spScreenRT->CreateRenderTarget(280, 120);

	m_spScreenZ = std::make_shared<KdTexture>();
	m_spScreenZ->CreateDepthStencil(280, 120);

	m_spFrame = ResFac.GetTexture("Data/Texture/UITexture/UI_FRAME.png");
	m_spFrame_1 = ResFac.GetTexture("Data/Texture/UITexture/UI_FRAME_1.png");

	m_sphuman = Scene::GetInstance().FindObjectWithName("PlayerHuman");
	m_sptank = Scene::GetInstance().FindObjectWithName("Tank");

	//プレイ時間
	if (jsonObj["Time"].is_null() == false)
	{
		time = jsonObj["Time"].int_value();
	}

	//プレイ時間
	const std::vector<json11::Json>& TimeNumber = jsonObj["TimeShow"].array_items();
	if (TimeNumber.size() == 3)
	{
		//m_infinite= (int)TimeNumber[0].number_value();       0…制限時間あり   １…制限時間なし
		m_TimeMinutes = (int)TimeNumber[1].number_value();
		m_TimeSeconds = (int)TimeNumber[2].number_value();
	}

	//雪を集める時のエフェクト
	for (UINT i = 0; i < SnouNum; i++)
	{
		m_SnowMat[i].SetTranslation(RND * 1280 - 640, RND * 720 - 360, 1);
		pos[i] = m_SnowMat[i].GetTranslation();
	}

	//雪の降るエフェクト
	for (UINT i = 0; i < 10; i++)
	{
		falleffect[i] = std::make_shared< AinmationEffect>();
		falleffectMat[i].SetTranslation(RND * 30 - 15, RND * 12, RND * 30 - 15);

		falleffectposY[i] = falleffectMat[i].GetTranslation().y;
	
		falleffect[i]->SetAnimationInfo(ResFac.GetTexture("Data/White.png"), 0.4f, 1, 1, 0, 0, 0);
		falleffect[i]->SetMatrix(falleffectMat[i]);
		Scene::GetInstance().AddObject(falleffect[i]);
	}
	m_spSnow = ResFac.GetTexture("Data/White.png");

	KD_AUDIO.Play("Data/Audio/BGM/loop100315.wav", true);
	
	time = time * 60;
}

void ActionGameProcess::Draw2D()
{

	//ゲーム中のクリスタル獲得UI(1の位と10の位）
	m_CrystalsOncePlaceMat.CreateScalling(0.25f, 0.25f, 1);
	m_CrystalsOncePlaceMat.SetTranslation(560, 300, 0);
	SHADER.m_spriteShader.SetMatrix(m_CrystalsOncePlaceMat);
	SHADER.m_spriteShader.DrawTex(m_spCrystalsOncePlace.get(), 0, 0);

	m_spCrystalsTenthPlaceMat.CreateScalling(0.25f, 0.25f, 1);
	m_spCrystalsTenthPlaceMat.SetTranslation(500, 300, 0);
	SHADER.m_spriteShader.SetMatrix(m_spCrystalsTenthPlaceMat);
	SHADER.m_spriteShader.DrawTex(m_spCrystalsTenthPlace.get(), 0, 0);


	//雪を集める時のエフェクト
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		for (UINT i = 0; i < SnouNum; i++)
		{
			move = m_SnowMat[i].GetTranslation();
			move.y *= 0.97;
			move.x *= 0.97;
			scale -= 0.002f;
			m_SnowMat[i].CreateScalling(scale, scale, 1.0f);
			m_SnowMat[i].SetTranslation(move.x, move.y, 1);
			SHADER.m_spriteShader.SetMatrix(m_SnowMat[i]);
			SHADER.m_spriteShader.DrawTex(m_spSnow.get(), 0, 0);
			if (abs(m_SnowMat[i].GetTranslation().y) < 50 && abs(m_SnowMat[i].GetTranslation().x) < 50)
			{
				m_SnowMat[i].SetTranslation(RND * 1280 - 640, RND * 720 - 360, 1);
				scale = 1;
			}
		}
		static const Math::Vector3 a = { 1.0,3.0,5.0 };
		SHADER.m_cb8_Light.Work().DL_Color = a;
	}
	else
	{
		//時間経過で暗くなる
		if (SHADER.m_cb8_Light.Work().DL_Color.x > 1)
		{
			static const Math::Vector3 a = { 0.00,0.00,0.00 };
			SHADER.m_cb8_Light.Work().DL_Color -= a;
			copy = SHADER.m_cb8_Light.Work().DL_Color;
		}
		SHADER.m_cb8_Light.Work().DL_Color = copy;
	}

	//時間表示UI(メーター)
	m_FrameMat_1.RotateZ(-0.15f * KdToRadians);
	m_FrameMat_1.SetTranslation(30, 360, 0);
	SHADER.m_spriteShader.SetMatrix(m_FrameMat_1);
	SHADER.m_spriteShader.DrawTex(m_spFrame_1.get(), 0, 0);

	m_FrameMat.CreateScalling(0.57f, 0.37f, 1);
	m_FrameMat.SetTranslation(30, 310, 0);
	SHADER.m_spriteShader.SetMatrix(m_FrameMat);
	SHADER.m_spriteShader.DrawTex(m_spFrame.get(), 0, 0);

	//if (m_infinite == 0)
	{
		//時間表示UI(1の位と10の位)
		m_TimeSecondsMat.CreateScalling(0.15f, 0.15f, 1);
		m_TimeSecondsMat.SetTranslation(50, 300, 0);
		SHADER.m_spriteShader.SetMatrix(m_TimeSecondsMat);
		SHADER.m_spriteShader.DrawTex(m_spTimeSeconds.get(), 0, 0);

		m_TimeMinutesMat.CreateScalling(0.15f, 0.15f, 1);
		m_TimeMinutesMat.SetTranslation(10, 300, 0);
		SHADER.m_spriteShader.SetMatrix(m_TimeMinutesMat);
		SHADER.m_spriteShader.DrawTex(m_spTimeMinutes.get(), 0, 0);
	}
	//else
	{
		//制限時間無しのUI表示
	}
}

void ActionGameProcess::Update()
{

	if (GetAsyncKeyState('Y'))
	{
		Scene::GetInstance().RequestChangeScene("Data/Scene/Result.json");
	}

	if (GetAsyncKeyState('U'))
	{
		Scene::GetInstance().RequestChangeScene("Data/Scene/Title.json");
	}

	//if (m_infinite == 0)    m_infinite == 0の時は制限時間
	{
		m_limit++;
		if (m_limit >= time)
		{
			Scene::GetInstance().RequestChangeScene("Data/Scene/Result.json");
		}
	}
	//else　　m_infinite == １の時は一定以上クリスタルを指定場所に溜めたら終わり
	{
		//一定以上クリスタルを指定場所に溜めたら終わり
	}

	std::shared_ptr<Human> human = std::dynamic_pointer_cast<Human>(m_sphuman);
	std::shared_ptr<Tank> tank = std::dynamic_pointer_cast<Tank>(m_sptank);

	if (human)
	{
		m_CrystalsOncePlace = human->GetCrystal();
		Scene::GetInstance().SetHitCnt(human->GetHitCntOne(), human->GetHitCntTen());
		AttackCnt = Scene::GetInstance().GetAttackCnt();
	}


	for (UINT i = 0; i < 10; i++)
	{
		falleffectposY[i] -= 0.02f;

		falleffectMat[i].SetTranslation(falleffectMat[i].GetTranslation().x, falleffectposY[i], falleffectMat[i].GetTranslation().z);
		falleffect[i]->SetMatrix(falleffectMat[i]);
		if (falleffectMat[i].GetTranslation().y < 0)
		{
			falleffectposY[i] = 12.0f;
		}
	}

	CrystaCount(m_CrystalsOncePlace, m_CrystalsTenthPlace);

	if (m_limit % 60 == 0) { m_TimeSeconds--; }
	if (m_TimeSeconds < 0)
	{
		m_TimeSeconds = 9;
		m_TimeMinutes--;
	}
	Time(m_TimeMinutes, m_TimeSeconds);
}

void ActionGameProcess::CrystaCount(int one, int ten)
{
	switch (one)
	{
	case 0:
		s = "0";
		m_spCrystalsOncePlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 1:
		s = "1";
		m_spCrystalsOncePlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 2:
		s = "2";
		m_spCrystalsOncePlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 3:
		s = "3";
		m_spCrystalsOncePlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 4:
		s = "4";
		m_spCrystalsOncePlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 5:
		s = "5";
		m_spCrystalsOncePlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 6:
		s = "6";
		m_spCrystalsOncePlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 7:
		s = "7";
		m_spCrystalsOncePlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 8:
		s = "8";
		m_spCrystalsOncePlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 9:
		s = "9";
		m_spCrystalsOncePlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	}

	switch (ten)
	{
	case 0:
		s = "0";
		m_spCrystalsTenthPlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 1:
		s = "1";
		m_spCrystalsTenthPlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 2:
		s = "2";
		m_spCrystalsTenthPlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 3:
		s = "3";
		m_spCrystalsTenthPlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 4:
		s = "4";
		m_spCrystalsTenthPlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 5:
		s = "5";
		m_spCrystalsTenthPlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 6:
		s = "6";
		m_spCrystalsTenthPlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 7:
		s = "7";
		m_spCrystalsTenthPlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 8:
		s = "8";
		m_spCrystalsTenthPlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 9:
		s = "9";
		m_spCrystalsTenthPlace = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	}
}

void ActionGameProcess::Time(int minutes, int seconds)
{
	switch (seconds)
	{
	case 0:
		s = "0";
		m_spTimeSeconds = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 1:
		s = "1";
		m_spTimeSeconds = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 2:
		s = "2";
		m_spTimeSeconds = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 3:
		s = "3";
		m_spTimeSeconds = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 4:
		s = "4";
		m_spTimeSeconds = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 5:
		s = "5";
		m_spTimeSeconds = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 6:
		s = "6";
		m_spTimeSeconds = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 7:
		s = "7";
		m_spTimeSeconds = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 8:
		s = "8";
		m_spTimeSeconds = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 9:
		s = "9";
		m_spTimeSeconds = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	}

	switch (minutes)
	{
	case 0:
		s = "0";
		m_spTimeMinutes = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 1:
		s = "1";
		m_spTimeMinutes = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 2:
		s = "2";
		m_spTimeMinutes = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 3:
		s = "3";
		m_spTimeMinutes = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 4:
		s = "4";
		m_spTimeMinutes = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 5:
		s = "5";
		m_spTimeMinutes = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 6:
		s = "6";
		m_spTimeMinutes = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 7:
		s = "7";
		m_spTimeMinutes = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 8:
		s = "8";
		m_spTimeMinutes = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	case 9:
		s = "9";
		m_spTimeMinutes = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + s + ".png");
		break;
	}
}

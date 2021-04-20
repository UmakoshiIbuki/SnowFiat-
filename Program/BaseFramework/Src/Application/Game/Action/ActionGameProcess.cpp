#include "ActionGameProcess.h"
#include"../Scene.h"
#include"../AnimationEffect.h"
#include"Human.h"
#include"Tank.h"
#include"../../main.h"
#include"../../Component//InputComponent.h"



void ActionGameProcess::Deserialize(const json11::Json& jsonObj)
{

	m_spFrameTex = ResFac.GetTexture("Data/Texture/UITexture/UI_FRAME.png");
	m_spMeterTex = ResFac.GetTexture("Data/Texture/UITexture/UI_FRAME_1.png");
	m_spInfinityTex = ResFac.GetTexture("Data/Texture/UITexture/UI_Infinite.png");
	m_spMissionTex = ResFac.GetTexture("Data/Texture/UITexture/UI_Mission001.png");
	m_spExpTex = ResFac.GetTexture("Data/Texture/UITexture/UI_Exp.png");

	m_sphuman = Scene::GetInstance().FindObjectWithName("PlayerHuman");
	m_sptank = Scene::GetInstance().FindObjectWithName("Tank");

	m_GoTitlePos.x = 2;
	m_GoTitlePos.y = -45;

	m_ReplayPos.x = 0;
	m_ReplayPos.y = 50;

	//プレイ時間
	if (jsonObj["Time"].is_null() == false)
	{
		time = jsonObj["Time"].int_value();			//0以外…制限時間あり   0…制限時間なし
	}

	if (jsonObj["Name"].is_null() == false)
	{
		m_name = jsonObj["Name"].string_value();
	}

	//プレイ時間
	const std::vector<json11::Json>& TimeNumber = jsonObj["TimeShow"].array_items();
	if (TimeNumber.size() == 3)
	{
		m_TimeMinutes = (int)TimeNumber[1].number_value();
		m_TimeSeconds = (int)TimeNumber[2].number_value();
	}

	//雪を集める時のエフェクト
	for (UINT i = 0; i < SnowNum; i++)
	{
		m_SnowMat[i].SetTranslation(RND * 1280 - 640, RND * 720 - 360, 1);
		pos[i] = m_SnowMat[i].GetTranslation();
	}

	//雪の降るエフェクト
	for (UINT i = 0; i < 10; i++)
	{
		fallSnowTex[i] = std::make_shared< AinmationEffect>();
		falleffectMat[i].SetTranslation(RND * 30 - 15, RND * 12, RND * 30 - 15);

		falleffectposY[i] = falleffectMat[i].GetTranslation().y;
	
		fallSnowTex[i]->SetAnimationInfo(ResFac.GetTexture("Data/Texture/White.png"), 0.4f, 1, 1, 0, 0, 0);
		fallSnowTex[i]->SetMatrix(falleffectMat[i]);
		Scene::GetInstance().AddObject(fallSnowTex[i]);
	}
	m_spSnowTex = ResFac.GetTexture("Data/Texture/White.png");

	KD_AUDIO.Play("Data/Audio/BGM/loop100315.wav", true);

	time = time * 60;

	m_angle = 360/(float)time/2;
}

void ActionGameProcess::Draw2D()
{

	//ゲーム中のクリスタル獲得UI(1の位と10の位）
	m_TexMat.CreateScalling(0.25f, 0.25f, 1);
	m_TexMat.SetTranslation(560, 300, 0);
	SHADER.m_spriteShader.SetMatrix(m_TexMat);
	SHADER.m_spriteShader.DrawTex(m_spCrystalsOncePlaceTex.get(), 0, 0);

	m_TexMat.CreateScalling(0.25f, 0.25f, 1);
	m_TexMat.SetTranslation(500, 300, 0);
	SHADER.m_spriteShader.SetMatrix(m_TexMat);
	SHADER.m_spriteShader.DrawTex(m_spCrystalsTenthPlaceTex.get(), 0, 0);

	//チャージ中のエフェクト
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		for (UINT i = 0; i < SnowNum; i++)
		{
			move = m_SnowMat[i].GetTranslation();
			move.y *= 0.97;
			move.x *= 0.97;
			scale -= 0.002f;
			m_SnowMat[i].CreateScalling(scale, scale, 1.0f);
			m_SnowMat[i].SetTranslation(move.x, move.y, 1);
			SHADER.m_spriteShader.SetMatrix(m_SnowMat[i]);
			SHADER.m_spriteShader.DrawTex(m_spSnowTex.get(), 0, 0);
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
	m_TexMat.CreateRotationZ(-m_rotate * KdToRadians);
	m_rotate += m_angle;
	m_TexMat.SetTranslation(30, 360, 0);
	SHADER.m_spriteShader.SetMatrix(m_TexMat);
	SHADER.m_spriteShader.DrawTex(m_spMeterTex.get(), 0, 0);

	m_TexMat.CreateScalling(0.57f, 0.37f, 1);
	m_TexMat.SetTranslation(30, 310, 0);
	SHADER.m_spriteShader.SetMatrix(m_TexMat);
	SHADER.m_spriteShader.DrawTex(m_spFrameTex.get(), 0, 0);

	if (!(time == 0))
	{
		//時間表示UI(1の位と10の位)
		m_TexMat.CreateScalling(0.15f, 0.15f, 1);
		m_TexMat.SetTranslation(50, 300, 0);
		SHADER.m_spriteShader.SetMatrix(m_TexMat);
		SHADER.m_spriteShader.DrawTex(m_spTimeSecondsTex.get(), 0, 0);

		m_TexMat.CreateScalling(0.15f, 0.15f, 1);
		m_TexMat.SetTranslation(10, 300, 0);
		SHADER.m_spriteShader.SetMatrix(m_TexMat);
		SHADER.m_spriteShader.DrawTex(m_spTimeMinutesTex.get(), 0, 0);
	}
	else
	{
		m_TexMat.CreateScalling(0.25f, 0.15f, 1);
		m_TexMat.SetTranslation(25, 300, 0);
		SHADER.m_spriteShader.SetMatrix(m_TexMat);
		SHADER.m_spriteShader.DrawTex(m_spInfinityTex.get(), 0, 0);
	}

	m_TexMat.CreateScalling(0.5, 0.5, 1);
	m_TexMat.SetTranslation(-300, 330, 0);
	SHADER.m_spriteShader.SetMatrix(m_TexMat);
	SHADER.m_spriteShader.DrawTex(m_spMissionTex.get(), 0, 0);

	m_TexMat.CreateScalling(0.5, 0.5, 1);
	m_TexMat.SetTranslation(m_TexPos);
	SHADER.m_spriteShader.SetMatrix(m_TexMat);
	SHADER.m_spriteShader.DrawTex(m_spExpTex.get(), 0, 0);

	if (m_GoPause)
	{
		m_spPauseTex = ResFac.GetTexture("Data/Texture/Stop.png");
		m_PauseMat.SetTranslation(0, 150, 0);
		SHADER.m_spriteShader.SetMatrix(m_PauseMat);
		SHADER.m_spriteShader.DrawTex(m_spPauseTex.get(), 0, 0);

		m_spPauseTex = ResFac.GetTexture("Data/Texture/GoTitle.png");
		m_PauseMat.SetTranslation(m_GoTitlePos);
		SHADER.m_spriteShader.SetMatrix(m_PauseMat);
		SHADER.m_spriteShader.DrawTex(m_spPauseTex.get(), 0, 0);

		m_spPauseTex = ResFac.GetTexture("Data/Texture/Replay.png");
		SHADER.m_spriteShader.SetMatrix(m_ReplayMat);
		SHADER.m_spriteShader.DrawTex(m_spPauseTex.get(), 0, -0);
	}
}

void ActionGameProcess::Update()
{
	//改善//////////////////////////////////////////////////////////////////
	POINT nowMousePos;
	HWND hwnd;
	hwnd = APP.m_window.GetWndHandle();

	GetCursorPos(&nowMousePos);				//マウス現在位置の取得
	ScreenToClient(hwnd, &nowMousePos);		//マウスをクライアント座標に

	nowMousePos.x -= 640;
	nowMousePos.y -= 360;

	MousePos.x = nowMousePos.x;
	MousePos.y = nowMousePos.y;
	//改善//////////////////////////////////////////////////////////////////

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		m_GoPause = true;
		m_sphuman->m_base = true;
		ShowCursor(true);
		m_ReplayMat.SetTranslation(m_ReplayPos);
		m_GoTitleMat.SetTranslation(m_GoTitlePos);
	}

	if (m_GoPause)
	{
		if (Collision2D(m_ReplayPos, MousePos, 300, 50))
		{
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				m_sphuman->m_base = false;
				m_GoPause = false;
				ShowCursor(false);
			}
		}

		if (Collision2D(m_GoTitlePos, MousePos, 300, 50))
		{
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				m_sphuman->m_base = false;

				Scene::GetInstance().RequestChangeScene("Data/Scene/Title.json");
			}
		}
	}

	if (GetAsyncKeyState('Y'))
	{
		KD_AUDIO.StopBGM();
		ShowCursor(true);
		Scene::GetInstance().RequestChangeScene("Data/Scene/Result.json");
	}

	if (GetAsyncKeyState('U'))
	{
		Scene::GetInstance().RequestChangeScene("Data/Scene/Title.json");
	}

	if (GetAsyncKeyState('Q'))
	{
		m_TexPos.SetPos(0, 0, 0);
		m_spExpTex = ResFac.GetTexture("Data/Texture/Expall.png");
	}
	else
	{
		m_TexPos.SetPos(600, -300, 0);
		m_spExpTex = ResFac.GetTexture("Data/Texture/UITexture/UI_Exp.png");
	}

	if (!(time == 0))
	{
		time--;
		if (time <= 0)
		{
			ShowCursor(true);
			Scene::GetInstance().RequestChangeScene("Data/Scene/Result.json");
		}
	}
	else if(Scene::GetInstance().GetHitCnt()>=3)
	{
		ShowCursor(true);
		Scene::GetInstance().SetHitCnt(-3);
		Scene::GetInstance().RequestChangeScene("Data/Scene/Result.json");
	}

	std::shared_ptr<Human> human = std::dynamic_pointer_cast<Human>(m_sphuman);
	std::shared_ptr<Tank> tank = std::dynamic_pointer_cast<Tank>(m_sptank);

	if (human)
	{
		m_CrystalsOncePlace = human->GetCrystal();
		Scene::GetInstance().SetHitCnt(human->GetHitCntOne(), human->GetHitCntTen());
		AttackCnt = Scene::GetInstance().GetHitCnt();
	}


	for (UINT i = 0; i < 10; i++)
	{
		falleffectposY[i] -= 0.02f;

		falleffectMat[i].SetTranslation(falleffectMat[i].GetTranslation().x, falleffectposY[i], falleffectMat[i].GetTranslation().z);
		fallSnowTex[i]->SetMatrix(falleffectMat[i]);
		if (falleffectMat[i].GetTranslation().y < 0)
		{
			falleffectposY[i] = 12.0f;
		}
	}

	if (time % 60 == 0)
	{
		m_TimeSeconds--; 
	}

	if (m_TimeSeconds < 0)
	{
		m_TimeSeconds = 9;
		m_TimeMinutes--;
	}
	Time(m_TimeMinutes, m_TimeSeconds);

	CrystaCount(m_CrystalsOncePlace, m_CrystalsTenthPlace);

}

void ActionGameProcess::CrystaCount(int one, int ten)
{
	switch (one)
	{
	case 0:
		m_number = "0";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 1:
		m_number = "1";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 2:
		m_number = "2";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 3:
		m_number = "3";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 4:
		m_number = "4";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 5:
		m_number = "5";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 6:
		m_number = "6";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 7:
		m_number = "7";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 8:
		m_number = "8";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 9:
		m_number = "9";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	}

	switch (ten)
	{
	case 0:
		m_number = "0";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 1:
		m_number = "1";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 2:
		m_number = "2";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 3:
		m_number = "3";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 4:
		m_number = "4";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 5:
		m_number = "5";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 6:
		m_number = "6";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 7:
		m_number = "7";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 8:
		m_number = "8";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 9:
		m_number = "9";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	}
}

void ActionGameProcess::Time(int minutes, int seconds)
{
	switch (seconds)
	{
	case 0:
		m_number = "0";
		m_spTimeSecondsTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 1:
		m_number = "1";
		m_spTimeSecondsTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 2:
		m_number = "2";
		m_spTimeSecondsTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 3:
		m_number = "3";
		m_spTimeSecondsTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 4:
		m_number = "4";
		m_spTimeSecondsTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 5:
		m_number = "5";
		m_spTimeSecondsTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 6:
		m_number = "6";
		m_spTimeSecondsTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 7:
		m_number = "7";
		m_spTimeSecondsTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 8:
		m_number = "8";
		m_spTimeSecondsTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 9:
		m_number = "9";
		m_spTimeSecondsTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	}

	switch (minutes)
	{
	case 0:
		m_number = "0";
		m_spTimeMinutesTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 1:
		m_number = "1";
		m_spTimeMinutesTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 2:
		m_number = "2";
		m_spTimeMinutesTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 3:
		m_number = "3";
		m_spTimeMinutesTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 4:
		m_number = "4";
		m_spTimeMinutesTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 5:
		m_number = "5";
		m_spTimeMinutesTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 6:
		m_number = "6";
		m_spTimeMinutesTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 7:
		m_number = "7";
		m_spTimeMinutesTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 8:
		m_number = "8";
		m_spTimeMinutesTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 9:
		m_number = "9";
		m_spTimeMinutesTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	}
}

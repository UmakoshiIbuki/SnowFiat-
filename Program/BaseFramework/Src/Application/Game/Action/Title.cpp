#include "Title.h"
#include"../Scene.h"
#include"../../main.h"

void Title::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);

	m_spTitleTex = ResFac.GetTexture("Data/Title/Title.png");

	m_SelectPos.x = 0;
	m_SelectPos.y = -270;

	m_Select001Pos.x =  400;
	m_Select001Pos.y = -270;

	m_Select002Pos.x = 800;
	m_Select002Pos.y = -270;

	m_ArrowPos.x = -500;
	m_ArrowPos.y = -270;

	m_Arrow001Pos.x = 500;
	m_Arrow001Pos.y = -270;

	KD_AUDIO.Play("Data/Audio/BGM/loop100302.wav", false);
}

void Title::Update()
{
	POINT nowMousePos;
	HWND hwnd;
	hwnd=APP.m_window.GetWndHandle();

	GetCursorPos(&nowMousePos);				//マウス現在位置の取得
	ScreenToClient(hwnd, &nowMousePos);		//マウスをクライアント座標に

	nowMousePos.x -= 640;
	nowMousePos.y -= 360;

	MousePos.x = nowMousePos.x;
	MousePos.y = nowMousePos.y;

	m_CposY =-80+10* sin(m_CspeedY);

	m_CspeedY += 0.1f;
	if (nowMousePos.x< m_Arrow001Pos.x + 25 && nowMousePos.x>m_Arrow001Pos.x - 25 && nowMousePos.y > (m_Arrow001Pos.y + 25) * -1 && nowMousePos.y < (m_Arrow001Pos.y - 25) * -1)
	{
		if (m_Arrow001scale < 0.85f) { m_Arrow001scale += 0.01f; }
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			m_ScrollSpeed = 0.0f;
			if (m_canChange)
			{
				m_mStart.SetTranslation(m_SelectPos);
				m_mStart001.SetTranslation(m_Select001Pos);
				m_mStart002.SetTranslation(m_Select002Pos);

				m_vGoal = Vec3(m_SelectPos.x+400, -270, 0);
				m_vGoal001 = Vec3(m_Select001Pos.x + 400, -270, 0);
				m_vGoal002 = Vec3(m_Select002Pos.x + 400, -270, 0);

				flgs--;

				m_canChange = false;
				m_CanScroll = true;
			}
		}
		else { m_canChange = true; }
	}
	else
	{
		if (m_Arrow001scale > 0.7) { m_Arrow001scale -= 0.01f; }
	}

	if (nowMousePos.x< m_ArrowPos.x + 25 && nowMousePos.x>m_ArrowPos.x - 25 && nowMousePos.y > (m_ArrowPos.y + 25) * -1 && nowMousePos.y < (m_ArrowPos.y - 25) * -1)
	{
		if (m_Arrowscale < 0.85f) { m_Arrowscale += 0.01f; }
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_canChange)
			{
				m_ScrollSpeed = 0.0f;

				m_mStart.SetTranslation(m_SelectPos);
				m_mStart001.SetTranslation(m_Select001Pos);
				m_mStart002.SetTranslation(m_Select002Pos);

				m_vGoal = Vec3(m_SelectPos.x-400, -270, 0);
				m_vGoal001 = Vec3(m_Select001Pos.x-400, -270, 0);
				m_vGoal002 = Vec3(m_Select002Pos.x-400, -270, 0);
				
				flgs++;

				m_canChange = false;
				m_CanScroll = true;
			}
		}
		else { m_canChange = true; }
	}
	else
	{
		if (m_Arrowscale > 0.7) { m_Arrowscale -= 0.01f; }
	}

	auto& vStart = m_mStart.GetTranslation();
	auto& vGoal = m_vGoal;
	Vec3 vTo = vGoal - vStart;
	Vec3 vNow = vStart + vTo * easeOutQuart(m_ScrollSpeed);

	auto& vStart001 = m_mStart001.GetTranslation();
	auto& vGoal001 = m_vGoal001;
	Vec3 vTo001 = vGoal001 - vStart001;
	Vec3 vNow001 = vStart001 + vTo001 * easeOutQuart(m_ScrollSpeed);

	auto& vStart002 = m_mStart002.GetTranslation();
	auto& vGoal002 = m_vGoal002;
	Vec3 vTo002 = vGoal002 - vStart002;
	Vec3 vNow002 = vStart002 + vTo002 * easeOutQuart(m_ScrollSpeed);


	if (m_CanScroll)
	{
		if (m_ScrollSpeed < 0.9f)
		{
			m_SelectPos = vNow;
			m_Select001Pos = vNow001;
			m_Select002Pos = vNow002;

			m_ScrollSpeed += 0.01f;
		}
	}

	if (m_ScrollSpeed > 1) { m_ScrollSpeed = 0; }

	if (flgs == 1) {
		if (nowMousePos.x< m_SelectPos.x + 100 && nowMousePos.x>m_SelectPos.x - 100 && nowMousePos.y > (m_SelectPos.y + 100) * -1 && nowMousePos.y < (m_SelectPos.y - 100) * -1)
		{
			if (m_scale < 0.85f) { m_scale += 0.01f; }
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				if (m_canChange)
				{
					flg = true;
					m_canChange = false;
				}
			}
			else { m_canChange = true; }
		}
		else
		{
			if (m_scale > 0.7) { m_scale -= 0.01f; }
		}
	}

	if (flgs == 2) {

		if (nowMousePos.x< m_Select001Pos.x + 100 && nowMousePos.x>m_Select001Pos.x - 100 && nowMousePos.y > (m_Select001Pos.y + 100) * -1 && nowMousePos.y < (m_Select001Pos.y - 100) * -1)
		{
			if (m_scale001 < 0.85f) { m_scale001 += 0.01f; }
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				if (m_canChange)
				{
					flg = true;
					m_canChange = false;
					Scene::GetInstance().RequestChangeScene("Data/Scene/CarPark.json");
				}
			}
			else { m_canChange = true; }
		}
		else
		{
			if (m_scale001 > 0.7) { m_scale001 -= 0.01f; }
		}
	}

	if (flgs == 3) {

		if (nowMousePos.x< m_Select002Pos.x + 100 && nowMousePos.x>m_Select002Pos.x - 100 && nowMousePos.y > (m_Select002Pos.y + 100) * -1 && nowMousePos.y < (m_Select002Pos.y - 100) * -1)
		{
			if (m_scale002 < 0.85f) { m_scale002 += 0.01f; }
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				if (m_canChange)
				{
					flg = true;
					m_canChange = false;
					Scene::GetInstance().RequestChangeScene("Data/Scene/Bridge.json");
				}
			}
			else { m_canChange = true; }
		}
		else
		{
			if (m_scale002 > 0.7) { m_scale002 -= 0.01f; }
		}
	}

	if (m_black>0.6)
	{
		Scene::GetInstance().RequestChangeScene("Data/Scene/ActionGame.json");
	}
	//ImguiUpdate();
}

void Title::Draw2D()
{
	m_spTitleTex = ResFac.GetTexture("Data/Title/Title.png");
	
	m_TitleMat.SetTranslation(Vec3(0,0,0));
	SHADER.m_spriteShader.SetMatrix(m_TitleMat);
	SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);

	/*if (flgs==1)
	{
		m_StageWindowMat.SetTranslation(100, 100, 1);
		m_spStageWindowTex =ResFac.GetTexture("Data/Title/Hiroba.png");
		SHADER.m_spriteShader.SetMatrix(m_StageWindowMat);
		SHADER.m_spriteShader.DrawTex(m_spStageWindowTex.get(), 0, 0);
		flgs = 0;
	}

	if (flgs == 2)
	{
		m_StageWindowMat.SetTranslation(200, 100, 1);
		m_spStageWindowTex = ResFac.GetTexture("Data/Title/select_000.png");
		SHADER.m_spriteShader.SetMatrix(m_StageWindowMat);
		SHADER.m_spriteShader.DrawTex(m_spStageWindowTex.get(), 0, 0);
		flgs = 0;
	}

	if (flgs == 3)
	{
		m_StageWindowMat.SetTranslation(300, 100, 1);
		m_spStageWindowTex = ResFac.GetTexture("Data/Title/select_000.png");
		SHADER.m_spriteShader.SetMatrix(m_StageWindowMat);
		SHADER.m_spriteShader.DrawTex(m_spStageWindowTex.get(), 0, 0);
		flgs = 0;
	}*/

	if (flg)
	{
		m_black += 0.01f;
		SHADER.m_postProcessShader.ColorDraw(m_spTitleTex.get(), DirectX::SimpleMath::Vector4(0, 0, 0, m_black));
	}
	else
	{
		//SHADER.m_postProcessShader.ColorDraw(m_spTitleTex.get(), DirectX::SimpleMath::Vector4(0,0,0,0));
	}

	m_spTitleTex = ResFac.GetTexture("Data/Title/Hiroba.png");
	m_SelectMat.CreateScalling(m_scale, m_scale, 0);
	m_SelectMat.SetTranslation(m_SelectPos.x, m_SelectPos.y, m_SelectPos.z);
	SHADER.m_spriteShader.SetMatrix(m_SelectMat);
	SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(),0,0);

	m_spTitleTex = ResFac.GetTexture("Data/Title/select_001.png");
	m_Select001Mat.CreateScalling(m_scale001, m_scale001, 0);
	m_Select001Mat.SetTranslation(m_Select001Pos.x, m_Select001Pos.y, m_Select001Pos.z);
	SHADER.m_spriteShader.SetMatrix(m_Select001Mat);
	SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);

	m_spTitleTex = ResFac.GetTexture("Data/Title/select_002.png");
	m_Select002Mat.CreateScalling(m_scale002, m_scale002, 0);
	m_Select002Mat.SetTranslation(m_Select002Pos.x, m_Select002Pos.y, m_Select002Pos.z);
	SHADER.m_spriteShader.SetMatrix(m_Select002Mat);
	SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);

	m_spTitleTex = ResFac.GetTexture("Data/Title/arrow.png");
	m_ArrowMat.CreateScalling(m_Arrowscale, m_Arrowscale, 0);
	m_ArrowMat.SetTranslation(m_ArrowPos);
	SHADER.m_spriteShader.SetMatrix(m_ArrowMat);
	SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);

	m_spTitleTex = ResFac.GetTexture("Data/Title/arrow.png");
	m_Arrow001Mat.CreateScalling(m_Arrow001scale, m_Arrow001scale, 0);
	m_Arrow001Mat.RotateY(180*KdToRadians);
	m_Arrow001Mat.SetTranslation(m_Arrow001Pos);
	SHADER.m_spriteShader.SetMatrix(m_Arrow001Mat);
	SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);

	m_spTitleTex = ResFac.GetTexture("Data/Title/ClickToStart.png");
	m_ClickToStartMat.SetTranslation(0, m_CposY, 1);
	SHADER.m_spriteShader.SetMatrix(m_ClickToStartMat);
	SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);
}

void Title::ImguiUpdate()
{

	if (ImGui::Begin("MousePos"))
	{
		ImGui::DragFloat2("nowMousePos", &MousePos.x, 0.01f);
	}
	ImGui::End();

	Vec3 start = m_mStart.GetTranslation();
	if (ImGui::Begin("m_vGoal"))
	{
		ImGui::DragFloat("m_vGoal", &m_ScrollSpeed, 0.01f);
	}
	ImGui::End();
}

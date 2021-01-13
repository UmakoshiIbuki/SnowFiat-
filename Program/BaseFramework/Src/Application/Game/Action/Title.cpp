#include "Title.h"
#include"../Scene.h"
#include"../../main.h"
void Title::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);

	m_spTitleTex = ResFac.GetTexture("Data/Title/Title.png");
	m_spSelectTex = ResFac.GetTexture("Data/Title/select_000.png");
	m_spSelect001Tex = ResFac.GetTexture("Data/Title/select_001.png");
	m_spSelect002Tex = ResFac.GetTexture("Data/Title/select_002.png");

	m_SelectPos.x = -200;
	m_SelectPos.y = -270;

	m_Select001Pos.x =  0;
	m_Select001Pos.y = -270;

	m_Select002Pos.x = 200;
	m_Select002Pos.y = -270;

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
	if (nowMousePos.x< m_SelectPos.x+100 && nowMousePos.x>m_SelectPos.x-100&& nowMousePos.y> (m_SelectPos.y+100)*-1 && nowMousePos.y<(m_SelectPos.y-100)*-1)
	{
		if (m_scale < 0.85f)	{m_scale += 0.01f;}
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_canChange)
			{
				flg = true;
				m_canChange = false;
			}
		}
		else{m_canChange = true;}
	}
	else
	{
		if (m_scale > 0.7) {m_scale -= 0.01f; }
	}

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

	if (nowMousePos.x< m_Select002Pos.x + 100 && nowMousePos.x>m_Select002Pos.x - 100 && nowMousePos.y > (m_Select002Pos.y + 100) * -1 && nowMousePos.y < (m_Select002Pos.y - 100) * -1)
	{
		if (m_scale002 < 0.85f) { m_scale002 += 0.01f; }
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
		if (m_scale002 > 0.7) { m_scale002 -= 0.01f; }
	}

	if (m_black<0)
	{
		Scene::GetInstance().RequestChangeScene("Data/Scene/ActionGame.json");
	}
	ImguiUpdate();
}

void Title::Draw2D()
{
	m_TitleMat.SetTranslation(Vec3(0,0,0));
	SHADER.m_spriteShader.SetMatrix(m_TitleMat);
	SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(),0,0);
	if (flg)
	{
		m_black -= 0.01f;
		//SHADER.m_postProcessShader.ColorDraw(m_spTitleTex.get(), DirectX::SimpleMath::Vector4(m_black, m_black, m_black, 1));
	}/*
	else
	{
		SHADER.m_postProcessShader.ColorDraw(m_spTitleTex.get(), DirectX::SimpleMath::Vector4(1, 1, 1, 1));
	}*/
	m_SelectMat.CreateScalling(m_scale, m_scale, 0);
	m_SelectMat.SetTranslation(m_SelectPos.x, m_SelectPos.y, m_SelectPos.z);
	SHADER.m_spriteShader.SetMatrix(m_SelectMat);
	SHADER.m_spriteShader.DrawTex(m_spSelectTex.get(),0,0);

	m_Select001Mat.CreateScalling(m_scale001, m_scale001, 0);
	m_Select001Mat.SetTranslation(m_Select001Pos.x, m_Select001Pos.y, m_Select001Pos.z);
	SHADER.m_spriteShader.SetMatrix(m_Select001Mat);
	SHADER.m_spriteShader.DrawTex(m_spSelect001Tex.get(), 0, 0);

	m_Select002Mat.CreateScalling(m_scale002, m_scale002, 0);
	m_Select002Mat.SetTranslation(m_Select002Pos.x, m_Select002Pos.y, m_Select002Pos.z);
	SHADER.m_spriteShader.SetMatrix(m_Select002Mat);
	SHADER.m_spriteShader.DrawTex(m_spSelect002Tex.get(), 0, 0);
}

void Title::ImguiUpdate()
{

	if (ImGui::Begin("MousePos"))
	{
		ImGui::DragFloat2("nowMousePos", &MousePos.x, 0.01f);
	}
	ImGui::End();
}

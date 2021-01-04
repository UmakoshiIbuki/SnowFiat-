#include "Title.h"
#include"../Scene.h"

void Title::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);

	m_spTitleTex = ResFac.GetTexture("Data/Title/Title.png");
	m_spSelectTex = ResFac.GetTexture("Data/Title/select.png");
}

void Title::Update()
{
	POINT nowMousePos;

	GetCursorPos(&nowMousePos);//マウス現在位置の取得
	//ScreenToClient(hwnd, &nowMousePos)
	MousePos.x = nowMousePos.x;
	MousePos.y = nowMousePos.y;
	if (nowMousePos.x<750 && nowMousePos.x>550 && nowMousePos.y<500 && nowMousePos.y>300)
	{
		if (m_scale < 1.5f)	{m_scale += 0.1f;}
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
		if (m_scale > 1) {m_scale -= 0.1f; }
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
	m_SelectMat.SetTranslation(Vec3(0, 0, 0));
	SHADER.m_spriteShader.SetMatrix(m_SelectMat);
	SHADER.m_spriteShader.DrawTex(m_spSelectTex.get(),0,0);

}

void Title::ImguiUpdate()
{

	if (ImGui::Begin("MousePos"))
	{
		ImGui::DragFloat2("nowMousePos", &MousePos.x, 0.01f);
	}
	ImGui::End();
}

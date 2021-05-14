#include "Title.h"
#include "CharacterUpdate.h"
#include"../AnimationEffect.h"
#include"../Scene.h"
#include"../../main.h"
#include"../../Component//CameraComponent.h"
#include"../../Component//ModelComponent.h"

void Title::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);

	if (m_spCameraComponent)
	{
		m_spCameraComponent->OffsetMatrix().CreateTranslation(7.0f, 5.0f, -30.0f);
		m_spCameraComponent->OffsetMatrix().RotateX(0.0 * ToRadians);
		m_spCameraComponent->OffsetMatrix().RotateY(0 * ToRadians);
	}

	Scene::GetInstance().SteTargetCamera(m_spCameraComponent);

	m_CamMat.RotateZ((0 * ToRadians));
	m_CamMat.RotateY((-180 * ToRadians));
	m_spCameraComponent->SetCameraMatrix(m_CamMat);
	m_spModelComponent->SetModel(ResFac.GetModel(string));

	m_spTitleTex = ResFac.GetTexture("Data/Texture/Title/Title.png");

	m_SelectPos.x = 0;
	m_SelectPos.y = -270;

	m_Select001Pos.x = 400;
	m_Select001Pos.y = -270;

	m_Select002Pos.x = 800;
	m_Select002Pos.y = -270;

	m_ArrowPos.x = -500;
	m_ArrowPos.y = -270;

	m_Arrow001Pos.x = 500;
	m_Arrow001Pos.y = -270;

	KD_AUDIO.Play("Data/Audio/BGM/loop100302.wav", true);

	m_spPoworUp = std::make_shared<CharacterUpdate>();

	for (UINT i = 0; i < 100; i++)
	{
		fallSnowTex[i] = std::make_shared< AinmationEffect>();
		falleffectMat[i].SetTranslation(RND * 30 - 15, RND * 20, RND * 30 - 15);

		fallSnowTex[i]->SetAnimationInfo(ResFac.GetTexture("Data/Texture/White1.png"), 0.4f, 1, 1, 0, 0, 0);
		fallSnowTex[i]->SetMatrix(falleffectMat[i]);
		m_FalleffectPos[i] = falleffectMat[i].GetTranslation();
		Scene::GetInstance().AddObject(fallSnowTex[i]);
	}
	falleffectposY = 0.02f;
}

void Title::Update()
{
	POINT nowMousePos;
	HWND hwnd;
	hwnd = APP.m_window.GetWndHandle();

	GetCursorPos(&nowMousePos);				//マウス現在位置の取得
	ScreenToClient(hwnd, &nowMousePos);		//マウスをクライアント座標に

	nowMousePos.x -= 640;
	nowMousePos.y -= 360;

	MousePos.x = nowMousePos.x;
	MousePos.y = nowMousePos.y;

	if (m_isTitleScene)
	{
		m_CposY = -80 + 10 * sin(m_CspeedY);
		m_CspeedY += 0.1f;

		if (m_ScrollSpeed == 0.0f)
		{
			if (Collision2D(m_Arrow001Pos, MousePos, 25, 25))
			{
				if (m_Arrow001scale < 0.85f) { m_Arrow001scale += 0.01f; }
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					//m_isTitleScene = false;
					if (m_canChange)
					{

						m_mStart.SetTranslation(m_SelectPos);
						m_mStart001.SetTranslation(m_Select001Pos);
						m_mStart002.SetTranslation(m_Select002Pos);

						m_vGoal = Vec3(m_SelectPos.x - 400, -270, 0);
						m_vGoal001 = Vec3(m_Select001Pos.x - 400, -270, 0);
						m_vGoal002 = Vec3(m_Select002Pos.x - 400, -270, 0);

						flgs++;

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

			if (Collision2D(m_ArrowPos, MousePos, 25, 25))
			{
				if (m_Arrowscale < 0.85f) { m_Arrowscale += 0.01f; }
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					if (m_canChange)
					{
						m_mStart.SetTranslation(m_SelectPos);
						m_mStart001.SetTranslation(m_Select001Pos);
						m_mStart002.SetTranslation(m_Select002Pos);

						m_vGoal = Vec3(m_SelectPos.x + 400, -270, 0);
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
				if (m_Arrowscale > 0.7) { m_Arrowscale -= 0.01f; }
			}
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
				if (m_Arrow001scale > 0.7) { m_Arrow001scale -= 0.01f; }
				if (m_Arrowscale > 0.7) { m_Arrowscale -= 0.01f; }

				m_ScrollSpeed += 0.01f;
			}
			else {
				m_CanScroll = false;
			}
		}
		else
		{
			m_ScrollSpeed = 0.0f;
		}


		

		if (flgs == 1) {
			string = "Data/StageMap/StageMap.gltf";
			m_spModelComponent->SetModel(ResFac.GetModel(string));

			if (Collision2D(m_SelectPos, MousePos, 100, 100))
			{
				if (m_scale < 0.85f) { m_scale += 0.01f; }
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					if (m_canChange)
					{
						flg = true;
						m_canChange = false;
						m_stagename = "Stage_1";
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
			string = "Data/StageMap/Unlock.gltf";
			m_spModelComponent->SetModel(ResFac.GetModel(string));
			if (Collision2D(m_Select001Pos, MousePos, 100, 100))
			{
				if (m_scale001 < 0.85f) { m_scale001 += 0.01f; }
				if (m_stageclearcnt < 1) { return; }

				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					if (m_canChange)
					{
						flg = true;
						m_canChange = false;
						m_stagename = "CarPark";
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
			string = "Data/StageMap/Unlock.gltf";
			m_spModelComponent->SetModel(ResFac.GetModel(string));
			if (Collision2D(m_Select002Pos, MousePos, 100, 100))
			{
				if (m_scale002 < 0.85f) { m_scale002 += 0.01f; }
				if (m_stageclearcnt < 2) { return; }

				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					if (m_canChange)
					{
						flg = true;
						m_canChange = false;
						m_stagename = "Bridge";					
					}
				}
				else { m_canChange = true; }
			}
			else
			{
				if (m_scale002 > 0.7) { m_scale002 -= 0.01f; }
			}
		}

		if (m_black > 0.6)
		{

			KD_AUDIO.StopBGM();
			Scene::GetInstance().RequestChangeScene("Data/Scene/" + m_stagename + ".json");
		}
	}
	/*else
	{
		m_spPoworUp->Update();
		m_spPoworUp->Collision();
		m_isTitleScene= m_spPoworUp->Collision();
	}*/

	//ImguiUpdate();

	for (UINT i = 0; i < 100; i++)
	{
		m_FalleffectPos[i].y -= falleffectposY;
		falleffectMat[i].Scale(0.99, 0.99, 0.99);
		falleffectMat[i].SetTranslation(m_FalleffectPos[i]);
		fallSnowTex[i]->SetMatrix(falleffectMat[i]);
		if (falleffectMat[i].GetTranslation().y < 0)
		{
			falleffectMat[i].CreateScalling(1, 1, 1);
			m_FalleffectPos[i].Move(RND * 30 - 15, RND * 20, RND * 30 - 15);
		}
	}
}

void Title::Draw2D()
{
	if (m_isTitleScene)
	{
	/*	m_spTitleTex = ResFac.GetTexture("Data/Texture/Title/Title.png");

		m_TitleMat.SetTranslation(Vec3(0, 0, 0));
		SHADER.m_spriteShader.SetMatrix(m_TitleMat);
		SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);*/

		if (flg)
		{
			m_black += 0.01f;
			SHADER.m_postProcessShader.ColorDraw(m_spTitleTex.get(), DirectX::SimpleMath::Vector4(0, 0, 0, m_black));
		}
		else
		{
			//SHADER.m_postProcessShader.ColorDraw(m_spTitleTex.get(), DirectX::SimpleMath::Vector4(0,0,0,0));
		}

		m_spTitleTex = ResFac.GetTexture("Data/Texture/Title/Hiroba.png");
		m_SelectMat.CreateScalling(m_scale, m_scale, 0);
		m_SelectMat.SetTranslation(m_SelectPos.x, m_SelectPos.y, m_SelectPos.z);
		SHADER.m_spriteShader.SetMatrix(m_SelectMat);
		SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);

		m_spTitleTex = ResFac.GetTexture("Data/Texture/Title/lock.png");
		m_Select001Mat.CreateScalling(m_scale001, m_scale001, 0);
		m_Select001Mat.SetTranslation(m_Select001Pos.x, m_Select001Pos.y, m_Select001Pos.z);
		SHADER.m_spriteShader.SetMatrix(m_Select001Mat);
		SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);

		m_spTitleTex = ResFac.GetTexture("Data/Texture/Title/lock.png");
		m_Select002Mat.CreateScalling(m_scale002, m_scale002, 0);
		m_Select002Mat.SetTranslation(m_Select002Pos.x, m_Select002Pos.y, m_Select002Pos.z);
		SHADER.m_spriteShader.SetMatrix(m_Select002Mat);
		SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);

		m_spTitleTex = ResFac.GetTexture("Data/Texture/Title/arrow.png");
		m_ArrowMat.CreateScalling(m_Arrowscale, m_Arrowscale, 0);
		m_ArrowMat.SetTranslation(m_ArrowPos);
		SHADER.m_spriteShader.SetMatrix(m_ArrowMat);
		SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);

		m_spTitleTex = ResFac.GetTexture("Data/Texture/Title/arrow.png");
		m_Arrow001Mat.CreateScalling(m_Arrow001scale, m_Arrow001scale, 0);
		m_Arrow001Mat.RotateY(180 * ToRadians);
		m_Arrow001Mat.SetTranslation(m_Arrow001Pos);
		SHADER.m_spriteShader.SetMatrix(m_Arrow001Mat);
		SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);

		m_spTitleTex = ResFac.GetTexture("Data/Texture/Title/ClickToStart.png");
		m_ClickToStartMat.CreateScalling(1.3, 1, 0);
		m_ClickToStartMat.SetTranslation(-35, m_CposY, 1);
		SHADER.m_spriteShader.SetMatrix(m_ClickToStartMat);
		SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);
	}
	/*else
	{
		m_spPoworUp->Draw2D();
	}*/

}

void Title::Draw()
{
	if (m_spModelComponent == nullptr) { return; }

	m_spModelComponent->Draw();
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

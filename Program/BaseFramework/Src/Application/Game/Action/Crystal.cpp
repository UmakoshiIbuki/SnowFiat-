#include "Crystal.h"
#include "Human.h"
#include "Enemy.h"
#include "../Scene.h"
#include"../Particle.h"
#include"../../Component//ModelComponent.h"

void Crystal::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	m_pos = m_mWorld.GetTranslation();
	m_pos.y += 1;

	m_colRadius = 3;
	m_dissolveThreshold = 0;
	m_spTex = ResFac.GetTexture("Data/Texture/UITexture/UI_Crystal.png");
	m_spTex1 = ResFac.GetTexture("Data/Texture/UITexture/UI_Drain.png");
}

void Crystal::UpdateCollision()
{
	SphereInfo Info;
	Info.m_pos = m_mWorld.GetTranslation();		
	Info.m_radius = m_colRadius;				//動いた方向に向かって

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		//自分自身は無視
		if (obj.get() == this) { continue; }

		bool hit = false;

		//球判定
		if (obj->GetTag() & TAG_Character)
		{
			if (obj->HitCheckBySphere(Info))
			{
				hit = true;
			}
		}

		if (hit)
		{
			std::shared_ptr<Human> human = std::dynamic_pointer_cast<Human>(obj);
			if (human)
			{
				human->CanDrain(true);
				m_dissolveThreshold=human->m_CrystalDrain;
				m_CanDrain = true;
			}			
			std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj);
			if (enemy)
			{
				//enemy->CanTrace(true);
			}
		}
	}
}

void Crystal::Update()
{

	m_frame++;
	//m_LightPower += 0.03;
	SHADER.AddPointLight(m_pos, m_LightPower, Vec3(255,255,255));

	/*if (m_frame % 360 == 0)
	{
		m_LightPower = 0;
		std::shared_ptr<Enemy> spEnemy = std::make_shared<Enemy>();
		if (spEnemy)
		{
			spEnemy->Deserialize(ResFac.GetJSON("Data/Scene/Enemy.json"));

			Scene::GetInstance().AddObject(spEnemy);
		}
	}*/

	this->GetModelComponent()->SetDissolveThreshold(m_dissolveThreshold);
	if (m_dissolveThreshold > 0.5f) {
		ShowCursor(true);
		Scene::GetInstance().RequestChangeScene("Data/Scene/Result.json");
	}
	/*if (ImGui::Begin("Graphics Debug"))
	{
		ImGui::DragFloat("dissolveThreshold", &m_dissolveThreshold, 0.1f);
	}
	ImGui::End();*/

	m_rot += 0.5;
	m_mWorld.CreateRotationY(m_rot*ToRadians);
	m_mWorld.Move(m_pos);

	UpdateCollision();
}

void Crystal::Draw2D()
{
	SHADER.m_spriteShader.DrawTex(m_spTex.get(), -20, -470);
	if (m_CanDrain)
	{
		SHADER.m_spriteShader.DrawTex(m_spTex1.get(), 500, -270);
	}
}

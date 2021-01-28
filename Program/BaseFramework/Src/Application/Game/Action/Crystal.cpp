#include "Crystal.h"
#include "Human.h"
#include "Enemy.h"
#include "../Scene.h"
#include"../Particle.h"
#include"../../Component//ModelComponent.h"

void Crystal::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	pos = m_mWorld.GetTranslation();

	m_dissolveThreshold = 0.5f;

}

void Crystal::UpdateCollision()
{
	SphereInfo Info;
	Info.m_pos = m_mWorld.GetTranslation();				//ひとつ前の場所から
	Info.m_radius = m_colRadius;				//動いた方向に向かって

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		//自分自身は無視
		if (obj.get() == this) { continue; }
		if (!obj->m_LetsMove) { continue; }

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
			//std::dinamic_pointer_cast=基底クラス型をダウンキャストするときに使う。失敗するとnullptrが帰る
			std::shared_ptr<Human> human = std::dynamic_pointer_cast<Human>(obj);
			if (human)
			{
				human->SetCrystal(1);
			}
			Destroy();
		}
	}
}

void Crystal::Update()
{
	//static const std::string filename = "Data/White.png";
	//std::shared_ptr<Particle> particle = std::make_shared< Particle>();
	//particle->SetTextureFile(filename);
	//particle->SetShowTime(30);
	//particle->SetSize(0.5f);
	//particle->SetMove(0.3f, 0.3f, 0.2f, 0.2f);
	//particle->Deserialize(ResFac.GetJSON("Data/Scene/Particle.json"));
	//particle->SetMatrix(mat);

	//Scene::GetInstance().AddObject(particle);
	
	m_dissolveThreshold -= 0.001f;

	this->GetModelComponent()->SetDissolveThreshold(m_dissolveThreshold);

	m_rot += 0.5;
	m_mWorld.CreateRotationY(m_rot*KdToRadians);
	m_mWorld.Move(pos);


	UpdateCollision();
}

void Crystal::ImguiUpdate()
{
	if (ImGui::Begin("Dissolve Debug"))
	//ディゾルブ閾値
	if (ImGui::DragFloat("Dissolve", &m_dissolveThreshold, 0.001, 0.0f, 1.0f))
	{
		this->GetModelComponent()->SetDissolveThreshold(m_dissolveThreshold);
	}
	ImGui::End();
}
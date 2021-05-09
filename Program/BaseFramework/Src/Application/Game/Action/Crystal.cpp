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
	pos.y += 1;

	m_colRadius = 3;
	m_dissolveThreshold = 0;
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
		if (obj->Getname()=="PlayerHuman")
		{
			if (obj->HitCheckBySphere(Info))
			{
				hit = true;
				m_CanDrain = true;
			}
		}

		if (hit)
		{
			std::shared_ptr<Human> human = std::dynamic_pointer_cast<Human>(obj);
			if (human)
			{
				human->CanDrain(true);
				m_dissolveThreshold=human->m_CrystalDrain;
			}			
			std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj);
			if (enemy)
			{
				enemy->CanTrace(true);
			}
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
	
	this->GetModelComponent()->SetDissolveThreshold(m_dissolveThreshold);
	

	m_rot += 0.5;
	m_mWorld.CreateRotationY(m_rot*ToRadians);
	m_mWorld.Move(pos);

	UpdateCollision();
}
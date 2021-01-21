#include "Crystal.h"
#include "Human.h"
#include "Enemy.h"
#include "../Scene.h"
#include"../Particle.h"

void Crystal::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	mat.SetTranslation(m_mWorld.GetTranslation().x + 1.5, m_mWorld.GetTranslation().y+2,m_mWorld.GetTranslation().z);
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
			std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj);
			if (enemy)
			{
				enemy->Damage(10);
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
	UpdateCollision();
}

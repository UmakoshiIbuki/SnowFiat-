#include "Crystal.h"
#include "Human.h"
#include "Enemy.h"
#include "../Scene.h"

void Crystal::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
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
	UpdateCollision();
}

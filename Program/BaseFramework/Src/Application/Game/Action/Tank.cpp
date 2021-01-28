#include "Tank.h"
#include"../Scene.h"
#include"Human.h"
#include"../../Component//ModelComponent.h"

void Tank::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	m_spTankTex = ResFac.GetTexture("Data/Texture/UITexture/UI_BER.png");

	m_sphuman = Scene::GetInstance().FindObjectWithName("PlayerHuman");
	
	pos = m_mWorld.GetTranslation();

	m_dissolveThreshold = 0.5f;
}

void Tank::Update()
{
	this->GetModelComponent()->SetDissolveThreshold(m_dissolveThreshold);

	if (m_crystal >= 5)
	{
		Scene::GetInstance().RequestChangeScene("Data/Scene/Result.json");
	}

	m_hit = false;

	m_rot += 0.5;
	m_mWorld.CreateRotationY(m_rot * KdToRadians);
	m_mWorld.Move(pos);

	UpdateCollision();	

	if (!m_hit) { return; }

	std::shared_ptr<Human> human = std::dynamic_pointer_cast<Human>(m_sphuman);
	if (human)
	{
		m_crystal += human->GetCrystal();
		human->SetCrystal(-human->GetCrystal());
		m_dissolveThreshold = 0.5f-(float)m_crystal / 10;
	}
}

void Tank::UpdateCollision()
{
	SphereInfo sInfo;
	sInfo.m_pos = m_mWorld.GetTranslation();
	sInfo.m_radius = 1;

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		//自分自身
		if (obj.get() == this) { continue; }
		{
			//キャラクター以外は無視
			if (!(obj->GetTag() & TAG_Character)) { continue; }

			//当たり判定
			if (obj->HitCheckBySphere(sInfo))
			{
				Scene::GetInstance().AddDebugSphereLine(sInfo.m_pos, sInfo.m_radius, { 1,0,0,1 });
				m_hit = true;
			}
		}
	}
}

void Tank::Draw2D()
{
	if (!m_hit) { return; }
	m_TankMat.SetTranslation(0,0,0);
	SHADER.m_spriteShader.SetMatrix(m_TankMat);
	SHADER.m_spriteShader.DrawTex(m_spTankTex.get(), 0, 0);
}

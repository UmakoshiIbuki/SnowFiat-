#pragma once
#include"../GameObject.h"

class Human;

class Wall :public GameObject
{
public:

	 void Deserialize(const json11::Json& jsonObj) override;
	 void Update()override;
	 void Draw2D();

	 inline void SetOwner(const std::shared_ptr<GameObject>& spOwner) { m_wpOwner = spOwner; }
private:
	int		m_wallHeight = 0;
	float	m_makeSpeed=0.0f;
	Matrix	m_ui;
	int		m_ShowUiTime;
	int     m_Life = 150;
	Vec3    pos;
	Vec3    scale;
	std::weak_ptr<GameObject>m_wpOwner;		
	std::shared_ptr<KdTexture> m_spCollTimeTex;
	std::shared_ptr<GameObject> m_sphuman = std::make_shared<GameObject>();

};
#include "Lamp.h"

void Lamp::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	m_light = 4.7;
}

void Lamp::Update()
{
	Vec3 pos = m_mWorld.GetTranslation();
	pos.y += m_light;
	SHADER.AddPointLight(pos, 6, { 50.0f,50.0f,50.0f });
	ImGuiUpdate();
}

void Lamp::ImGuiUpdate()
{

	if (ImGui::Begin("LightSettings"))
	{

		if (ImGui::DragFloat("Light", &m_light, 0.01f))
		{
			m_light=m_light;
		}
	}
	ImGui::End();
}

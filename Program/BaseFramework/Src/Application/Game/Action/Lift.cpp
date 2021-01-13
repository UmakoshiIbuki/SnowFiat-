#include "Lift.h"
#include"Human.h"
#include"../Scene.h"

void Lift::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	//最初の地点を覚える
	m_mStart = m_mWorld;

	//移動先情報の読み込み
	if (jsonObj["MoveTo"].is_array())
	{
		auto& p = jsonObj["MoveTo"].array_items();
		m_vGoal = Vec3(p[0].number_value(), p[1].number_value(), p[2].number_value());
	}
	
	//移動スピード読み込み
	if (jsonObj["Speed"].is_number())
	{
		m_speed = jsonObj["Speed"].number_value();
	}
	m_sphuman = Scene::GetInstance().FindObjectWithName("PlayerHuman");

}

void Lift::Update()
{
	m_mPrev = m_mWorld;

	auto& vStart = m_mStart.GetTranslation();
	auto& vGoal = m_vGoal;

	Vec3 vTo = vGoal - vStart;//ゴール地点のベクトル
	Vec3 vNow = vStart + vTo * sin(easeInOut(m_progress));//進行具合を加味して座標を求める

	//m_mWorld.SetTranslation(vNow);

	if ((m_sphuman->GetMatrix().GetTranslation().y - m_mWorld.GetTranslation().y)==1)
	{
		//進行具合の更新
		//m_progress += m_speed;
		m_posY += m_speed;
		m_mWorld.SetTranslation(m_mWorld.GetTranslation().x,m_posY,m_mWorld.GetTranslation().z);

	}
}

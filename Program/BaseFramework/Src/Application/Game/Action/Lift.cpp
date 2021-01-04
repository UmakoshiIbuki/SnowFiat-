#include "Lift.h"

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
}

void Lift::Update()
{
	m_mPrev = m_mWorld;

	auto& vStart = m_mStart.GetTranslation();
	auto& vGoal = m_vGoal;

	Vec3 vTo = vGoal - vStart;//ゴール地点のベクトル
	Vec3 vNow = vStart + vTo * sin(easeInOut(m_progress));//進行具合を加味して座標を求める

	m_mWorld.SetTranslation(vNow);
	//進行具合の更新
	m_progress += m_speed;

}

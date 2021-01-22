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
	m_Floor3Pos = Vec3(45.86, 30.46, -15);
	m_Floor2Pos = Vec3(45.86, 19.86, -15);
	m_Floor1Pos = Vec3(45.86, 8.866, -15);
}

void Lift::Update()
{
	m_mPrev = m_mWorld;

	if(GetAsyncKeyState(VK_RETURN)&0x8000)
	{
		m_progress = 0.0f;
		SwitchOn = 4;
		m_OnceClick = false;

	}
	else {
		m_OnceClick = true;
	}

	if (GetAsyncKeyState('E') )
	{
		m_progress = 0.0f;
		SwitchOn = 1;
		m_OnceClick = false;

	}
	else {
		m_OnceClick = true;
	}

	if (GetAsyncKeyState('R'))
	{
		m_progress = 0.0f;
		SwitchOn = 2;
		m_OnceClick = false;

	}
	else {
		m_OnceClick = true;
	}

	if (GetAsyncKeyState('T') )
	{
		m_progress = 0.0f;
		SwitchOn = 3;
		m_OnceClick = false;

	}
	else {
		m_OnceClick = true;
	}

	if (SwitchOn==4)
	{

		auto& vStart = m_mWorld.GetTranslation();
		auto& vGoal = m_sphuman->GetMatrix().GetTranslation();

		Vec3 vTo = vGoal - vStart;//ゴール地点のベクトル
		Vec3 vNow = vStart + vTo * sin(easeInOut(m_progress));//進行具合を加味して座標を求める

		m_mWorld.SetTranslation(vStart.x, vNow.y, vStart.z);
		m_progress += m_speed;
		if (m_progress > 1) { SwitchOn=0; }
	}

	if (SwitchOn == 1)
	{

		auto& vStart = m_mWorld.GetTranslation();
		auto& vGoal = m_Floor1Pos;

		Vec3 vTo = vGoal - vStart;//ゴール地点のベクトル
		Vec3 vNow = vStart + vTo * sin(easeInOut(m_progress));//進行具合を加味して座標を求める

		m_mWorld.SetTranslation(vStart.x, vNow.y, vStart.z);
		m_progress += m_speed;
		if (m_progress > 1) { SwitchOn = false; }
	}
	
	if (SwitchOn == 2)
	{

		auto& vStart = m_mWorld.GetTranslation();
		auto& vGoal = m_Floor2Pos;

		Vec3 vTo = vGoal - vStart;//ゴール地点のベクトル
		Vec3 vNow = vStart + vTo * sin(easeInOut(m_progress));//進行具合を加味して座標を求める

		m_mWorld.SetTranslation(vStart.x, vNow.y, vStart.z);
		m_progress += m_speed;
		if (m_progress > 1) { SwitchOn = false; }
	}

	if (SwitchOn==3)
	{

		auto& vStart = m_mWorld.GetTranslation();
		auto& vGoal = m_Floor3Pos;

		Vec3 vTo = vGoal - vStart;//ゴール地点のベクトル
		Vec3 vNow = vStart + vTo * sin(easeInOut(m_progress));//進行具合を加味して座標を求める

		m_mWorld.SetTranslation(vStart.x, vNow.y, vStart.z);
		m_progress += m_speed;
		if (m_progress > 1) { SwitchOn = false; }
	}
}

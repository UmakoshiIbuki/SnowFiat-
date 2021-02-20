#include"Wall.h"
#include"../Scene.h"


void Wall::Deserialize(const json11::Json& jsonObj)
{

	GameObject::Deserialize(jsonObj);
	m_spCollTimeTex = ResFac.GetTexture("Data/Texture/UITexture/UI_GURUGURU.png");
	m_sphuman =Scene::GetInstance().FindObjectWithName("PlayerHuman");

	scale.x = 1.001f;
	scale.y = 1;
	scale.z = 1.001f;

}
void Wall::Update()
{

	SphereInfo sinfo;
	sinfo.m_pos = m_sphuman->GetMatrix().GetTranslation();
	sinfo.m_radius = 5;
	if (!HitCheckBySphere(sinfo)) { return; }

	pos = m_mWorld.GetTranslation();

	Vec3 move;
	
	if (--m_Life <= 0)
	{
		//m_makeSpeed = -0.01f;
		//move.y = m_makeSpeed;
		//m_mWorld.Move(move);
		//if(m_mWorld.GetTranslation().y<-1){Destroy();}
	}

	m_mWorld.Scale(scale.x, scale.y, scale.z);
	m_gravity += 0.01f;
	pos.y -= m_gravity;

	if (m_Life >= 0)
	{
		if (m_wallHeight < 100)
		{

			/*m_wallHeight++;
			m_makeSpeed = 0.01f;
			move.y = m_makeSpeed;*/
			//scale.x += 0.01;
		

		}
	}
	
	move.y = 0;

	SphereInfo info;

	info.m_pos = pos;		
	info.m_radius = 0.2f;
	CheckBump();
	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		if (obj.get() == this) { continue; }
		SphereResult Result;
		if (obj->GetTag() == TAG_StageObject)
		{
			if (obj->HitCheckBySphereVsMesh(info,Result))
			{
				pos.Move(Result.m_push);
				m_gravity = 0.0f;
			}
		}
	}

	m_mWorld.SetTranslation(pos);

}

void Wall::Draw2D()
{
	
	if (!m_spCollTimeTex) { return; }
	//2D描画
	m_ui.RotateZ(10 * KdToRadians);
	SHADER.m_spriteShader.SetMatrix(m_ui);
	SHADER.m_spriteShader.DrawTex(m_spCollTimeTex.get(), 0, 0);
	m_ShowUiTime++;
	if (m_ShowUiTime>100){ m_spCollTimeTex.reset();}
}

void Wall::CheckBump()
{
	SphereInfo info;

	info.m_pos = pos;		//中心点キャラクターの位置
	info.m_pos.y += 0.4f;	//キャラクターのぶつかり判定をするので、ちょっと上に持ち上げる
	info.m_radius = 0.4f;	//キャラクターの大きさに合わせて半径サイズもいい感じに設定する

	Scene::GetInstance().AddDebugSphereLine(info.m_pos, info.m_radius, { 1,1,1,1 });

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		if (obj.get() == this) { continue; }

		SphereResult Result;
		if (obj->HitCheckBySphereVsMesh(info, Result))
		{
			{
				pos.Move(Result.m_push);
			}
		}
	}

}
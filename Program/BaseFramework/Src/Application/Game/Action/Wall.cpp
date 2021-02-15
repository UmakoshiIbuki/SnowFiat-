#include"Wall.h"
#include"../Scene.h"


void Wall::Deserialize(const json11::Json& jsonObj)
{

	GameObject::Deserialize(jsonObj);
	m_spCollTimeTex = ResFac.GetTexture("Data/Texture/UITexture/UI_GURUGURU.png");
	m_sphuman =Scene::GetInstance().FindObjectWithName("PlayerHuman");
	pos = m_sphuman->GetMatrix().GetTranslation();
	pos.y -= 0.5f;

	scale.x = 1;
}
void Wall::Update()
{
	Vec3 move;

	if (--m_Life <= 0)
	{
		//m_makeSpeed = -0.01f;
		//move.y = m_makeSpeed;
		//m_mWorld.Move(move);
		//if(m_mWorld.GetTranslation().y<-1){Destroy();}
	}

	m_mWorld.Scale(1.001, 1, 1.001);
	m_mWorld.SetTranslation(pos);
	if (m_Life >= 0)
	{
		if (m_wallHeight < 100)
		{

			/*m_wallHeight++;
			m_makeSpeed = 0.01f;
			move.y = m_makeSpeed;*/
			scale.x += 0.01;
		

		}
	}
	
	move.y = 0;
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

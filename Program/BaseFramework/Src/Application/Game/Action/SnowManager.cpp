#include "SnowManager.h"
#include"../../Component//InputComponent.h"
#include"../Scene.h"
#include"Human.h"

void SnowManager::Deserialize(const json11::Json& jsonObj)
{

}

void SnowManager::Draw2DTex(float f1,float f2)
{	
	m_spSnowGageTex = ResFac.GetTexture("Data/Texture/UITexture/UI_BER.png");

	m_spSnowGageMat.CreateScalling(1.0f, 1.0f, 1.0f);
	m_spSnowGageMat.SetTranslation(Vec3(-400, -340, 0));
	SHADER.m_spriteShader.SetMatrix(m_spSnowGageMat);
	SHADER.m_spriteShader.DrawTex(m_spSnowGageTex.get(), 0, 0);

	m_spSnowGageTex = ResFac.GetTexture("Data/Texture/UITexture/UI_BER1.png");

	m_spSnowGageMat.CreateScalling(f1 / 3, 1.0f, 1.0f);
	m_spSnowGageMat.SetTranslation(Vec3(-400 - (40 * f2), -340, 0));
	SHADER.m_spriteShader.SetMatrix(m_spSnowGageMat);
	SHADER.m_spriteShader.DrawTex(m_spSnowGageTex.get(), 0, 0);
}

void SnowManager::Draw2DHP(float f1,int f2)
{
	m_spBackTex = ResFac.GetTexture("Data/Texture/UITexture/back.png");
	m_spBackMat.CreateScalling(1.5f, 1.0f, 1.0f);
	m_spBackMat.SetTranslation(Vec3(-410, -340, 0));
	SHADER.m_spriteShader.SetMatrix(m_spBackMat);
	SHADER.m_spriteShader.DrawTex(m_spBackTex.get(), 0, 0);

	m_spHpTex = ResFac.GetTexture("Data/Texture/UITexture/UI_Hp_00.png");
	m_spHpBerTex = ResFac.GetTexture("Data/Texture/UITexture/UI_HP_BER.png");

	m_HpMat.CreateScalling(f1 / 100, 0.65f, 1.0f);
	m_HpMat.SetTranslation(Vec3((float)-400 - f2, -285, 0));
	SHADER.m_spriteShader.SetMatrix(m_HpMat);
	SHADER.m_spriteShader.DrawTex(m_spHpTex.get(), 0, 0);

	m_HpMat.CreateScalling(0.4f, 0.25f, 1.0f);
	m_HpMat.SetTranslation(Vec3(-400, -280, 0));
	SHADER.m_spriteShader.SetMatrix(m_HpMat);
	SHADER.m_spriteShader.DrawTex(m_spHpBerTex.get(), 0, 0);

	

}

void SnowManager::Draw2D()
{
	
}

void SnowManager::Draw2DRemaining(float snow)
{
	/*ChangeTex(snow);
	m_SnowRemainingMat.CreateScalling(0.25f, 0.25f, 1.0f);
	m_SnowRemainingMat.SetTranslation(Vec3(-170, -315, 0));
	SHADER.m_spriteShader.SetMatrix(m_SnowRemainingMat);
	SHADER.m_spriteShader.DrawTex(m_spSnowRemainingTex.get(), 0, 0);*/
}

void SnowManager::Update()
{
	////パーティクル更新
	//for (int i = 0; i < particleNum; i++)
	//{
	//	UpdateParticle(&smoke[i]);
	//}
	
}

void SnowManager::EmitParticle(PARTICLE* p, float aPosX, float aPosY, float aMoveX, float aMoveY, int size, int LifeSpan)
{

}

void SnowManager::UpdateParticle(PARTICLE* p)
{

}

void SnowManager::DrawParticle(PARTICLE* p)
{

}

void SnowManager::ChangeTex(float m_snow)
{
	switch ((int)m_snow)
	{
	case 3:
		m_spSnowRemainingTex = ResFac.GetTexture("Data/Texture/UITexture/UI_BUCKET.png");
		break;
	case 2:
		m_spSnowRemainingTex = ResFac.GetTexture("Data/Texture/UITexture/UI_BUCKET.png");
		break;
	case 1:
		m_spSnowRemainingTex = ResFac.GetTexture("Data/Texture/UITexture/UI_BUCKET.png");
		break;
	case 0:
		m_spSnowRemainingTex = ResFac.GetTexture("Data/Texture/UITexture/UI_BUCKET.png");
		break;
	}
}
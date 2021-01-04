#include "Bridge.h"
#include"Scene.h"
#include"Action/ActionGameProcess.h"

Bridge::Bridge()
{
	m_spCrystals = std::make_shared<ActionGameProcess>();
	m_sphuman = std::make_shared<Human>();
}

void Bridge::Update()
{
	m_crystalOne = m_spCrystals->GetOne();
	m_crystalTen = m_spCrystals->GetTen();

	m_hitCntOne = m_sphuman->GetHitCntOne();
	m_hitCntTen = m_sphuman->GetHitCntTen();
}

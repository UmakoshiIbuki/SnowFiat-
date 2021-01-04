#pragma once
#include"Action/ActionGameProcess.h"
#include"Action/Human.h"

class Bridge
{
public:
	Bridge();
	void Update();
	int GetOne() { return m_crystalOne; }
	int GetTen() { return m_crystalTen; }

	int GetHitCntOne() { return m_hitCntOne; }
	int GetHitCntTen() { return m_hitCntTen; }

private:
	int m_crystalOne = 0;
	int m_crystalTen = 0;

	int m_hitCntOne = 0;
	int m_hitCntTen = 0;
	std::shared_ptr<ActionGameProcess>m_spCrystals=nullptr;
	std::shared_ptr<Human> m_sphuman = nullptr;
};
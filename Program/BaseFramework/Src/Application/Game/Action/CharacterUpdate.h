#pragma once

class Title;
class CharacterUpdate
{
public:
	CharacterUpdate();
	~CharacterUpdate();

	void Update();
	void Draw2D();
	bool Collision();
	void LoadJson();
private:
	float test = 12;

	std::shared_ptr<KdTexture> m_spReplayTex;
	Matrix					   m_PoworMat;
	Vec3					   m_PoworPos;

	std::shared_ptr<KdTexture> m_spReturnTex;
	Matrix					   m_ReturnMat;
	Vec3					   m_ReturnPos;

	Vec3                       MousePos;

	std::shared_ptr<Title>	   m_spTitle;

	Vec3					  m_pos;

};
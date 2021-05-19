#pragma once
#include"../GameObject.h"

class Human;
class AinmationEffect;

class Enemy:public GameObject
{
public:

	virtual void Deserialize(const json11::Json& jsonObj) override;
	virtual void Update()override;
	void DebugDraw2D();

	void UpdateShoot();							//発射関数
	void Reload();

	void CheckBump();

	bool IsGround() { return m_isGround; }

	void Damage(int damage);

	void SetAnimation(const char* pAnimName);
	void SetTargetMat(Matrix targetMat) { m_TraceMat = targetMat; }
	inline void SetTarget(const std::shared_ptr<GameObject>& spTarget) { m_wpTarget = spTarget; }

	inline void SetRespawnTime(int time) { m_RespawnTime = time; }
	inline void SetHp(int hp) { m_hp = hp; }
	void CanTrace(bool flg) { m_canTrace = flg; }
	bool CanSee();
	void Back();

	inline int GetHp() { return m_hp; }

private:
	std::shared_ptr<KdAnimationData> m_spAnimation = nullptr;
	float m_animationTime = 0.0f;

	Vec3		m_pos;
	Vec3		m_prevPos;
	Vec3		m_force;						
	bool		m_notMove = false;

	bool CanShoot();

	void UpdateRotate(const Vec3& rMoveDir);    //操作やキャラクターの行動による回転計算
	float		m_rotateAngle = 10.0f;			//キャラクターの回転速度
	Vec3		m_rot;						    //ワールド行列の回転角度 


	void UpdateCollision();
	bool CheckGround(float& rDistance,UINT m_tag,Vec3 Pos);

	static const float s_allowToStepHeight;		//歩いて乗り越えられる段差の高さ
	static const float s_landingHeight;			//地面から足が離れていても着地しているとする高さ(坂道などを下るときに宙に浮くのを避ける)

	bool		m_isGround;
	
	UINT		m_tag = OBJECT_TAG::TAG_None;
	float		Shot = 0;

	float		m_charge = 100.0f;		//雪玉のリロード
	bool        m_canShoot;				//発射可能かどうか
	bool		m_makeWall;

	float m_length;
	Vec3 m_hitVec;

	std::shared_ptr<GameObject> m_sphuman ;
	std::shared_ptr<GameObject> m_spCrystal;
	std::vector<Matrix> m_EnemyMat;
	bool locate;
	//bool SearchPlayer(float& rDstDistance, UINT m_tag, Vec3 Pos);
	int pos = 0;//ランダム用
	float m_hp = 9;

	int frame = 0;
	bool canMove=false;

	int m_RespawnTime = 0;

	float posY;

	KdAnimator m_animator;

	float       m_jumpPow = 0.2f;
	int         m_cnt = 0;

	std::weak_ptr<GameObject>m_wpTarget;

	void Trace();
	Matrix	m_TraceMat;
	void Escape();
	int m_canTrace=0;
	bool m_IsHitRange = false;
	int  m_hitRange = 9;

	std::shared_ptr<AinmationEffect> m_spBikkurieffect;
	Matrix							 m_BikkurieffectMat;

	int pX = -10;
	int pY = -10;
	Vec3 m_EnemyPos;


};
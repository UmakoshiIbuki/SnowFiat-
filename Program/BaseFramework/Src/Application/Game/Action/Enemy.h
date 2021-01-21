#pragma once
#include"../GameObject.h"

class Human;
class AinmationEffect;

class Enemy:public GameObject
{
public:

	virtual void Deserialize(const json11::Json& jsonObj) override;
	virtual void Update()override;

	void UpdateShoot();							//発射関数
	void Reload();

	void MakeWall();

	void CheckBump();

	bool IsGround() { return m_isGround; }

	void Damage(int damage);

	void SetAnimation(const char* pAnimName);

	inline void SetTarget(const std::shared_ptr<GameObject>& spTarget) { m_wpTarget = spTarget; }

	inline void SetRespawnTime(int time) { m_RespawnTime = time; }
	inline void SetHp(int hp) { m_hp = hp; }

	inline int GetHp() { return m_hp; }

private:
	std::shared_ptr<KdAnimationData> m_spAnimation = nullptr;
	float m_animationTime = 0.0f;

	Vec3		m_pos;
	bool		m_notMove = false;

	void UpdateRotate(const Vec3& rMoveDir);    //操作やキャラクターの行動による回転計算
	float		m_rotateAngle = 10.0f;			//キャラクターの回転速度
	Vec3		m_rot;						    //ワールド行列の回転角度 

	Vec3		m_force;						//キャラクターにかかる力

	static const float s_allowToStepHeight;		//歩いて乗り越えられる段差の高さ
	static const float s_landingHeight;			//地面から足が離れていても着地しているとする高さ(坂道などを下るときに宙に浮くのを避ける)

	void UpdateCollision();
	bool CheckGround(float& rDistance,UINT m_tag,Vec3 Pos);

	Vec3		m_prevPos;
	bool		m_isGround;
	
	UINT		m_tag = OBJECT_TAG::TAG_None;
	float		Shot = 0;

	float		m_speed = 0.2f;			//移動スピード

	float		m_charge = 10.0f;		//雪玉のリロード
	bool        m_canShoot;				//発射可能かどうか
	bool		m_makeWall;
	bool CanShoot();
	bool CanMakeWall();
	float m_canWall = 3;

	std::shared_ptr<KdTexture> m_spWallUiTex;
	Matrix	m_WallUi;
	Matrix M;

	std::shared_ptr<GameObject> m_sphuman ;
	std::vector<Matrix> m_EnemyMat;
	bool locate;
	//bool SearchPlayer(float& rDstDistance, UINT m_tag, Vec3 Pos);
	int pX;
	int pY;
	Vec3 m_EnemyPos;
	int i = 1;//ランダム用
	int m_hp = 20;

	int frame = 0;
	bool canMove=false;

	int m_RespawnTime = 0;

	float posY;

	KdAnimator m_animator;

	float       m_jumpPow = 0.2f;
	int         m_cnt = 0;

	std::weak_ptr<GameObject>m_wpTarget;

	void Trace();
	bool m_canTrace = false;

	//基本アクションステート
	class BaseAction
	{
	public:
		virtual void Update(Enemy& rOwner) = 0;
	};

	//やられた
	class ActionCrash :public BaseAction
	{
	public:
		virtual void Update(Enemy& owner)override;

	};

	class ActionEscape :public BaseAction
	{
	public:
		virtual void Update(Enemy& owner)override;
		std::shared_ptr<GameObject> m_sphuman;
	};
	std::shared_ptr<BaseAction>m_spActionState;

	std::shared_ptr<AinmationEffect> m_spBikkurieffect;
	Matrix							 m_BikkurieffectMat;

};
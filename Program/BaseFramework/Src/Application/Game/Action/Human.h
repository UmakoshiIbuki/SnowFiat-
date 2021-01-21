#pragma once
#include"../GameObject.h"

class HumanMini;

class Human:public GameObject
{
public:

	virtual void Deserialize(const json11::Json& jsonObj) override;
	virtual void Update()override;

	bool IsGround() { return m_isGround; }

	void Damage(float damage);

	void SetAnimation(const char* pAnimName);
	void SetChange(float snow) { m_snow = snow; }

	void SetCrystal(int crystal) { m_crystal = crystal; }
	int GetCrystal() { return m_crystal; }

	int GetHitCntOne(){ return m_hitCntOne; }
	int GetHitCntTen(){ return m_hitCntTen; }

	void SetAttackCnt(int attackCnt) { AttackCnt = attackCnt; }
	int GetAttackCntTen(){ return AttackCnt; }
private:
	int AttackCnt = 0;

	bool CanAction();

	//キャラクターの移動関係//////////////////////////////////////////////////////////////////
	void UpdateMove();							//移動更新処理
	float		m_movespeed = 0.02f;				//移動スピード
	Vec3		m_pos;
	Vec3		m_force;						//キャラクターにかかる力
	Vec3		m_rot;						    //ワールド行列の回転角度 
	float		m_rotateAngle = 10.0f;			//キャラクターの回転速度
	Vec3		m_prevPos;						//1フレーム前のキャラクターのPos
	bool        m_hit = false;					//球に当たったか？

	//カメラ/////////////////////////////////////////////////////////////////////////////////
	void UpdaetCamera();
	float		m_camRotSpeed = 0.2f;
	
	//衝突判定///////////////////////////////////////////////////////////////////////////////
	bool		m_isGround;
	void UpdateCollision();
	void CheckBump();
	bool CheckGround(float& rDistance, UINT m_tag, Vec3 Pos);

	UINT		m_tag = OBJECT_TAG::TAG_None;

	//雪玉////////////////////////////////////////////////////////////////////////////////////
	void UpdateShoot();							//発射関数
	void ChargeSnow();
	float		m_snow = 3.0f;		//雪玉のリロード
	bool        m_canShoot;				//発射可能かどうか
	float		m_gather = 0;

	std::shared_ptr<KdTexture> m_spReLoadTex;
	Matrix					   m_ReLoadMat;

	//壁//////////////////////////////////////////////////////////////////////////////////////
	void MakeWall();
	bool		m_makeWall;
	float		m_canWall = 1;

	//HP//////////////////////////////////////////////////////////////////////////////////////
	float		m_hp = 100;

	int         m_hitCntOne = 0;
	int         m_hitCntTen = 0;

	void Draw2D();
	std::shared_ptr<KdTexture> m_spHpTex;			//HPの内側テクスチャ(増減するもの)
	std::shared_ptr<KdTexture> m_spHpBerTex;		//HPの外側のテクスチャ
	Matrix					   m_HpMat;			//描画位置
	int						   m_hpScroll = 0;	//描画位置を合わせる用(位置の調整)	
	
	bool m_notMove = false;
	int  m_canMove = 0;//壁を作った時やリロードの間動かなくする用

	KdAnimator m_animator;

	static const float s_allowToStepHeight;		//歩いて乗り越えられる段差の高さ
	static const float s_landingHeight;			//地面から足が離れていても着地しているとする高さ(坂道などを下るときに宙に浮くのを避ける)
	
	int frame = 0;
	float       m_jumpPow = 0.1f;

	int m_crystal = 0;

	bool IsChangeMove();  // 移動状態に遷移するかどうか
	bool IsChangeJump();  // ジャンプ状態に遷移するかどうか

	void ChangeWait();// 待機に変更
	void ChangeMove();// 移動に変更
	void ChangeJump();// ジャンプに変更

		//基本アクションステート
	class BaseAction
	{
	public:
		virtual void Update(Human& rOwner) = 0;
	};

	//待機モーション
	class ActionWait :public BaseAction
	{
	public:
		virtual void Update(Human& rOwner)override;
	};

	//歩きモーション
	class ActionWalk :public BaseAction
	{
	public:
		virtual void Update(Human& rOwner)override;
	};

	//ジャンプモーション
	class ActionJump :public BaseAction
	{
	public:
		virtual void Update(Human& rOwner)override;
	};

	//やられた
	class ActionDie :public BaseAction
	{
	public:
		virtual void Update(Human& owner)override;
	};


	Matrix m_CamMat;
	std::shared_ptr<BaseAction>m_spActionState;

};
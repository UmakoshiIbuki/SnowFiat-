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

	void SetCrystal(int crystal) { m_crystal += crystal; }
	int GetCrystal() { return m_crystal; }

	int GetHitCntOne(){ return m_hitCntOne; }
	int GetHitCntTen(){ return m_hitCntTen; }

	void SetAttackCnt(int attackCnt) { AttackCnt = attackCnt; }
	int GetAttackCntTen(){ return AttackCnt; }

	void CanDrain(bool flg) { m_CanDrain = flg; }
	float		m_power = 0.0f;
	float m_CrystalDrain;

private:
	int AttackCnt = 0;

	bool CanAction();

	//キャラクターの移動関係//////////////////////////////////////////////////////////////////
	void UpdateMove();							//移動更新処理
	void Crouch();
	float		m_movespeed = 0.02f;				//移動スピード
	Vec3		m_pos;
	Vec3		m_force;						//キャラクターにかかる力
	Vec3		m_rot;						    //ワールド行列の回転角度 
	float		m_rotateAngle = 10.0f;			//キャラクターの回転速度
	Vec3		m_prevPos;						//1フレーム前のキャラクターのPos
	float       m_jumpPow = 0.1f;
	bool		m_jumpflg = true;
	Vec3        m_memoryPos;
	float       m_crouch;
	float       m_crouchSpeed=0.1f;


	//カメラ/////////////////////////////////////////////////////////////////////////////////
	void UpdaetCamera();
	float		m_camRotSpeed = 0.2f;
	
	//衝突判定///////////////////////////////////////////////////////////////////////////////
	void UpdateCollision();
	void CheckBump();
	bool CheckGround(float& rDistance, UINT m_tag, Vec3 Pos);
	bool        m_hit = false;					//球に当たったか？
	bool		m_isGround;
	UINT		m_tag = OBJECT_TAG::TAG_None;

	//雪玉////////////////////////////////////////////////////////////////////////////////////
	void UpdateShoot();
	void DrainSnow();
	int			m_SnowBallNum = 0;
	float		m_snow = 0.0f;		//雪玉のリロード
	float		m_gather = -3.0f;
	bool        m_CanShoot;			//発射可能かどうか
	bool        m_CanDrain=false;			//吸収可能かどうか

	std::shared_ptr<KdTexture> m_spReLoadTex;
	Matrix					   m_ReLoadMat;

	//壁//////////////////////////////////////////////////////////////////////////////////////
	void MakeWall();
	float		m_canWall = 1;
	bool		m_makeWall;

	//HP//////////////////////////////////////////////////////////////////////////////////////
	float		m_hp = 100;

	int         m_hitCntOne = 0;
	int         m_hitCntTen = 0;

	void Draw2D();
	std::shared_ptr<KdTexture> m_spHpTex;		//HPの内側テクスチャ(増減するもの)
	std::shared_ptr<KdTexture> m_spHpBerTex;	//HPの外側のテクスチャ
	Matrix					   m_HpMat;			//描画位置
	int						   m_hpScroll = 0;	//描画位置を合わせる用(位置の調整)	

	std::shared_ptr<KdTexture> m_spReticleTex;		//十字のレティクル
	Matrix					   m_ReticleMat;		//描画位置
	float					   m_Scale = 0.6;
	
	
	bool m_notMove = false;
	bool m_canmGather =false;//壁を作った時やリロードの間動かなくする用

	KdAnimator m_animator;

	static const float s_allowToStepHeight;		//歩いて乗り越えられる段差の高さ
	static const float s_landingHeight;			//地面から足が離れていても着地しているとする高さ(坂道などを下るときに宙に浮くのを避ける)
	
	int frame = 0;

	int m_crystal = 0;

	Matrix m_CamMat;


};
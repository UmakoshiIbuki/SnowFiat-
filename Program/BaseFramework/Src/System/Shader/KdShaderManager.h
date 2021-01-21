﻿#pragma once

#include "EffectShader/KdEffectShader.h"
#include "StandardShader/KdStandardShader.h"
#include "SpriteShader/KdSpriteShader.h"
#include "ModelShader/KdModelShader.h"
#include "KdPostProcessShader/KdPostProcessShader.h"
#include "GenerateShadowMapShader/KdGenerateShadowMapShader.h"

//==========================================================
//
// シェーダ関係をまとめたクラス
//
//==========================================================
class KdShaderManager
{
public:

	//==============================================================
	//
	// 初期化・解放
	//
	//==============================================================

	// 初期化
	void Init();

	// 解放
	void Release();

	//==========================
	//
	// シェーダ
	//
	//==========================
	KdStandardShader		m_standardShader;		// 3Dモデル描画シェーダ
	KdEffectShader			m_effectShader;			// エフェクト描画シェーダ
	KdSpriteShader			m_spriteShader;			// 2Dテクスチャ描画シェーダ
	KdModelShader			m_modelShader;			//モデルシェーダー
	KdPostProcessShader		m_postProcessShader;
	KdGenerateShadowMapShader	m_genShadowMapShader;	//シャドウマップシェーダー

	ID3D11SamplerState* m_ss_Anisotropic_Wrap = nullptr;
	ID3D11SamplerState* m_ss_Anisotropic_Clamp = nullptr;
	ID3D11SamplerState* m_ss_Linear_Clamp = nullptr;

	//==========================
	//
	// カメラ関係
	//
	//==========================

	// カメラ定数バッファ用構造体
	struct cbCamera
	{
		Math::Matrix		mV;			// ビュー行列
		Math::Matrix		mP;			// 射影行列
		Math::Vector3		CamPos;		// カメラワールド座標

		int					DistanceFogEnable = 0;			// 距離フォグ有効
		Math::Vector3		DistanceFogColor = { 1, 1, 1 };	// 距離フォグ色
		float				DistanceFogDensity = 0.001f;	// 距離フォグ減衰率
	};

	// カメラ定数バッファ
	KdConstantBuffer<cbCamera>	m_cb7_Camera;


	//==========================
	//
	// ライト関係
	//
	//==========================

	// ライト定数バッファ用構造体
	struct cbLight
	{
		// 環境光
		Math::Vector3		AmbientLight = { 0.5f, 0.5f, 0.5f };
		float					tmp;

		// 平行光
		Math::Vector3		DL_Dir = { 0.5f,-1,-0.09 };    // 光の方向
		float					tmp2;
		Math::Vector3		DL_Color = { 5,5,5 };  // 光の色
		float					tmp3;
		Matrix            DL_mLightVP;        // ライトカメラのビュー行列*射影行列

		//-----------------
		// 点光
		//-----------------
	// 使用数
		int                PL_Cnt = 0;
		float            tmp4[3];

		// データ
		struct PointLight
		{
			Vec3    Color;            // 色
			float    Radius;            // 半径
			Vec3    Pos;            // 座標
			float    tmp;
		};
		std::array<PointLight, 100>        PL;

	};

	// 点光を追加
	void AddPointLight(const Vec3& pos, float radius, const Vec3& color)
	{
		int idx = m_cb8_Light.GetWork().PL_Cnt;
		if (idx < (int)m_cb8_Light.GetWork().PL.size())
		{
			m_cb8_Light.Work().PL[idx].Pos = pos;
			m_cb8_Light.Work().PL[idx].Radius = radius;
			m_cb8_Light.Work().PL[idx].Color = color;

			m_cb8_Light.Work().PL_Cnt++;
		}
	}
	// 点光をリセット
	void ResetPointLight()
	{
		m_cb8_Light.Work().PL_Cnt = 0;
	}


	// ライト定数バッファ
	KdConstantBuffer<cbLight>		m_cb8_Light;

	//==========================
	//
	// パイプラインステート
	//
	//==========================

	//深度ステンシル
	ID3D11DepthStencilState* m_ds_ZEnable_ZWriteEnable = nullptr;
	ID3D11DepthStencilState* m_ds_ZDisable_ZWriteDisable = nullptr;

	ID3D11DepthStencilState* m_ds_ZEble_ZWriteDisable = nullptr;

	//ラスタライズステート
	ID3D11RasterizerState* m_rs_CullBack = nullptr;
	ID3D11RasterizerState* m_rs_CullNone = nullptr;

	//ブレンドステート
	ID3D11BlendState* m_bs_Alpha = nullptr;
	ID3D11BlendState* m_bs_Add = nullptr;

	//==========================
	//
	// その他
	//
	//==========================





//-------------------------------
// シングルトン
//-------------------------------
private:

	KdShaderManager() {}

public:
	static KdShaderManager& GetInstance() {
		static KdShaderManager instance;
		return instance;
	}
};

#define SHADER KdShaderManager::GetInstance()

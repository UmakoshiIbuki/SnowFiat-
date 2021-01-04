#include "../inc_KdCommon.hlsli"
#include "inc_KdModelShader.hlsli"

//================================
// 頂点シェーダ
//================================
VSOutput main(float4 pos : POSITION, // 頂点座標
    float2 uv : TEXCOORD0, // テクスチャUV座標
    float3 normal : NORMAL, // 法線ベクトル
    float4 color : COLOR
)
{
    VSOutput Out;

    // 座標変換
    Out.Pos = mul(pos, g_mW); // ローカル座標系 -> ワールド座標系へ変換
    Out.wPos = Out.Pos.xyz;	// ワールド座標を出力
    Out.Pos = mul(Out.Pos, g_mV); // ワールド座標系 -> ビュー座標系へ変換
    Out.Pos = mul(Out.Pos, g_mP); // ビュー座標系 -> 射影座標系へ変換

    // 頂点色
    Out.Color = color;

    // 法線
    Out.wN = normalize(mul(normal, (float3x3)g_mW));

    // UV座標
    Out.UV = uv;

    // 出力
    return Out;
}

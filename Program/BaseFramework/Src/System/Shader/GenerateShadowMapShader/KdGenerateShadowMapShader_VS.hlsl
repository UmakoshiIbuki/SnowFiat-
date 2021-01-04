#include "../inc_KdCommon.hlsli"

#include "inc_KdGenerateShadowMapShader.hlsli"

// ���_�V�F�[�_
VSOutput main(float4 pos : POSITION,
	float2 uv : TEXCOORD0)
{
	VSOutput Out;

	// ���C�g�J�����Ŏˉe�ϊ�
	Out.Pos = mul(pos, g_mW);
	Out.Pos = mul(Out.Pos, g_mLightVP);

	// �ˉe���W
	Out.wvpPos = Out.Pos;
	// UV���W
	Out.UV = uv;

	return Out;
}

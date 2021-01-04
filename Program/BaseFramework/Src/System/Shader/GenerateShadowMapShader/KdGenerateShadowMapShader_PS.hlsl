#include "inc_KdGenerateShadowMapShader.hlsli"

Texture2D g_baseTex : register(t0);
SamplerState g_ss : register(s0);

// �s�N�Z���V�F�[�_
float4 main(VSOutput In) : SV_Target0
{
    // �A���t�@�l�����ȉ��̂��͎̂̂Ă�
    float4 texCol = g_baseTex.Sample(g_ss, In.UV);
    // �A���t�@�e�X�g
    if (texCol.a < 0.1)
    {
        discard;
    }

    // R�����Ɏˉe���W�n�ł̐[�x�l(�ŋߐڋ����F0.0 �` �ŉ��������F1.0)���o��
    // ���ˉe���W�n��xyz��w�ŏ��Z����K�v������
    return float4(In.wvpPos.z / In.wvpPos.w, 0, 0, 1);
}


//===========================================
//
// �萔�o�b�t�@
//  �Q�[���v���O���������玝���Ă���f�[�^
//
//===========================================

//------------------------------
// �萔�o�b�t�@(�}�e���A��)
//------------------------------
// �}�e���A���\����
struct Material
{
    float4 BaseColor; // �x�[�X�F
    float Metallic; // �����x
    float  Roughness; // �e��
};

cbuffer cbMaterial : register(b1)
{
    Material g_Material;
};

// �萔�o�b�t�@(�I�u�W�F�N�g�P�ʍX�V)
cbuffer cbObject : register(b0)
{
    // �I�u�W�F�N�g���
    row_major float4x4 g_mW; // ���[���h�ϊ��s��(DirectX�͍s�D��AOpenGl�Ȃǂ͗�D��)
};

// ���_�V�F�[�_����o�͂���f�[�^
struct VSOutput
{
    float4 Pos      : SV_Position;  // �ˉe���W
    float2 UV       : TEXCOORD0;    // UV���W
    float4 Color    : TEXCOORD1;    // �F
    float3 wN       : TEXCOORD2;    // ���[���h�@��
    float3 wPos     : TEXCOORD3;    // ���[���h3D���W
};

#include "../inc_KdCommon.hlsli"
#include "inc_KdModelShader.hlsli"

// �e�N�X�`���i�摜�f�[�^�����̂܂܂ł͎g���Ȃ��j
Texture2D g_baseTex : register(t0);         // �x�[�X�J���[�e�N�X�`��
Texture2D g_mrTex : register(t1);            // ���^���b�N/���t�l�X�e�N�X�`��
Texture2D g_dirShadowMap : register(t102); // ���s���V���h�E�}�b�v

// �T���v���i�e�N�X�`���̃f�[�^��������ځj
SamplerState g_ss : register(s0);

// BlinnPhong NDF
// �ElightDir    �c ���C�g�̕���
// �EvCam        �c �s�N�Z������J�����ւ̕���
// �Enormal      �c �@��
// �EspecPower   �c ���˂̉s��
float BlinnPhong(float3 lightDir, float3 vCam, float3 normal, float specPower)
{
	float3 H = normalize(-lightDir + vCam);
	float NdotH = saturate(dot(normal, H)); // �J�����̊p�x��(0�`1)
	float spec = pow(NdotH, specPower);

	// ���K��Blinn-Phong
	return spec * ((specPower + 2) / (2 * 3.1415926535));
}

//================================
// �s�N�Z���V�F�[�_
//================================
float4 main(VSOutput In) : SV_Target0
{
	// �J�����ւ̕���
	float3 vCam = g_CamPos - In.wPos;
	float camDist = length(vCam);       // �J���� - �s�N�Z������
	vCam = normalize(vCam);

	// �@�����K��
	// 2�_�Ԃ̒��_�̖@�����g���ĐV�����@��������邽�߁A�ēx������1�ɂ���K�v������
	float3 wN = normalize(In.wN);

	//------------------------------------------
	// �ގ��F
	//------------------------------------------
	float4 baseColor = g_baseTex.Sample(g_ss, In.UV) * g_Material.BaseColor * In.Color;

	float4 mr = g_mrTex.Sample(g_ss, In.UV);

	// ������
	float metallic = mr.b * g_Material.Metallic;

	// �e��
	float roughness = mr.g * g_Material.Roughness;

	// ���t�l�X����A���ˌ��p��SpecularPower�����߂�
	// Call of Duty : Black Ops��FarCry3�ł̎�@���Q�l
	// specPower = 2^(13*g)�@�O���X�lg = 0�`1��1�`8192�̃X�y�L�������x�����߂�
	// �����ł� specPower = 2^(11*g)�@1�`2048���g�p���܂��B
	// �Q�l�Fhttps://hanecci.hatenadiary.org/entry/20130511/p1
	float smoothness = 1.0 - roughness; // ���t�l�X���t�]�����u���炩�v���ɂ���
	float specPower = pow(2, 11 * smoothness); // 1�`2048

	//------------------------------------------
	// ���C�e�B���O
	//------------------------------------------
	// �ŏI�I�ȐF
	float3 color = 0;

	// �ގ��̊g�U�F�@������قǍގ��̐F�ɂȂ�A�����قǊg�U�F�͖����Ȃ�
	const float3 baseDiffuse = lerp(baseColor.rgb, float3(0, 0, 0), metallic);
	// �ގ��̔��ːF�@������قǌ��̐F�����̂܂ܔ��˂��A�����قǍގ��̐F�����
	const float3 baseSpecular = lerp(0.04, baseColor.rgb, metallic);

	//-------------------------------
	// �V���h�E�}�b�s���O(�e����)
	//-------------------------------
	float shadow = 1;
	// �s�N�Z����3D���W����A�V���h�E�}�b�v��Ԃ֕ϊ�
	float4 liPos = mul(float4(In.wPos, 1), g_DL_mLightVP);
	//���g�Ŏˉe���W�ɕϊ�����ꍇ�͉��s�ŕϊ����Ă��(���������܂�)
	liPos.xyz /= liPos.w;

	// �[�x�}�b�v�͈͓̔��H
	if (abs(liPos.x) <= 1 && abs(liPos.y) <= 1 && liPos.z <= 1)
	{
		// �ˉe���W -> UV���W�֕ϊ�
		float2 uv = liPos.xy * float2(1, -1) * 0.5 + 0.5;
		// ���C�g�J��������̋���
		float z = liPos.z - 0.002; // �V���h�E�A�N�l�΍�

		// �e����
		shadow = g_dirShadowMap.Sample(g_ss, uv).r < z ? 0 : 1;
	}
	//-------------------------------

	//-------------------------
	// ���s��
	//-------------------------

	// Diffuse(�g�U��) 
	{
		// ���̕����Ɩ@���̕����Ƃ̊p�x�������̋����ɂȂ�
		float lightDiffuse = dot(-g_DL_Dir, wN);
		lightDiffuse = saturate(lightDiffuse);  // �}�C�i�X�l��0�ɂ���@0(��)�`1(��)�ɂȂ�

		// ���K��Lambert
		lightDiffuse /= 3.1415926535;

		// ���̐F * �ގ��̊g�U�F * ������
		//color += (g_DL_Color * lightDiffuse) * baseColor.rgb * baseColor.a;
		color += (g_DL_Color * lightDiffuse) * baseDiffuse * baseColor.a * shadow;
	}

	// Specular(���ːF) 
	{
		// ���˂������̋��������߂�
		// Phong NDF
		//float3 vRef = reflect(g_DL_Dir, wN);
		//float spec = dot(vRef, vCam);
		//spec = saturate(spec);		// 0�`1�ɐ؂�l�߂�
		//spec = pow(spec, specPower);

		// Blinn-Phong NDF
		float spec = BlinnPhong(g_DL_Dir, vCam, wN, specPower);

		// ���̐F * ���ˌ��̋��� * �ގ��̔��ːF * ������ * �K���Ȓ����l
		//color += (g_DL_Color * spec) * baseColor.rgb * baseColor.a * 0.5;
		color += (g_DL_Color * spec) * baseSpecular * baseColor.a * shadow;;
	}

	//-------------------------
	// �_��
	//-------------------------
	for (int pi = 0; pi < g_PL_Cnt.x; pi++)
	{
		// �s�N�Z������_���ւ̕���
		float3 dir = g_PL[pi].Pos - In.wPos;
		// �������Z�o
		float dist = length(dir);
		// ���K��
		dir /= dist;

		// �_���̔���ȓ�
		if (dist < g_PL[pi].Radius)
		{
			// ���a�����ƂɁA�����̔䗦�����߂�
			float atte = 1.0 - saturate(dist / g_PL[pi].Radius);
			// �t�Q��̖@��
			atte *= atte;

			// Diffuse(�g�U��)
			{
				// ���̕����Ɩ@���̕����Ƃ̊p�x�������̋����ɂȂ�
				float lightDiffuse = dot(dir, wN);
				lightDiffuse = saturate(lightDiffuse); // �}�C�i�X�l��0�ɂ���@0(��)�`1(��)�ɂȂ�

				lightDiffuse *= atte; // ����

				// ���K��Lambert
				lightDiffuse /= 3.1415926535;

				// ���̐F * �ގ��̊g�U�F * ������
				color += (g_PL[pi].Color * lightDiffuse) * baseDiffuse * baseColor.a;
			}

			// Specular(���ːF)
			{
				// ���˂������̋��������߂�
				// Blinn-Phong NDF
				float spec = BlinnPhong(-dir, vCam, wN, specPower);

				spec *= atte; // ����

				// ���̐F * ���ˌ��̋��� * �ގ��̔��ːF * ������ * �K���Ȓ����l
				color += (g_PL[pi].Color * spec) * baseSpecular * baseColor.a * 0.5;
			}
		}
	}


	//------------------------------------------
	// �o��
	//------------------------------------------
	return float4(color, baseColor.a);
    //return g_baseTex.Sample(g_ss, In.UV) * g_Material.BaseColor * In.Color;
}

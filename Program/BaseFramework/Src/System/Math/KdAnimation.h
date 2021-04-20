#pragma once

//アニメーションキー(クォータニオン)
struct KdAnimKeyQuaternion
{
	float m_time = 0;
	KdQuaternion m_quat;
};

//アニメーションキー(ベクトル)
struct KdAnimKeyVector3
{
	float m_time = 0;
	Vec3  m_vec;
};


//アニメーションデータ
struct KdAnimationData
{
	//アニメーション名
	std::string		m_name;
	//アニメの長さ
	float m_maxLength;

	//1ノードのアニメーションデータ
	struct Node
	{
		void Interpolate(Matrix& rDst, float time);
		bool InterpolateTranslations(Vec3& resurt, float time);
		bool InterpolateRotations(KdQuaternion& resurt, float time);
		bool InterpolateScales(Vec3& resurt, float time);

		int m_nodeOffset = -1;

		//各チャンネル
		std::vector<KdAnimKeyVector3>		m_translations;
		std::vector<KdAnimKeyQuaternion>	m_rotations;
		std::vector<KdAnimKeyVector3>		m_scales;

	};

	std::vector<Node> m_nodes;
};

class KdAnimator
{
public:
	inline void SetAnimation(std::shared_ptr<KdAnimationData>& rData)
	{
		m_spAnimation = rData;

		m_time = 0.0f;
	}

	//アニメーションの更新
	void AdvanceTime(std::vector<KdModel::Node>& rNodes, float speed = 1.0f);

private:
	std::shared_ptr<KdAnimationData> m_spAnimation = nullptr;	//再生するアニメーションデータ
	float m_time = 0.0f;
};
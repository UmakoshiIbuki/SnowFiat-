#pragma once
#include"../Game/GameObject.h"

/////////////////////////////////////////////////
//モデルコンポーネント
////////////////////////////////////////////////

class ModelComponent
{
public:
	ModelComponent(GameObject& owner) : m_owner(owner) {}

	//有効フラグ
	bool IsEnable()const { return m_enable; }
	void SetEnable(bool enable) { m_enable = enable; }

	//モデル取得
	//inline const std::shared_ptr<KdModel>GetModel()const { return m_spModel; }
	const std::vector<KdModel::Node>& GetNodes()const { return m_coppiedNodes; }

	//メッシュ取得
	inline const std::shared_ptr<KdMesh>GetMesh(UINT index)const
	{
		if (index>= m_coppiedNodes.size()) { return nullptr; }
		return m_coppiedNodes[index].m_spMesh;
	}

	inline KdModel::Node* FindNode(const std::string& name)
	{
		/*if (!m_spModel) { return nullptr; }
		return m_spModel->FindNode(name);*/

		for (auto&& node : m_coppiedNodes)
		{
			if (node.m_name == name)
			{
				return &node;
			}
		}
		return nullptr;
	}

	const std::shared_ptr<KdAnimationData>GetAnimation(const std::string& animName)const
	{
		if (!m_spModel) { return nullptr; }
		return m_spModel->GetAnimation(animName);
	}

	std::vector<KdModel::Node>& GetChangeableNodes() { return m_coppiedNodes; }

	//モデルセット
	//inline void SetModel(const std::shared_ptr<KdModel> model) { m_spModel = model; }

	void SetModel(const std::shared_ptr<KdModel>& rModel);
	//StsndardShaderで描画
	void Draw();
	// シャドウマップ生成描画
	void DrawShadowMap();

private:

	std::vector<KdModel::Node>m_coppiedNodes;	//個別管理のためオリジナルからコピーして保持する配列

	//有効
	bool m_enable = true;

	//モデルデータの参照
	std::shared_ptr<KdModel>m_spModel;
	GameObject& m_owner;
};
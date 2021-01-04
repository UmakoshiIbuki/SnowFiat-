#pragma once
struct KdAnimationData;

class KdModel
{
public:
	KdModel();
	~KdModel();

	bool Load(const std::string& filename);
	
	//アクセサ
	const std::shared_ptr<KdMesh> GetMesh(UINT index) const 
	{
		return index < m_originalNodes.size() ? m_originalNodes[index].m_spMesh : nullptr;
	}

	const std::vector<KdMaterial>& GetMaterials()const { return m_materials; }

	//ノード：モデルを形成するメッシュを扱うための最小単位
	struct Node
	{
		std::string m_name;					//ノード名
		Matrix m_localTransform;			//変換行列(原点からどれだけ離れているか)
		std::shared_ptr<KdMesh>m_spMesh;	//メッシュ情報
	};

	//文字列を元にノードの検索
	inline Node* FindNode(const std::string& name)
	{
		for (auto&& node : m_originalNodes)
		{
			if (node.m_name == name) { return &node; }
		}
		return nullptr;
	}

	const std::vector<Node>& GetOriginalNodes()const { return m_originalNodes; }

	//アニメーションデータ取得
	const std::shared_ptr<KdAnimationData>GetAnimation(const std::string& animName)const;

private:

	void Relese();
	std::vector<Node>	m_originalNodes;	//データのノード配列

	//マテリアル配列
	std::vector<KdMaterial>m_materials;

	std::vector<std::shared_ptr<KdAnimationData>>m_spAnimations;

};
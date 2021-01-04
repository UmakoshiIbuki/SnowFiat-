#pragma once

////////////////////////////////////////////////////////////
//リソース管理クラス
//・デザインパターンのFlyweightパターンを採用
///////////////////////////////////////////////////////////

class ResourceFactory
{
public:
	//モデルデータ取得
	std::shared_ptr<KdModel> GetModel(const std::string& filename);

	//テクスチャデータ取得
	std::shared_ptr<KdTexture>GetTexture(const std::string& filename);

	//Json取得
	json11::Json GetJSON(const std::string& filename);

	//管理を破棄する
	void Clear()
	{
		m_modelMap.clear();
		m_texMap.clear();
		m_jsonMap.clear();
	}

private:

	//Json読み込み
	json11::Json LoadJSON(const std::string& filename);

	//モデルデータ管理マップ
	std::unordered_map<std::string, std::shared_ptr<KdModel>>m_modelMap;
	
	//テクスチャー管理マップ
	std::unordered_map<std::string, std::shared_ptr<KdTexture>>m_texMap;

	//Json管理マップ
	std::unordered_map < std::string, json11::Json>m_jsonMap;

private:
	//////////////////////////////////////////////////////
	//シングルトン
	/////////////////////////////////////////////////////
public:
	static ResourceFactory& GetInstance()
	{
		static ResourceFactory inctance;
		return inctance;
	}
};

#define ResFac ResourceFactory::GetInstance()
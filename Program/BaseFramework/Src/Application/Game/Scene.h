#pragma once

#include"EditorCamera.h"

//前方宣言
class GameObject;
class CameraComponent;
class Bridge;

class ImGuiLogWindow
{
public:
	void Clear() { m_Buf.clear(); }

	template<class...Args>
	void AddLog(const char* fmt, Args...args)
	{
		std::string str = fmt;
		str += "\n";
		m_Buf.appendf(str.c_str(), args...);
		m_ScrollToBotton = true;
	}

	void ImGuiUpdate(const char* title, bool* p_opened = NULL)
	{
		ImGui::Begin(title, p_opened);
		ImGui::TextUnformatted(m_Buf.begin());
		if (m_ScrollToBotton)
			ImGui::SetScrollHere(1.0f);

		m_ScrollToBotton = false;
		ImGui::End();
	}

private:
	ImGuiTextBuffer m_Buf;
	bool            m_ScrollToBotton = false;
};

class Scene
{
public:
	static Scene& GetInstance()
	{
		static Scene inctance;
		return inctance;
	}

	//デストラクタ
	~Scene();

	
	void Init();         //初期化関数
	void Deserialize();
	void Release();      //解放関数 
	void Update();       //更新関数   
	void Draw();         //描画関数   

	void RequestChangeScene(const std::string& filename);

	void AddObject(std::shared_ptr<GameObject> spObject);

	inline void SteTargetCamera(std::shared_ptr<CameraComponent>spCamera) { m_wpTaegetCamera = spCamera; }

	void ImGuiUpdate();

	void ImGuiPrefabFactoryUpdate();

	//デバッグライン描画
	void AddDebugLine(const Math::Vector3& p1, const Math::Vector3& p2, const Math::Color& color = { 1,1,1,1 });

	//デバッグスフィア描画
	void AddDebugSphereLine(const Math::Vector3& pos, float radius, const Math::Color& color = { 1,1,1,1 });

	//デバッグ座標軸描画
	void AddDebugCoordinateAxisLine(const Math::Vector3& pos, float scale = 1.0f);

	//GameObjectのリストを返す
	const std::list<std::shared_ptr<GameObject>>GetObjects()const { return m_spobjects; }
	
	std::shared_ptr<GameObject> FindObjectWithName(const std::string& name);

	void SetCrystal(int crystal) { m_crystals= crystal; };
	int GetCrystal() { return m_crystals; }

	void SetHitCnt(int one, int ten) { m_hitCntOne = one, m_hitCntTen = ten; }
	int GetHitCntOne() { return m_hitCntOne; }
	int GetHitCntTen() { return m_hitCntTen; }

	void SetWhiteIn(float f) { whiteness = f; }
	float GetWhite() { return whiteness; }

private:

	std::unique_ptr<DirectX::AudioEngine>            m_audioEng = nullptr;
	std::unique_ptr<DirectX::SoundEffect>            m_soundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance>    m_instance ;

	float whiteness = 0.0f;

	std::shared_ptr<KdTexture> m_spHeightBrightTex = nullptr;

	std::shared_ptr<KdTexture> m_spTestImage = nullptr;

	std::shared_ptr<KdTexture> m_spBlurRT[2] = { nullptr, nullptr };

	KdBlurTexture m_blurTex;

	int m_hitCntOne = 0;
	int m_hitCntTen = 0;

	int m_crystals = 0;

	std::shared_ptr<KdTexture> m_spScreenRT = nullptr;
	std::shared_ptr<KdTexture> m_spScreenZ = nullptr;

	//-------------------------
	// 平行光
	//-------------------------
	Vec3 m_lightDir = { 0,-1,0 };		//方向
	Vec3 m_lightColor = { 1,1,1 };		//光の色

	std::string m_jsonPath = "";

	void LoadScene(const std::string& sneneFilename);

	std::string m_nextSceneFilename = "";	//次のシーンのJsonファイル名
	bool m_isRequestChangeScene = false;	//シーン遷移のリクエストがあったか

	void ExecChangeScene();					//シーンを実際に変更するところ
	void Reset();							//シーンをまたぐときにリセットする処理

	//コンストラクタ						
	Scene();

	std::shared_ptr< KdModel>	m_spsky=nullptr;				//スカイフィア
	EditorCamera	m_camera;
	bool			m_editorCameraEnable = false;

	bool			pauseFlag = false;

	std::list<std::shared_ptr<GameObject>> m_spobjects;

	std::weak_ptr<GameObject>m_wpImguiSelectObj;

	//ターゲットのカメラ
	std::weak_ptr<CameraComponent>m_wpTaegetCamera;

	//デバッグライン描画用の頂点配列
	std::vector<KdEffectShader::Vertex> m_debugLines;

	ImGuiLogWindow m_Editor_Log;

	SquarePolygon m_poly;
};

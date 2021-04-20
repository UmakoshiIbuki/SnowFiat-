#include"CharacterUpdate.h"
#include"Title.h"
#include"../../main.h"

CharacterUpdate::CharacterUpdate()
{
	m_spReplayTex = ResFac.GetTexture("Data/Texture/CharacterUpdate/Hp.png");
	m_spReturnTex = ResFac.GetTexture("Data/Texture/CharacterUpdate/Return.png");

	m_PoworPos.x = 500;
	m_PoworPos.y = -270;

	m_ReturnPos.x = 0;
	m_ReturnPos.y = 0;

	m_spTitle = std::make_shared<Title>();
	LoadJson();
}

CharacterUpdate::~CharacterUpdate()
{
}

void CharacterUpdate::Update()
{
	//if(m_spTitle->)
	POINT nowMousePos;
	HWND hwnd;
	hwnd = APP.m_window.GetWndHandle();

	GetCursorPos(&nowMousePos);				//マウス現在位置の取得
	ScreenToClient(hwnd, &nowMousePos);		//マウスをクライアント座標に

	nowMousePos.x -= 640;
	nowMousePos.y -= 360;

	MousePos.x = nowMousePos.x;
	MousePos.y = nowMousePos.y;

	std::string filepath = "Data/Scene/Human.json";
	json11::Json::object serial;
	{
		serial["ClassName"] = "Human";
		serial["Name"] = "PlayerHuman";
		serial["Tag"] = 3;
		serial["ModelFileName"] = "Data/Character/SnowManaAnimation.gltf";
	}

	{
		json11::Json::array pos(3);
		pos[0] = m_pos.x+test;
		pos[1] = -0.9;
		pos[2] = test;

		serial["Pos"] = pos;
	}

	{
		json11::Json::array scale(3);
		scale[0] = 0.5;
		scale[1] = 0.5;
		scale[2] = 0.5;

		serial["Scale"] = scale;
	}

	std::ofstream ofs(filepath);
	if (ofs)
	{
		json11::Json jsn = serial;
		std::string strJson = jsn.dumpDebug();

		ofs.write(strJson.c_str(), strJson.size());
	}
}

void CharacterUpdate::Draw2D()
{
	m_PoworMat.SetTranslation(m_PoworPos);
	SHADER.m_spriteShader.SetMatrix(m_PoworMat);
	SHADER.m_spriteShader.DrawTex(m_spReplayTex.get(), 0, 0);

	m_ReturnMat.SetTranslation(m_ReturnPos);
	SHADER.m_spriteShader.SetMatrix(m_ReturnMat);
	SHADER.m_spriteShader.DrawTex(m_spReturnTex.get(), 0, 0);
}

bool CharacterUpdate::Collision()
{
	if (Collision2D(MousePos, m_PoworPos, 200, 100))
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			test = 1;		

			return false;
		}
	}

	if (Collision2D(m_ReturnPos, m_ReturnPos, 100, 100))
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			return true;
		}
	}

	return false;

}

void CharacterUpdate::LoadJson()
{
	std::ifstream ifs("Data/Scene/Human.json");
	if (ifs.fail()) { assert(0 && "jsonパスが違う"); };

	std::string strjson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	std::string err;
	json11::Json jsonobj = json11::Json::parse(strjson, err);
	if (err.size()>0) { assert(0 && "json変換失敗"); };

	std::vector<json11::Json>& rPos = jsonobj["Pos"].array_items();
	m_pos.x = (float)rPos[0].number_value();

}

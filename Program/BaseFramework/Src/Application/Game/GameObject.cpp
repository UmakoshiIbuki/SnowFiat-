#include"GameObject.h"
#include"../Component/CameraComponent.h"
#include"../Component/InputComponent.h"
#include"../Component/ModelComponent.h"
#include"Action/ActionGameProcess.h"
#include"Action/Human.h"
#include"Action/Enemy.h"
#include"Action/Wall.h"
#include"Action/Lift.h"
#include"Action/SnowManager.h"
#include"Action/Lamp.h"
#include"Action/Bench.h"
#include"../main.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
	Release();
}

void GameObject::Deserialize(const json11::Json& jsonObj)
{
	if (jsonObj.is_null()) { return; }

	if (jsonObj["Name"].is_null() == false)
	{
		m_name = jsonObj["Name"].string_value();
	}

	if (jsonObj["Tag"].is_null() == false)
	{
		m_tag=jsonObj["Tag"].int_value();
	}

	if (jsonObj["Speed"].is_null() == false)
	{
		m_Movespeed = jsonObj["Speed"].number_value();
	}

	//モデル////////////////////////////////////////////////////////
	if (jsonObj.is_null() == false)
	{
		if (m_spCameraComponent)
		{
			m_spModelComponent->SetModel(ResFac.GetModel(jsonObj["ModelFileName"].string_value()));
		}
	}

	//行列//////////////////////////////////////////////////////////
	Matrix mTrans,mRotate, mScale;

	//座標
	const std::vector<json11::Json>& rPos = jsonObj["Pos"].array_items();
	if (rPos.size() == 3)
	{
		mTrans.CreateTranslation((float)rPos[0].number_value(), (float)rPos[1].number_value(), (float)rPos[2].number_value());
	}

	//回転
	const std::vector<json11::Json>& rRot = jsonObj["Rot"].array_items();
	if (rRot.size() == 3)
	{
		mRotate.CreateRotationX((float)rRot[0].number_value()*ToRadians);
		mRotate.RotateY((float)rRot[1].number_value()*ToRadians);
		mRotate.RotateZ((float)rRot[2].number_value()*ToRadians);
	}

	//拡大
	const std::vector<json11::Json>& rScale = jsonObj["Scale"].array_items();
	if (rScale.size() == 3)
	{
		mScale.CreateScalling((float)rScale[0].number_value(), (float)rScale[1].number_value(), (float)rScale[2].number_value());
		m_scale.x = (float)rScale[0].number_value();
		m_scale.y = (float)rScale[1].number_value();
		m_scale.z = (float)rScale[2].number_value();
	}

	m_mWorld =mScale*mRotate*mTrans;

	hwnd = APP.m_window.GetWndHandle();

}
void GameObject::Update() {}

void GameObject::DrawShadowMap()
{
	if (m_spModelComponent == nullptr)return;
	m_spModelComponent->DrawShadowMap();
}

void GameObject::Draw()
{
	if (m_spModelComponent == nullptr) { return; }
	m_spModelComponent->Draw();
}

void GameObject::ImguiUpdate()
{

	ImGui::InputText("Name", &m_name);
		
	if (ImGui::TreeNodeEx("TAG", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::CheckboxFlags("Character", &m_tag, TAG_Character);
		ImGui::CheckboxFlags("Player", &m_tag, TAG_Player);
		ImGui::CheckboxFlags("StageObject", &m_tag, TAG_StageObject);
		ImGui::CheckboxFlags("AttackHit", &m_tag, TAG_AttackHit);

		if (ImGui::Button(u8"JSONテキストコピー"))
		{
			ImGui::SetClipboardText(KdFormat("\"TAG\":%d", m_tag).c_str());
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Vec3 pos = m_mWorld.GetTranslation();
		Vec3 rot = m_mWorld.GetAngles() * ToDegrees;
		float scale = m_mWorld.GetAxisX().Length();

		bool isChange = false;

		isChange |= ImGui::DragFloat3("Position", &pos.x, 0.01f);
		isChange |= ImGui::DragFloat3("Rotation", &rot.x, 0.1f);
		isChange |= ImGui::DragFloat("Scale", &scale, 0.1f);

		if (isChange)
		{
			//計算する時はradianに戻す
			rot *= ToRadians;

			Matrix mR;
			mR.RotateX(rot.x);
			mR.RotateY(rot.y);
			mR.RotateZ(rot.z);

			m_mWorld = mR;

			m_mWorld.SetTranslation(pos);

			m_mWorld.Scale(scale, scale, scale);
		}

		if (ImGui::Button(u8"JSONテキストコピー"))
		{
			std::string s = KdFormat("\"Pos\":[%.1f,%.1f,%.1f],\n", pos.x, pos.y, pos.z);
			s+= KdFormat("\"Rot\":[%.1f,%.1f,%.1f],\n", rot.x, rot.y, rot.z);
			ImGui::SetClipboardText(s.c_str());
		}

		//ディゾルブ閾値
		if (ImGui::DragFloat("Dissolve", &m_dissolveThreshold, 0.001, 0.0f, 1.0f))
		{
			this->GetModelComponent()->SetDissolveThreshold(m_dissolveThreshold);
		}

		ImGui::TreePop();
	}
	
}

//球による当たり判定(距離判定)
bool GameObject::HitCheckBySphere(const SphereInfo& rInfo)
{

	//当たったとする距離の計算
	float hitRange = rInfo.m_radius + m_colRadius;

	//自分の座標ベクトル
	Vec3 myPos = m_mWorld.GetTranslation();

	//二点間のベクトルを計算
	Vec3 betweenVec = rInfo.m_pos- myPos ;

	//二点間の距離を計算
	float distance = betweenVec.Length();

	bool isHit = false;
	if (distance <= hitRange)
	{
		isHit = true;
	}

	return isHit;
}

bool GameObject::HitCheckByRay(const RayInfo& rInfo,KdRayResult& rResult)
{
	//判定する対象のモデルがない場合は当たってないとして帰る
	if (!m_spModelComponent) { return false; }

	//if (m_spModelComponent->GetMesh() == nullptr) { return false; }
	//return KdRayToMesh(rInfo.m_pos, rInfo.m_dir, rInfo.m_maxRange, *(m_spModelComponent->GetMesh()), m_mWorld,rResult);

	//全てのノード(メッシュ)分当たり判定を行う
	for (auto& node : m_spModelComponent->GetNodes())
	{
		KdRayResult tmpResult;//結果返送用

		//レイ判定(本体からのずれ分も加味して計算)
		RayToMesh(rInfo.m_pos, rInfo.m_dir, rInfo.m_maxRange, *(node.m_spMesh), node.m_localTransform * m_mWorld, tmpResult);
		
		//より近い判定を優先する
		if (tmpResult.m_distance < rResult.m_distance)
		{
			rResult = tmpResult;
		}
	}
	return rResult.m_hit;
}

bool GameObject::HitCheckBySphereVsMesh(const SphereInfo& rInfo, SphereResult& rRsult)
{
	if (!m_spModelComponent) { return false; }

	//全てのノードのメッシュから押し返された位置を格納する座標ベクトル
	Vec3 pushedFromNodesPos = rInfo.m_pos;

	//全てのノード判定
	for (auto& node : m_spModelComponent->GetNodes())
	{
		//このノードがモデルを持っていなかった場合無視
		if (!node.m_spMesh) { continue; }

		//点とノードの判定
		if (KdSphereToMesh(pushedFromNodesPos, rInfo.m_radius, *node.m_spMesh, node.m_localTransform * m_mWorld, pushedFromNodesPos))
		{
			rRsult.m_hit = true;
		}
	}

	if (rRsult.m_hit)
	{
		//押し戻された球と前の位置から、押し戻すベクトルを計算する
		rRsult.m_push = pushedFromNodesPos - rInfo.m_pos;
	}
	return rRsult.m_hit;
}

float GameObject::easeInOut(float progress)
{
	return -(cos(M_PI * progress) - 1) * 0.5f;
}

float GameObject::easeOutBack(float progress)
{
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	return 1 + c3 * pow(progress - 1, 3) + c1 * pow(progress - 1, 2);
}

float GameObject::easeInBack(float progress)
{
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	return c3 * progress * progress * progress - c1 * progress * progress;
}

float easeInOutBack(float progress)
{
const float c1 = 1.70158;
const float c2 = c1 * 1.525;

return progress < 0.5
  ? (pow(2 * progress, 2) * ((c2 + 1) * 2 * progress - c2)) / 2
  : (pow(2 * progress - 2, 2) * ((c2 + 1) * (progress * 2 - 2) + c2) + 2) / 2;
}

float GameObject::easeOutQuart(float progress)
{
	return 1 - pow(1 - progress, 4);
}

void GameObject::Release()
{

}
#include"Action/Result.h"
#include"Action/Title.h"
#include"Action/StageSelect.h"
#include"Particle.h"
#include"Action/GameOver.h"
#include"Action/Crystal.h"
#include"Action/Tank.h"
#include"Action/Wall.h"
#include"Action/RollingBall.h"
std::shared_ptr<GameObject>CreateGameObject(const std::string& name)
{
	if (name == "GameObject") {
		return std::make_shared<GameObject>();
	}

	if (name == "Wall") {
		return std::make_shared<Wall>();
	}

	if (name == "RollingBall") {
		return std::make_shared<RollingBall>();
	}

	if (name == "Human") {
		return std::make_shared<Human>();
	}

	if (name == "Enemy") {
		return std::make_shared<Enemy>();
	}

	if (name == "Crystal") {
		return std::make_shared<Crystal>();
	}

	if (name == "Lift") {
		return std::make_shared<Lift>();
	}

	if (name == "Lamp") {
		return std::make_shared<Lamp>();
	}

	if (name == "Bench") {
		return std::make_shared<Bench>();
	}

	if (name == "ActionGameProcess") {
		return std::make_shared<ActionGameProcess>();
	}

	if (name == "Result")
	{
		return std::make_shared<Result>();
	}

	if (name == "GameOver")
	{
		return std::make_shared<GameOver>();
	}

	if (name == "Tank")
	{
		return std::make_shared<Tank>();
	}

	if (name == "Title") {
		return std::make_shared<Title>();
	}

	if (name == "StageSelect")
	{
		return std::make_shared<StageSelect>();
	}

	//文字列が既存のクラスに一致しなかった
	assert(0 && "存在しないGameObjectクラスです！");

	return nullptr;
}
﻿#include "TrailPolygon.h"

void TrailPolygon::Draw(float width)
{
	//ポイントが2つ以下の場合は描画不可
	if (m_pointList.size() < 2) { return; }

	//軌跡座標の分割数
	float sliceCount = (float)(m_pointList.size() - 1);

	//頂点配列
	std::vector<Vertex> vertex;

	//ポイント数分サイズ確保
	vertex.resize(m_pointList.size() * 2);

	for (UINT i = 0; i < m_pointList.size(); i++)
	{
		//登録の参照(ショートカット)
		Math::Matrix& mat = m_pointList[i];

		//2つの頂点の参照(ショートカット)
		Vertex& v1 = vertex[i * 2];
		Vertex& v2 = vertex[i * 2 + 1];

		//x方向
		Math::Vector3 axisX = mat.Right();
		axisX.Normalize();

		//座標
		v1.Pos = mat.Translation() + axisX * width * 0.5f;
		v2.Pos = mat.Translation() - axisX * width * 0.5f;

		//UV
		float uvY = i / sliceCount;
		v1.UV = { 0,uvY };
		v2.UV = { 1,uvY };
	}

	//テクスチャセット
	if (m_texture) {
		D3D.GetDevContext()->PSSetShaderResources(0, 1, m_texture->GetSRViewAddress());
	}
	else {
		D3D.GetDevContext()->PSSetShaderResources(0, 1, D3D.GetWhiteTex()->GetSRViewAddress());
	}

	//指定した頂点配列を描画する関数
	D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, vertex.size(), &vertex[0], sizeof(Vertex));

}

void TrailPolygon::DrawBillboard(float width)
{
	//ポイントが2つ以下の場合は描画不可
	if (m_pointList.size() < 2) { return; }

	//カメラの情報
	Math::Matrix mCam = SHADER.m_cb7_Camera.GetWork().mV.Invert();

	//軌跡座標の分割数
	float sliceCount = (float)(m_pointList.size() - 1);

	//頂点配列
	std::vector<Vertex> vertex;

	//ポイント数分サイズ確保
	vertex.resize(m_pointList.size() * 2);

	Math::Vector3 prevPos;
	for (UINT i = 0; i < m_pointList.size(); i++)
	{
		//登録の参照(ショートカット)
		Math::Matrix& mat = m_pointList[i];

		//2つの頂点の参照(ショートカット)
		Vertex& v1 = vertex[i * 2];
		Vertex& v2 = vertex[i * 2 + 1];

		//ラインの向き
		Math::Vector3 vDir;
		if (i == 0)
		{
			//初回時のみ、次のポイントを使用
			vDir = m_pointList[1].Translation() - mat.Translation();
		}
		else {
			//二回目以降は、前回の座標のから向きを決定する
			vDir = mat.Translation() - prevPos;
		}

		//カメラからポイントへの向き
		Math::Vector3 v = mat.Translation() - mCam.Translation();
		Math::Vector3 axisX = DirectX::XMVector3Cross(vDir, v);
		axisX.Normalize();

		//座標
		v1.Pos = mat.Translation() + axisX * width * 0.5f;
		v2.Pos = mat.Translation() - axisX * width * 0.5f;

		//UV
		float uvY = i / sliceCount;
		v1.UV = { 0,uvY };
		v2.UV = { 1,uvY };

		//座標を記録しておく
		prevPos = mat.Translation();

	}

	//テクスチャセット
	if (m_texture) {
		D3D.GetDevContext()->PSSetShaderResources(0, 1, m_texture->GetSRViewAddress());
	}
	else {
		D3D.GetDevContext()->PSSetShaderResources(0, 1, D3D.GetWhiteTex()->GetSRViewAddress());
	}

	//指定した頂点配列を描画する関数
	D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, vertex.size(), &vertex[0], sizeof(Vertex));
}

//頂点情報をそのまま繋げてポリゴンを描画
void TrailPolygon::DrawSstrip()
{
	UINT pointListSize = m_pointList.size();
	if (pointListSize < 4) { return; }

	//頂点配列
	std::vector<Vertex>vertex;
	vertex.resize(pointListSize);

	//軌跡画像の分割数
	float sliceNum = pointListSize * 0.5;

	//頂点データ作成
	for (UINT i = 0; i < pointListSize; i++)
	{
		Vertex& rVertex = vertex[i];

		//頂点座標
		rVertex.Pos = m_pointList[i].Translation();

		//UV
		//X座標は左右の頂点として画像の両端0.0と1.0を往復する
		rVertex.UV.x = (float)(i % 2);
		rVertex.UV.y = (i * 0.5f)/sliceNum;
	}

	//テクスチャセット
	if (m_texture) {
		D3D.GetDevContext()->PSSetShaderResources(0, 1, m_texture->GetSRViewAddress());
	}
	else {
		D3D.GetDevContext()->PSSetShaderResources(0, 1, D3D.GetWhiteTex()->GetSRViewAddress());
	}

	//指定した頂点配列を描画する関数
	D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, vertex.size(), &vertex[0], sizeof(Vertex));
}

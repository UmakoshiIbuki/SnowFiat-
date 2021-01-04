#pragma once

//レイ判定をしたときの結果
struct KdRayResult
{
	float m_distance = FLT_MAX;	//当たった時の距離
	bool  m_hit = false;	//当たったかどうか
	Vec3 rHitPos = {};
};

//レイによる当たり判定
bool RayToMesh(
	const DirectX::XMVECTOR& rRayPos, 
	const DirectX::XMVECTOR& rRayDir, 
	float maxDistance, 
	const KdMesh& rMash, 
	const Matrix& rMatrix,
	KdRayResult& rResult
	);

//球対メッシュの当たり判定
bool KdSphereToMesh(
	const Math::Vector3& rSpherePos,	//球の中心点の場所
	float radius,						//球の半径
	const KdMesh& mesh,					//判定するメッシュ情報
	const DirectX::XMMATRIX& matrix,	//判定する相手の行列
	Math::Vector3& rPushedPos			//当たっていた場合、押し返された球の中心点
);

//点vs三角形との最近接点を求める
//・p　…点の座標
//・a　…三角形の点1
//・b　…三角形の点2
//・c　…三角形の点3
//・outPt	…最近接点の座標が入る
void KdPointToTriangle(const DirectX::XMVECTOR& p, const DirectX::XMVECTOR& a,
	const DirectX::XMVECTOR& b, const DirectX::XMVECTOR& c, DirectX::XMVECTOR& outPt);
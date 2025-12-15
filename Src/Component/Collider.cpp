#include "Collider.h"

/*
*   @brief	コンストラクタ
* 　@param[in]	GameObject* _pObj	当たり判定をつけるオブジェクト
*/
Collider::Collider(GameObject* _pObj)
	: isEnable(true)
	, pGameObject(_pObj)
	, matrix(MGetIdent()) {
}

/*
*   @brief	デストラクタ
*/
Collider::~Collider() {
}


/*
*	@brief	コンストラクタ
*	@param[in]	GameObject* _pObj	当たり判定をつけるオブジェクト
*	@param[in]  VECTOR min			最小の点
*	@param[in]  VECTOR max			最大の点
*/
BoxCollider::BoxCollider(GameObject* _pObj, VECTOR _min, VECTOR _max)
	: Collider(_pObj)
	, originMinPoint(_min)
	, originMaxPoint(_max)
	, originVertex()
	, localVertex()
	, worldVertex() {
	// 無回転の状態での各頂点データを初期化
	originVertex[0] = originMinPoint;
	originVertex[1] = VAdd(originMinPoint, VGet(originMaxPoint.x - originMinPoint.x, 0, 0));
	originVertex[2] = VAdd(originMinPoint, VGet(0, 0, originMaxPoint.z - originMinPoint.z));
	originVertex[3] = VAdd(originMinPoint, VGet(0, originMaxPoint.y - originMinPoint.y, 0));
	originVertex[4] = VAdd(originMaxPoint, VGet(originMinPoint.x - originMaxPoint.x, 0, 0));
	originVertex[5] = VAdd(originMaxPoint, VGet(0, 0, originMinPoint.z - originMaxPoint.z));
	originVertex[6] = VAdd(originMaxPoint, VGet(0, originMinPoint.y - originMaxPoint.y, 0));
	originVertex[7] = originMaxPoint;
}

/*
*   @function	Update
*   @brief		更新処理
*/
void BoxCollider::Update() {
	//  無効であれば処理しない
	if (!isEnable)
		return;

	//  y軸中心の回転行列を取得する
	MATRIX rotY = MGetRotY(Deg2Rad(pGameObject->GetRotation().y));
	//  原点からの平行移動行列を取得する
	MATRIX trans = MGetTranslate(pGameObject->GetPosition());

	//  各頂点分
	for (int i = 0; i < 8; i++) {
		//  各頂点のローカル座標を求める
		//  VECTOR * MATRIX -> 回転後のベクトル
		localVertex[i] = VTransform(originVertex[i], rotY);

		//　各頂点のワールド座標を求める
		worldVertex[i] = VTransform(localVertex[i], trans);
	}
}

/*
*   @function	Render
*   @berief		描画処理
*/
void BoxCollider::Render() {
	//  無効であれば処理しない
	if (!isEnable)
		return;

#if _DEBUG
	//  線分の描画
	DrawLine3D(worldVertex[0], worldVertex[1], green);
	DrawLine3D(worldVertex[0], worldVertex[2], green);
	DrawLine3D(worldVertex[0], worldVertex[3], green);
	DrawLine3D(worldVertex[7], worldVertex[4], green);
	DrawLine3D(worldVertex[7], worldVertex[5], green);
	DrawLine3D(worldVertex[7], worldVertex[6], green);

	DrawLine3D(worldVertex[3], worldVertex[4], green);
	DrawLine3D(worldVertex[3], worldVertex[5], green);
	DrawLine3D(worldVertex[6], worldVertex[1], green);
	DrawLine3D(worldVertex[6], worldVertex[2], green);
	DrawLine3D(worldVertex[1], worldVertex[5], green);
	DrawLine3D(worldVertex[2], worldVertex[4], green);
#endif
}

/*
*   @brief		コンストラクタ
*   @param[in]	GameObject			当たり判定をつけるオブジェクト
* 　@param[in]　VECTOR _offset		位置調整
*   @param[in]  float _radius		半径
*/
SphereCollider::SphereCollider(GameObject* _pObj, VECTOR _offset, float _radius)
	: Collider(_pObj)
	, localCenter(_offset)
	, worldCenter(VZero)
	, radius(_radius) {
}

/*
*   @function	Update
*   @brief		更新処理
*/
void SphereCollider::Update() {
	//  無効であれば処理しない
	if (!isEnable)
		return;

	//  ワールド座標の更新
	worldCenter = VAdd(pGameObject->GetPosition(), localCenter);
}

/*
*   @function	Render
*   @berief		描画処理
*/
void SphereCollider::Render() {
	//  無効であれば処理しない
	if (!isEnable)
		return;

#if _DEBUG
	//  玉の描画
	DrawSphere3D(worldCenter, radius, 16, green, black, FALSE);
#endif
}

/*
*   @brief		コンストラクタv
*   @param[in]	GameObject* _pObj		当たり判定をつけるオブジェクト
* 　@param[in]　VECTOR _p1				線分を構成する点1
* 　@param[in]　VECTOR _p2				線分を構成する点2
*   @param[in]  float _radius			半径
*/
CapsuleCollider::CapsuleCollider(GameObject* _pObj, VECTOR _p1, VECTOR _p2, float _radius)
	: Collider(_pObj)
	, originPoint1(_p1)
	, originPoint2(_p2)
	, radius(_radius)
	, offset(VScale(VUp, _radius))
	, localPoint1(VZero)
	, localPoint2(VZero)
	, worldPoint1(VZero)
	, worldPoint2(VZero) {
}

/*
*   @function	Update
*   @brief		更新処理
*/
void CapsuleCollider::Update() {
	//  無効であれば処理しない
	if (!isEnable)
		return;

	//  回転行列を取得する
	//MATRIX rotY = MGetRotY(Deg2Rad(pGameObject->GetRotation().y));
	MATRIX rotXYZ = MGetRotElem(matrix);

	//  原点からの平行移動行列を取得する
	//MATRIX trans = MGetTranslate(pGameObject->GetPosition());
	VECTOR trans = MGetTranslateElem(matrix);

	//  線分のローカル座標を求める
	//  VECTOR * MATRIX -> 回転後のベクトル
	localPoint1 = VTransform(originPoint1, rotXYZ);
	localPoint2 = VTransform(originPoint2, rotXYZ);
	//　各頂点のワールド座標を求める
	worldPoint1 = VAdd(localPoint1, trans);
	worldPoint2 = VAdd(localPoint2, trans);

}

/*
*   @function	Render
*   @berief		描画処理
*/
void CapsuleCollider::Render() {
	//  無効であれば処理しない
	if (!isEnable)
		return;

#if _DEBUG
	//  カプセルを描画
	DrawCapsule3D(worldPoint1, worldPoint2, radius, 16, green, black, FALSE);
#endif
}

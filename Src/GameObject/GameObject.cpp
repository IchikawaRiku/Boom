#include "GameObject.h"
#include"../Component/Collider.h"

/*
*   @brief		コンストラクタ
*   @param[in]	VECTOR _pos = V_Zero	初期化する座標
*/
GameObject::GameObject(VECTOR _pos, std::string _tag)
	: modelHandle(INVALID)
	, isVisible(true)
	, speed(0)
	, position(_pos)
	, rotation(VZero)
	, scale(VOne)
	, matrix(MGetIdent())
	, pCollider(nullptr)
	, tag(_tag)
	, forward(VZero)
	, right(VZero)
	, up(VZero) {

}

/*
*   @brief		デストラクタ
*   @tips		基底クラスなので仮想関数で実装する
*/
GameObject::~GameObject() {
	if (pCollider != nullptr)
		delete pCollider;
	//  モデルハンドルの解放
	MV1DeleteModel(modelHandle);
}

/*
*   @function	Update
*   @brief		更新処理
* 　@tips		純粋仮想関数で実装する
*/
void GameObject::Update() {
	//  非表示なら更新しない
	if (!isVisible)
		return;

	// 座標、回転、拡縮から行列を求める
	MATRIX mRotX = MGetRotX(Deg2Rad(rotation.x));		//  X軸回転行列
	MATRIX mRotY = MGetRotY(Deg2Rad(rotation.y));		//  Y軸回転行列
	MATRIX mRotZ = MGetRotZ(Deg2Rad(rotation.z));		//  Z軸回転行列

	//  X->Y->Z の順で回転行列を作成する
	MATRIX mRotXYZ = MMult(MMult(mRotX, mRotY), mRotZ);

	//  拡縮行列を取得する
	MATRIX mScale = MGetScale(scale);

	//  平行移動行列を取得する
	MATRIX mTranslate = MGetTranslate(position);

	//  行列の乗算は合成
	//  回転行列 -> 拡縮行列 -> 平行移動行列 の順に掛け合わせる
	matrix = MMult(MMult(mRotXYZ, mScale), mTranslate);


	VECTOR tmp = VTransform(VForward, mRotXYZ);
	if (VSquareSize(tmp) >= 0.01f)
		forward = VNorm(tmp);

	tmp = VTransform(VRight, mRotXYZ);
	if (VSquareSize(tmp) >= 0.01f)
		right = VNorm(tmp);
	
	tmp = VTransform(VUp, mRotXYZ);
	if (VSquareSize(tmp) >= 0.01f)
		up = VNorm(tmp);
	
	//  当たり判定が伴う場合 matrix の情報を与える
	if (pCollider != nullptr)
		pCollider->SetMatrix(matrix);
}

/*
*   @function	OnTriggerEnter
*	@brief		当たった瞬間
*	@param[in]  Collider* _pOther	当たった相手
*/
void GameObject::OnTriggerEnter(Collider* _pOther)
{
}

/*
*   @function	OnTriggerStay
*	@brief		当たっている間
*	@param[in]  Collider* _pOther	当たった相手
*/
void GameObject::OnTriggerStay(Collider* _pOther)
{
}

/*
*   @function	OnTriggerExit
*	@brief		離れた瞬間
*	@param[in]  Collider* _pOther	当たった相手
*/
void GameObject::OnTriggerExit(Collider* _pOther)
{
}


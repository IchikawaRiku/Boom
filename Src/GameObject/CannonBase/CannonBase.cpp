#include "CannonBase.h"
#include "../../Manager/BulletManager.h"

/*
*   @brief	コンストラクタ
*/
CannonBase::CannonBase(VECTOR _pos)
	: GameObject(_pos) {
	scale = VScale(scale, 1.7f);
}

/*
*   @brief	デストラクタ
*/
CannonBase::~CannonBase() {

}

/*
*   @function	Start
*   @brief		初期化処理
*/
void CannonBase::Start() {

}

/*
*   @function	Update
*   @brief		更新処理
*/
void CannonBase::Update() {
	//  弾管理クラスの取得
	BulletManager* BMana = BulletManager::GetInstance();

	//  角度を弾の方向に合わせる
	if (BMana->GetMouseButton()) {
		rotation = VGet(0,(BMana->GetDownMouseX() - BMana->GetMouseX()) / 10, 0);
	}
	//  弾を放出した後元の角度に戻る
	if (!BMana->GetRelease()) {
		rotation = VSub(rotation, VScale(rotation, 0.1f));
	}
	
	//  計算した座標、回転(オイラー角)、拡縮をモデルに反映する
	MV1SetPosition(modelHandle, position);
	MV1SetRotationXYZ(modelHandle, VScale(rotation, DX_PI_F / 180.0f));
	MV1SetScale(modelHandle, scale);
}

/*
*   @function	Render
*   @brief		描画処理
*/
void CannonBase::Render() {
	//  モデルの描画
	MV1DrawModel(modelHandle);
}

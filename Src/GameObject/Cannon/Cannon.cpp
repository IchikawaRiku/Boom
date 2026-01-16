#include "Cannon.h"
#include "../../Manager/BulletManager.h"
#include "../../Manager/EffectManager.h"
#include "../../Manager/AudioManager.h"
#include "../../Manager/InputManager.h"

/*
*   @brief	コンストラクタ
*/
Cannon::Cannon(VECTOR _pos)
	: GameObject(_pos)
	, EffectFlag(false)
	, ReleaseFlag(false) 
	, CANNON_MUZZLE_LENGTH(150){
	scale = VScale(scale, 0.85f);
}

/*
*   @brief	デストラクタ
*/
Cannon::~Cannon() {

}

/*
*   @function	Start
*   @brief		初期化処理
*/
void Cannon::Start() {

}

/*
*   @function	Update
*   @brief		更新処理
*/
void Cannon::Update() {
	//  弾管理クラスの取得
	BulletManager* BMana = BulletManager::GetInstance();
	InputManager* input = InputManager::GetInstance();

	//  角度を弾の方向に合わせる
	if (!BMana->GetRelease() && input->IsMouseButton()) {
		rotation = VGet((input->GetMouseMoveValueY()) / MOUSE_SENSITIVITY,
			(input->GetMouseMoveValueX()) / MOUSE_SENSITIVITY, 0);
	}
	//  弾を放出した後元の角度に戻る
	if (!BMana->GetRelease()) {
		rotation = VSub(rotation, VScale(rotation, CANNON_RETURN_MOVE_SPEED));
		ReleaseFlag = false;
	}

	//  計算した座標、回転(オイラー角)、拡縮をモデルに反映する
	MV1SetPosition(modelHandle, position);
	MV1SetRotationXYZ(modelHandle, VScale(rotation, DX_PI_F / 180.0f));
	MV1SetScale(modelHandle, scale);

	GameObject::Update();

	//  弾が出たら大砲の先にエフェクト再生ついでに音も
	if (!EffectFlag) {
		if (!ReleaseFlag) {
			if (BMana->GetRelease()) {
				EffectFlag = true;
				ReleaseFlag = true;
			}
		}
	}
	if (EffectFlag) {
		EffectManager::GetInstance()->Instantiate(SHOT_NAME, VScale(forward, CANNON_MUZZLE_LENGTH), rotation);
		AudioManager::GetInstance()->PlayOneShot(SHOT_NAME, 1);
		EffectFlag = false;
	}
}

/*
*   @function	Render
*   @brief		描画処理
*/
void Cannon::Render() {
	//  モデルの描画
	MV1DrawModel(modelHandle);
}

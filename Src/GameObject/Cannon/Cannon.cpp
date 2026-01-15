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
	, ReleaseFlag(false) {
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
		rotation = VGet((input->GetMouseMoveValueY()) / 10,
			(input->GetMouseMoveValueX()) / 10, 0);
	}
	//  弾を放出した後元の角度に戻る
	if (!BMana->GetRelease()) {
		rotation = VSub(rotation, VScale(rotation, 0.1f));
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
		EffectManager::GetInstance()->Instantiate("Shot", VScale(forward, 150), rotation);
		AudioManager::GetInstance()->PlayOneShot("Shot", 1);
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

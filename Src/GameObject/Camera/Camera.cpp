#include "Camera.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/TimeManager.h"

//  静的メンバ変数の宣言
Camera* Camera::main = nullptr;

/*
*   @brief		コンストラクタ
*/
Camera::Camera(VECTOR _pos)
	: GameObject(_pos) {
	rotation.x = 5;
	speed = 10.0f;
	Start();

	//  カメラを複数インスタンス化しないのでここで初期化する
	main = this;
}

/*
	*   @brief		デストラクタ
	*/
Camera::~Camera() {

}

/*
	*   @function	Start
	*   @brief		初期化処理
	*/
void Camera::Start() {

}

/*
	*   @function	Update
	*   @brief		更新処理
	*/
void Camera::Update() {

	//  入力管理クラスの取得
	InputManager* input = InputManager::GetInstance();

	//  入力ベクトル
	VECTOR inputVec = VZero;
#if _DEBUG
	if (input->IsKey(KEY_INPUT_UP))
		inputVec = VAdd(inputVec, VDown);
	if (input->IsKey(KEY_INPUT_LEFT))
		inputVec = VAdd(inputVec, VLeft);
	if (input->IsKey(KEY_INPUT_DOWN))
		inputVec = VAdd(inputVec, VUp);
	if (input->IsKey(KEY_INPUT_RIGHT))
		inputVec = VAdd(inputVec, VRight);

	//  最終的な入力ベクトルを自身の回転(オイラー角)に加える
	rotation.y += inputVec.x;	//  y軸回転
	rotation.x += inputVec.y;	//  x軸回転

	//  入力ベクトル
	inputVec = VZero;

	if (input->IsKey(KEY_INPUT_W))
		inputVec = VAdd(inputVec, VForward);
	if (input->IsKey(KEY_INPUT_A))
		inputVec = VAdd(inputVec, VLeft);
	if (input->IsKey(KEY_INPUT_S))
		inputVec = VAdd(inputVec, VBack);
	if (input->IsKey(KEY_INPUT_D))
		inputVec = VAdd(inputVec, VRight);
	if (input->IsKey(KEY_INPUT_LSHIFT))
		inputVec = VAdd(inputVec, VDown);
	if (input->IsKey(KEY_INPUT_SPACE))
		inputVec = VAdd(inputVec, VUp);
#endif
	//  入力があれば
	if (VSquareSize(inputVec) >= 0.01f) {
		//　入力ベクトルの正規化
		inputVec = VNorm(inputVec);

		//  移動する方向ベクトル
		VECTOR moveDirection = VZero;

		// 行列で計算する
		MATRIX mRotY = MGetRotY(Deg2Rad(Camera::main->GetRotation().y));		//  カメラのY軸回転行列
		moveDirection = VTransform(inputVec, mRotY);

		//  計算した移動方向ベクトルを加算
		position = VAdd(position, VScale(moveDirection, speed));
	}

	//  カメラシェイク用
	if (isShaking) {
		//  タイマーを始動
		timer += TimeManager::GetInstance()->GetDeltaTime();
		//  揺れ => カメラの位置を変えている
		position = VAdd(position, VScale(shakePattern, sinf(timer * FPS)));

		//　揺れ終了の検知
		if (timer >= shakeTime) {
			isShaking = false;
			timer = 0.0f;
		}
	}

	//  カメラの位置と回転を設定する
	SetCameraPositionAndAngle(position, Deg2Rad(rotation.x), Deg2Rad(rotation.y), Deg2Rad(rotation.z));

}

/*
	*   @function	Render
	*   @brief		描画処理
	*/
void Camera::Render() {
#if _DEBUG
	DrawFormatString(0, 0, red, "Camera.position : %.2f, %.2f, %.2f", position.x, position.y, position.z);
	DrawFormatString(0, 20, red, "Camera.rotation : %.2f, %.2f, %.2f", rotation.x, rotation.y, rotation.z);
#endif
}

/*
*	@function	Shake
*	@brief		カメラシェイク、カメラの揺れ
*	@param[in]	int _direction			揺れの方向  0:横, 1:縦, 2:両方
*	@param[in]	float _time				揺れの長さ
*	@param[in]	float _power = 0.25f	揺れの強さ
*/
void Camera::Shake(int _direction, float _time, float _power) {
	//  各種必要な変数を初期化
	timer = 0.0f;
	shakeTime = _time;
	isShaking = true;
	shakeOffset = VGet(100, 100, 100);

	//  揺らす方向に応じて揺れのパターンを変更する
	switch (_direction) {
	case 0://　横
		shakePattern = VGet(shakeOffset.x, 0.0f, 0.0f);
		shakePattern = VTransform(shakePattern, MGetRotY(Deg2Rad(rotation.y)));
		break;

	case 1://　縦
		shakePattern = VGet(0.0f, shakeOffset.y, 0.0f);;

	default://　両方 + 前後
		shakePattern = shakeOffset;
	}

	//  揺れの強さを掛ける
	shakePattern = VScale(shakePattern, _power);
}

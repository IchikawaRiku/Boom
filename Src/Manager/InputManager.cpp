#include "InputManager.h"
#include <DxLib.h>

#pragma region シングルトン

//  静的メンバ変数の初期化
InputManager* InputManager::pInstance = nullptr;

/*
*   @brief		コンストラクタ
*   @tips		外部で生成されないようにアクセス指定子を private にする
*/
InputManager::InputManager()
	: keyState()
	, prevKeyState()
	, prevMouseButtonL(false)
	, currentMouseButtonL(false)
	, mouseX(0.0f)
	, mouseY(0.0f)
	, mouseDownX(0.0f)
	, mouseDownY(0.0f){

}

/*
*   @function	CreateInstance
*   @brief		自身のインスタンスを生成する
*/
void InputManager::CreateInstance() {
	pInstance = new InputManager();
}

/*
*   @function	GetInstance
*   @brief		自身のインスタンスを取得する唯一の手段
*   @return		InputManager*	自身のインスタンスのアドレス
*   @tips		生成は１度だけ行う
*/
InputManager* InputManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

/*
*   @function	DestroyInstance
*   @brief		自身のインスタンスを破棄する唯一の手段
*/
void InputManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

#pragma endregion

/*
*   @function   Update
*   @brief		更新処理
*/
void InputManager::Update() {
	//  1フレーム前の状態を保存
	memcpy_s(prevKeyState, 256, keyState, 256);
	//　今のフレームの状態を取得
	GetHitKeyStateAll(keyState);

	prevMouseButtonL = currentMouseButtonL;
	currentMouseButtonL = (GetMouseInput() & MOUSE_INPUT_LEFT);

	// マウスの移動量保存
	GetMousePoint(&mouseX, &mouseY);
	if (IsMouseButtonDown()) {
		mouseDownX = mouseX;
		mouseDownY = mouseY;
	}
	else if (IsMouseButtonUp()) {
		mouseDownX = 0;
		mouseDownY = 0;
	}

}
#pragma once
#include "../Definition.h"

/*
*   @brief  入力管理クラス
* 　@tips　	シングルトン
*/
class InputManager {
#pragma region  シングルトンのデータ構造
private:	//　静的メンバ変数
	static InputManager* pInstance;		//  自身のインスタンスのアドレスを格納

private:	//  コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @tips		外部で生成されないようにアクセス指定子を private にする
	*/
	InputManager();

	/*
	*   @brief		デストラクタ
	*/
	~InputManager() = default;

public:		//  コピーと譲渡の禁止
	InputManager(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator = (const InputManager&) = delete;
	InputManager& operator = (InputManager&&) = delete;

private:	//  静的メンバ変数
	/*
	*   @function	CreateInstance
	*   @brief		自身のインスタンスを生成する
	*/
	static void CreateInstance();

public:		//	静的メンバ変数
	/*
	*   @function	GetInstance
	*   @brief		自身のインスタンスを取得する唯一の手段
	*   @return		InputManager*	自身のインスタンスのアドレス
	*/
	static InputManager* GetInstance();

	/*
	*   @function	DestroyInstance
	*   @brief		自身のインスタンスを破棄する唯一の手段
	*/
	static void DestroyInstance();

#pragma endregion

private:	//　メンバ変数
	char keyState[256];			//  現在のキーの状態
	char prevKeyState[256];		//　１フレーム前のキーの状態

	bool prevMouseButtonL;
	bool currentMouseButtonL;

	int mouseX;
	int mouseY;
	int mouseDownX;
	int mouseDownY;

public:		//　メンバ変数
	/*
	*   @function   Update
	*   @brief		更新処理
	*/
	void Update();

public:		//  キーボード用入力管理
	/*
	*   @function	IsKeyDown
	*   @brief		キーが押されたかどうか
	*   @param[in]	int _key	キー番号
	*   @return		bool]
	*   @tips		押されていない状態 -> 押された状態
	*/
	inline bool IsKeyDown(int _key) const {
		//  !前 && 今
		return !prevKeyState[_key] && keyState[_key];
	}

	/*
	*   @function	IsKey
	*   @brief		キーが押しているかどうか
	*   @param[in]	int _key	キー番号
	*   @return		bool]
	*   @tips		押されている状態
	*/
	inline bool IsKey(int _key) const {
		//  今
		return keyState[_key];
	}

	/*
	*   @function	IsKeyUp
	*   @brief		キーが離されたかどうか
	*   @param[in]	int _key	キー番号
	*   @return		bool
	*   @tips		押されている状態 -> 押されていない状態
	*/
	inline bool IsKeyUp(int _key) const {
		//  前 && !今
		return prevKeyState[_key] && !keyState[_key];
	}

public:		// マウス用入力管理
	/*
	*	@function	IsMouseButtonDown
	*	@brief		マウスが押されたかどうか
	*	@return		bool
	*/
	inline bool IsMouseButtonDown() const {
		return !prevMouseButtonL && currentMouseButtonL;
	}

	/*
	*	@function	IsMouseButtonDown
	*	@brief		マウスが押されているどうか
	*	@return		bool
	*/
	inline bool IsMouseButton() const {
		return currentMouseButtonL;
	}

	/*
	*	@function	IsMouseButtonDown
	*	@brief		マウスが離されたかどうか
	*	@return		bool
	*/
	inline bool IsMouseButtonUp() const {
		return prevMouseButtonL && !currentMouseButtonL;
	}

	/*
	*	@function	GetMouseDownX
	*	@brief		マウスが押された位置X
	*	@return		bool
	*/
	inline int GetMouseDownX() const {
		return mouseDownX;
	}

	/*
	*	@function	GetMouseDownY
	*	@brief		マウスが押された位置Y
	*	@return		bool
	*/
	inline int GetMouseDownY() const {
		return mouseDownY;
	}

	/*
	*	@function	GetMouseX
	*	@brief		マウスの位置X
	*	@return		bool
	*/
	inline int GetMouseX() const {
		return mouseX;
	}

	/*
	*	@function	GetMouseY
	*	@brief		マウスの位置Y
	*	@return		bool
	*/
	inline int GetMouseY() const {
		return mouseY;
	}

	/*
	*	@function	GetMouseMoveValueX
	*	@brief		マウスが押されてからのXの移動量
	*	@return		int
	*/
	inline int GetMouseMoveValueX() const {
		return mouseDownX - mouseX;
	}

	/*
	*	@function	GetMouseMoveValueY
	*	@brief		マウスが押されてからのYの移動量
	*	@return		int
	*/
	inline int GetMouseMoveValueY() const {
		return mouseDownY - mouseY;
	}

public:
	inline bool SameVec(VECTOR _Vec1, VECTOR _Vec2) {
		if (_Vec1.x == _Vec2.x  && _Vec1.y == _Vec2.y && _Vec1.z == _Vec2.z) {
			return true;
		}
		else {
			return false;
		}
	}
};


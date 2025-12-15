#pragma once
#include "../Definition.h"

/*
*	@brief	シーン管理クラス
*/
class SceneManager {
#pragma region  シングルトンのデータ構造
private:	//　静的メンバ変数
	static SceneManager* pInstance;		//  自身のインスタンスのアドレスを格納

private:	//  コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @tips		外部で生成されないようにアクセス指定子を private にする
	*/
	SceneManager();

	/*
	*   @brief		デストラクタ
	*/
	~SceneManager();

public:		//  コピーと譲渡の禁止
	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator = (const SceneManager&) = delete;
	SceneManager& operator = (SceneManager&&) = delete;

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
	*   @return		SceneManager*	自身のインスタンスのアドレス
	*/
	static SceneManager* GetInstance();

	/*
	*   @function	DestroyInstance
	*   @brief		自身のインスタンスを破棄する唯一の手段
	*/
	static void DestroyInstance();

#pragma endregion

private:	//  メンバ変数
	class BaseScene* pCurrentScene;
	SceneType current;
	SceneType next;

	bool changed = false;
	bool title = false;

public:		//   メンバ関数

	/*
	*	@function	Update
	*	@brief		更新処理
	*/
	void Update();

	/*
	*	@function	Render
	*	@brief		更新処理
	*/
	void Render();

	/*
	*	@function	LoadScene
	*	@brief		シーン遷移
	*/
	void LoadScene();

	/*
	*	@function	SetNextScene
	*	@brief		シーン遷移の予約
	*	@param[in]	SceneType _next
	*/
	void SetNextScene(SceneType _next);

public:		//　GetterとSetter

	inline bool GetTitleFlag() { return title; }
};


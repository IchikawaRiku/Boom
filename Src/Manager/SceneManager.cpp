#include "SceneManager.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"


#pragma region シングルトン

//  静的メンバ変数の初期化
SceneManager* SceneManager::pInstance = nullptr;

/*
*   @brief		コンストラクタ
*   @tips		外部で生成されないようにアクセス指定子を private にする
*/
SceneManager::SceneManager()
	: pCurrentScene(nullptr)
	, next(SceneType::Title)
	, current((SceneType)INVALID){

	////  遷移先のシーンを生成する
	//switch (next) {
	//case SceneType::Title:
	//	pCurrentScene = new TitleScene();
	//	break;

	//case SceneType::Game:
	//	pCurrentScene = new GameScene();
	//	break;

	//default:
	//	pCurrentScene = nullptr;
	//	break;
	//}
	//current = next;
}

/*
*   @brief		デストラクタ
*/
SceneManager::~SceneManager() {

}

/*
*   @function	CreateInstance
*   @brief		自身のインスタンスを生成する
*/
void SceneManager::CreateInstance() {
	pInstance = new SceneManager();
}

/*
*   @function	GetInstance
*   @brief		自身のインスタンスを取得する唯一の手段
*   @return		SceneManager*	自身のインスタンスのアドレス
*   @tips		生成は１度だけ行う
*/
SceneManager* SceneManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

/*
*   @function	DestroyInstance
*   @brief		自身のインスタンスを破棄する唯一の手段
*/
void SceneManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

#pragma endregion

/*
*	@function	Update
*	@brief		更新処理
*/
void SceneManager::Update() {
	//  シーンがなければ更新しない
	if (pCurrentScene != nullptr)
		//  現在のシーンの更新
		pCurrentScene->Update();

	//  シーンの切り替えがあれば遷移する
	if (current != next)
		LoadScene();
}

/*
*	@function	Render
*	@brief		更新処理
*/
void SceneManager::Render() {
	//　シーンがなければ描画しない
	if (pCurrentScene == nullptr)
		return;

	//  現在のシーンの描画
	pCurrentScene->Render();
}

/*
*	@function	LoadScene
*	@brief		シーン遷移
*/
void SceneManager::LoadScene() {
	//  現在のシーンタイプを切り替える
	current = next;

	//  現在のシーンを削除する
	delete pCurrentScene;
	pCurrentScene = nullptr;

	//  遷移先のシーンを作成する
	switch (next) {
	case SceneType::Title:
		title = true;
		pCurrentScene = new TitleScene();
		break;

	case SceneType::Game:
		title = false;
		pCurrentScene = new GameScene();
		break;

	default:
		pCurrentScene = nullptr;
		break;
	}

}

/*
*	@function	SetNextScene
*	@brief		シーン遷移の予約
*	@param[in]	SceneType _next
*/
void SceneManager::SetNextScene(SceneType _next) {
	//  次に読み込むシーンをセットする
	next = _next;
	changed = true;
}

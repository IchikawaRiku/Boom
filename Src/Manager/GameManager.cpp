#include "GameManager.h"
#include "../Definition.h"
#include "../Manager/InputManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/ScoreManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/FadeManager.h"
#include "../Manager/BulletManager.h"

#pragma region シングルトン

//  静的メンバ変数の初期化
GameManager* GameManager::pInstance = nullptr;

/*
*   @brief		コンストラクタ
*   @tips		外部で生成されないようにアクセス指定子を private にする
*/
GameManager::GameManager()
	: GameOverFlag(false)
	, changed(false)
	, arrow(false)
	, mouse(false)
	, Exit(false)
	, ArrowModel(INVALID)
	, MouseModel(INVALID)
	, MouseLeftModel(INVALID)
	, HP(4)
	, ArrowX(700.0f)
	, ArrowY(700.0f)
	, Count(0.0f) {
}

/*
*   @brief		デストラクタ
*/
GameManager::~GameManager() {
	DeleteFontToHandle(NomalFont);
}

/*
*   @function	CreateInstance
*   @brief		自身のインスタンスを生成する
*/
void GameManager::CreateInstance() {
	pInstance = new GameManager();
}

/*
*   @function	GetInstance
*   @brief		自身のインスタンスを取得する唯一の手段
*   @return		GameManager*	自身のインスタンスのアドレス
*   @tips		生成は１度だけ行う
*/
GameManager* GameManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

/*
*   @function	DestroyInstance
*   @brief		自身のインスタンスを破棄する唯一の手段
*/
void GameManager::DestroyInstance() {
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
void GameManager::Update() {
	//  ゲームオーバーになったら
	if (GameOverFlag && ScoreManager::GetInstance()->GetScoreRender()) {
		//  画面クリックでスタートに戻る
		if (InputManager::GetInstance()->IsMouseButtonUp() && !changed) {
			changed = true;
			FadeManager::GetInstance()->FadeOut();
		}
		if (changed && FadeManager::GetInstance()->GetFadeState() == FadeState::FadeEnd) {
			SceneManager::GetInstance()->SetNextScene(SceneType::Title);
			FadeManager::GetInstance()->FadeIn();
		}
	}
	else {
		changed = false;
	}

	//  1秒を繰り返すカウント
	Count += TimeManager::GetInstance()->GetDeltaTime();
	if (Count > 0.7f) {
		Count = 0;
		if (mouse)
			mouse = false;
		else
			mouse = true;
	}
}

/*
*   @function   TitleUpdate
*   @brief		タイトルの更新処理
*/
void GameManager::TitleUpdate() {
	//  入力管理クラスの取得
	InputManager* input = InputManager::GetInstance();
	//  1秒を繰り返すカウント
	Count += TimeManager::GetInstance()->GetDeltaTime();
	if (Count > 0.7f) {
		Count = 0;
		ArrowY = 700.0f;
	}
	ArrowY = input->GetMouseDownY() + Count * 100;
	ArrowX = input->GetMouseDownX() - 75;

	if (input->IsMouseButtonDown() &&
		input->GetMouseY() > WINDOW_HEIGHT * 0.73f &&
		input->GetMouseY() < WINDOW_HEIGHT * 0.9f &&
		input->GetMouseX() > WINDOW_WIDTH * 0.075f &&
		input->GetMouseX() < WINDOW_WIDTH * 0.16f) {
		arrow = false;
		Exit = true;
	}

}

/*
*   @function   Render
*   @brief		描画処理
*/
void GameManager::Render() {
	//  ゲームオーバーになったら表示
	if (GameOverFlag) {
		DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle("HIGH SCORE", 10, NomalFont)) / 2,
			WINDOW_HEIGHT * 0.7 - 240, red, NomalFont, "HIGH SCORE");

		//  全てのUIが表示仕切ったら
		if (ScoreManager::GetInstance()->GetScoreRender()) {
			if (mouse)
				DrawGraph(WINDOW_WIDTH * 0.7f, WINDOW_HEIGHT * 0.6f, MouseModel, true);
			else
				DrawGraph(WINDOW_WIDTH * 0.7f, WINDOW_HEIGHT * 0.6f, MouseLeftModel, true);
	}		}


	if (arrow) {
		DrawGraph(ArrowX, ArrowY, ArrowModel, true);
	}

	DrawGraph(WINDOW_WIDTH * 0.05f, WINDOW_HEIGHT * 0.65f, ExitModel, true);
	
	for (int i = 0; i < HP; i++)
		DrawGraph(WINDOW_WIDTH * (0.02f + (i * 0.065f)), WINDOW_HEIGHT * 0.025f, HeartModel, true);

}

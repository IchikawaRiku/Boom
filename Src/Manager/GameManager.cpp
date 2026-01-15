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
	, Count(0.0f) 
	, INTERVAL_SEC(0.7f)
	, ARROW_Y_MAX(700.0f)
	, ARROW_SPEED(100.0f)
	, ARROW_HALF_SIZE(75)
	, BUTTON_X_MAX_RATIO(0.16f)
	, BUTTON_X_MIN_RATIO(0.075f)
	, BUTTON_Y_MAX_RATIO(0.9f)
	, BUTTON_Y_MIN_RATIO(0.73f)
	, HIGH_SCORE_TEXT("HIGH SCORE")
	, HIGH_SCORE_POS_Y_RATIO(0.7f)
	, HIGH_SCORE_POS_Y_OFFSET(240)
	, MOUSE_POS_X_RATIO(0.7f)
	, MOUSE_POS_Y_RATIO(0.6f)
	, EXIT_POS_X_RATIO(0.05f)
	, EXIT_POS_Y_RATIO(0.65f)
	, HEART_POS_X_RATIO(0.02f)
	, HEART_POS_Y_RATIO(0.025f)
	, HEART_POS_INTERVAL(0.065f)
{
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
	if (Count > INTERVAL_SEC) {
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
	if (Count > INTERVAL_SEC) {
		Count = 0;
		ArrowY = ARROW_Y_MAX;
	}
	ArrowY = input->GetMouseDownY() + Count * ARROW_SPEED;
	ArrowX = input->GetMouseDownX() - ARROW_HALF_SIZE;

	if (input->IsMouseButtonDown() &&
		input->GetMouseY() > WINDOW_HEIGHT * BUTTON_Y_MIN_RATIO &&
		input->GetMouseY() < WINDOW_HEIGHT * BUTTON_Y_MAX_RATIO &&
		input->GetMouseX() > WINDOW_WIDTH * BUTTON_X_MIN_RATIO &&
		input->GetMouseX() < WINDOW_WIDTH * BUTTON_X_MAX_RATIO) {
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
		DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle(HIGH_SCORE_TEXT, _tcslen(HIGH_SCORE_TEXT), NomalFont)) / 2,
			WINDOW_HEIGHT * HIGH_SCORE_POS_Y_RATIO - HIGH_SCORE_POS_Y_OFFSET, red, NomalFont, HIGH_SCORE_TEXT);

		//  全てのUIが表示仕切ったら
		if (ScoreManager::GetInstance()->GetScoreRender()) {
			if (mouse)
				DrawGraph(WINDOW_WIDTH * MOUSE_POS_X_RATIO, WINDOW_HEIGHT * MOUSE_POS_Y_RATIO, MouseModel, true);
			else
				DrawGraph(WINDOW_WIDTH * MOUSE_POS_X_RATIO, WINDOW_HEIGHT * MOUSE_POS_Y_RATIO, MouseLeftModel, true);
	}		}


	if (arrow) {
		DrawGraph(ArrowX, ArrowY, ArrowModel, true);
	}

	DrawGraph(WINDOW_WIDTH * EXIT_POS_X_RATIO, WINDOW_HEIGHT * EXIT_POS_Y_RATIO, ExitModel, true);
	
	for (int i = 0; i < HP; i++)
		DrawGraph(WINDOW_WIDTH * (HEART_POS_X_RATIO + (i * HEART_POS_INTERVAL)), WINDOW_HEIGHT * HEART_POS_Y_RATIO, HeartModel, true);

}

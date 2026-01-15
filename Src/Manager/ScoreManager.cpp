#include "ScoreManager.h"
#include "GameManager.h"
#include "TimeManager.h"
#include "AudioManager.h"
#include <DxLib.h>
#include "../Definition.h"

#pragma region シングルトン

//  静的メンバ変数の初期化
ScoreManager* ScoreManager::pInstance = nullptr;

/*
*   @brief		コンストラクタ
*   @tips		外部で生成されないようにアクセス指定子を private にする
*/
ScoreManager::ScoreManager()
	: TotalScore(0)
	, SaveScore(0)
	, count(0.0f)
	, scoreNum(0)
	, NewRecord(false)
	, SE(false)
	, scoreRender(false) 
	, SCORE_VISIBLE_MAX(10)
	, SCORE_HOLD_MAX(5)
	, SCORE_VISIBLE_COUNT(0.02f)
	, SCORE_TEXT("SCORE : ")
	, SCORE_COUNT_TEXT("SCORE : %d")
	, NEW_RECORD_TEXT("NEW RECORD!!")
	, SCORE_TEXT_POS_X_RATIO(1.2f)
	, SCORE_TEXT_POS_Y_RATIO(40)
	, HIGH_SCORE_TEXT_POS_X_RATIO(2.2f)
	, HIGH_SCORE_TEXT_POS_Y_RATIO(0.7f)
	, HIGH_SCORE_TEXT_INTERVAL(40)
	, BIN_NAME("Score.bin")
{
	for (int i = 0; i < SCORE_VISIBLE_MAX; i++)
	{
		score[i].isVisible = false;
		score[i].timer = 2.0f;
		score[i].scorePos = VZero;
		score[i].amount = 0;
	}
	Start();
}

/*
*   @brief		デストラクタ
*/
ScoreManager::~ScoreManager() {
	DeleteFontToHandle(NomalFont);
	DeleteFontToHandle(BigFont);
	DeleteFontToHandle(ScoreFont);
}

/*
*   @function	CreateInstance
*   @brief		自身のインスタンスを生成する
*/
void ScoreManager::CreateInstance() {
	pInstance = new ScoreManager();
}

/*
*   @function	GetInstance
*   @brief		自身のインスタンスを取得する唯一の手段
*   @return		ScoreManager*	自身のインスタンスのアドレス
*   @tips		生成は１度だけ行う
*/
ScoreManager* ScoreManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

/*
*   @function	DestroyInstance
*   @brief		自身のインスタンスを破棄する唯一の手段
*/
void ScoreManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

#pragma endregion

/*
*   @function   Start
*   @brief		初期化処理
*/
void ScoreManager::Start() {
	//  スコアのロード
	ScoreRankingLoad();
}

/*
*   @function   Update
*   @brief		更新処理
*/
void ScoreManager::Update() {
	//  ゲームオーバーになったらスコアランキングを調節
	if (GameManager::GetInstance()->GetGameOverFlag()) {
		if (ScoreRanking[0] < SaveScore) {
			NewRecord = true;
		}
		//　ランキング5位のスコアを超えたら更新
		if (ScoreRanking[SCORE_HOLD_MAX - 1] <= SaveScore) {
			ScoreRanking[SCORE_HOLD_MAX - 1] = SaveScore;
			SaveScore = 0;
		}
		//　ランキングの調節
		for (int i = 0; i < SCORE_HOLD_MAX; i++) {
			if (ScoreRanking[SCORE_HOLD_MAX - 1] >= ScoreRanking[i]) {
				//  ランキングの入れ替え
				SaveScore = ScoreRanking[i];
				ScoreRanking[i] = ScoreRanking[SCORE_HOLD_MAX - 1];
				ScoreRanking[SCORE_HOLD_MAX - 1] = SaveScore;
			}
		}
		//  スコアのセーブ
		ScoreRankingSave();
	}
	else {
		//  スコアを保存しておく
		SaveScore = TotalScore;
		//  ゲームオーバー後に必要なものは初期化しておく
		NewRecord = false;
		SE = false;
		scoreRender = false;
	}
}

/*
*   @function   Render
*   @brief		描画処理
*/
void ScoreManager::Render() {
	//  スコア表示
	for (int i = 0; i < SCORE_VISIBLE_MAX; i++) {
		//  非表示ならスキップ
		if (!score[i].isVisible)
			continue;

		VECTOR screen = ConvWorldPosToScreenPos(score[i].scorePos);

		DrawFormatStringToHandle(
			screen.x,
			screen.y,
			yellow, NomalFont, "%d", score[i].amount);
		score[i].timer -= SCORE_VISIBLE_COUNT;
		// 一定時間たったら表示しない
		if (score[i].timer <= 0)
			score[i].isVisible = false;
	}
	DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle(SCORE_TEXT, _tcslen(SCORE_TEXT), ScoreFont)) / SCORE_TEXT_POS_X_RATIO,
		WINDOW_HEIGHT / SCORE_TEXT_POS_Y_RATIO, red, ScoreFont, SCORE_COUNT_TEXT, TotalScore);

	//　ゲームオーバーになったらランキングを表示
	if (GameManager::GetInstance()->GetGameOverFlag()) {
		count += TimeManager::GetInstance()->GetDeltaTime();
		//  0.5秒ごとにスコアを表示
		if (count >= 0.5f) {
			if (scoreNum != SCORE_HOLD_MAX) {
				count = 0;
				scoreNum += 1;
				AudioManager::GetInstance()->PlayOneShot(SCORE_NAME, 1);
			}
			//　SE後なら0.5秒ごとに点滅
			if (SE) {
				DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle(NEW_RECORD_TEXT, _tcslen(NEW_RECORD_TEXT), BigFont)) / 2,
					WINDOW_HEIGHT / 3, red, BigFont, NEW_RECORD_TEXT);
				if (count >= 1) {
					count = 0;
				}
			}
		}
		//　スコア表示1秒後最高記録なら NEW RECORD
		if (count >= 1) {
			scoreRender = true;
			if (NewRecord) {
				DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle(NEW_RECORD_TEXT, _tcslen(NEW_RECORD_TEXT), BigFont)) / 2,
					WINDOW_HEIGHT / 3, red, BigFont, NEW_RECORD_TEXT);
				if (!SE) {
					AudioManager::GetInstance()->PlayOneShot(NEW_RECORD_NAME, NEW_RECORD_SE_VOLUME);
					SE = true;
				}
				if (count >= 1.5f) {
					count = 0;
				}
			}
		}
		for (int i = 0; i < scoreNum; i++) {
			DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle(SCORE_TEXT, _tcslen(SCORE_TEXT), NomalFont)) / HIGH_SCORE_TEXT_POS_X_RATIO,
				WINDOW_HEIGHT * HIGH_SCORE_TEXT_POS_Y_RATIO - ((i + 1) * HIGH_SCORE_TEXT_INTERVAL), white, NomalFont, SCORE_COUNT_TEXT, ScoreRanking[(SCORE_HOLD_MAX - 1) - i]);
		}
	}
}

/*
*   @function   ScoreRankingSave
*   @brief		スコアランキングのセーブ
*/
void ScoreManager::ScoreRankingSave() {
	FILE* fp;

	// ハイスコアセーブ用ファイルを開く
	fopen_s(&fp, BIN_NAME, "wb");

	// オープンできなかったらここで終了
	if (fp == NULL) return;

	// ハイスコアデータの書き出し
	for (int i = 0; i < SCORE_HOLD_MAX; i++) {
		fwrite(&ScoreRanking[i], sizeof(ScoreRanking[i]), 1, fp);
	}

	// ファイルを閉じる
	fclose(fp);
}

/*
*   @function   ScoreRankingLoad
*   @brief		スコアランキングのロード
*/
void ScoreManager::ScoreRankingLoad() {
	FILE* fp;

	// ハイスコアセーブ用ファイルを開く
	fopen_s(&fp, BIN_NAME, "rb");

	// オープンできなかったらファイルが無いとみなし
	// 標準スコア０をセット
	if (fp == NULL) {
		for (int i = 0; i < SCORE_HOLD_MAX; i++) {
			ScoreRanking[i] = 0;
		}
	}
	else {
		for (int i = 0; i < SCORE_HOLD_MAX; i++) {
			// ハイスコアデータの読み込み
			fread(&ScoreRanking[i], sizeof(ScoreRanking[i]), 1, fp);
		}
		// ファイルを閉じる
		fclose(fp);
	}
}

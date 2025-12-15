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
	, scoreRender(false) {
	for (int i = 0; i < 10; i++)
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
		if (ScoreRanking[4] <= SaveScore) {
			ScoreRanking[4] = SaveScore;
			SaveScore = 0;
		}
		//　ランキングの調節
		for (int i = 0; i < 5; i++) {
			if (ScoreRanking[4] >= ScoreRanking[i]) {
				//  ランキングの入れ替え
				SaveScore = ScoreRanking[i];
				ScoreRanking[i] = ScoreRanking[4];
				ScoreRanking[4] = SaveScore;
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
	for (int i = 0; i < 10; i++) {
		//  非表示ならスキップ
		if (!score[i].isVisible)
			continue;

		VECTOR screen = ConvWorldPosToScreenPos(score[i].scorePos);

		DrawFormatStringToHandle(
			screen.x,
			screen.y,
			yellow, NomalFont, "%d", score[i].amount);
		score[i].timer -= 0.02f;
		// 一定時間たったら表示しない
		if (score[i].timer <= 0)
			score[i].isVisible = false;
	}
	//DrawFormatStringToHandle(620, 10, red, NomalFont, "SCORE : %d", TotalScore);
	DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle("SCORE : ", 8, NomalFont)) / 1.15,
		WINDOW_HEIGHT / 100, red, NomalFont, "SCORE : %d", TotalScore);

	//　ゲームオーバーになったらランキングを表示
	if (GameManager::GetInstance()->GetGameOverFlag()) {
		count += TimeManager::GetInstance()->GetDeltaTime();
		//  0.5秒ごとにスコアを表示
		if (count >= 0.5f) {
			if (scoreNum != 5) {
				count = 0;
				scoreNum += 1;
				AudioManager::GetInstance()->PlayOneShot("Score", 1);
			}
			//　SE後なら0.5秒ごとに点滅
			if (SE) {
				DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle("NEW RECORD!!", 12, BigFont)) / 2,
					WINDOW_HEIGHT / 3, red, BigFont, "NEW RECORD!!");
				if (count >= 1) {
					count = 0;
				}
			}
		}
		//　スコア表示1秒後最高記録なら NEW RECORD
		if (count >= 1) {
			scoreRender = true;
			if (NewRecord) {
				DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle("NEW RECORD!!", 12, BigFont)) / 2,
					WINDOW_HEIGHT / 3, red, BigFont, "NEW RECORD!!");
				if (!SE) {
					AudioManager::GetInstance()->PlayOneShot("NewRecord", 0.7f);
					SE = true;
				}
				if (count >= 1.5f) {
					count = 0;
				}
			}
		}
		for (int i = 0; i < scoreNum; i++) {
			DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle("SCORE : ", 8, NomalFont)) / 2.2,
				WINDOW_HEIGHT * 0.7 - ((i + 1) * 40), white, NomalFont, "SCORE : %d", ScoreRanking[4 - i]);
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
	fopen_s(&fp, "Score.bin", "wb");

	// オープンできなかったらここで終了
	if (fp == NULL) return;

	// ハイスコアデータの書き出し
	for (int i = 0; i < 5; i++) {
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
	fopen_s(&fp, "Score.bin", "rb");

	// オープンできなかったらファイルが無いとみなし
	// 標準スコア０をセット
	if (fp == NULL) {
		for (int i = 0; i < 5; i++) {
			ScoreRanking[i] = 0;
		}
	}
	else {
		for (int i = 0; i < 5; i++) {
			// ハイスコアデータの読み込み
			fread(&ScoreRanking[i], sizeof(ScoreRanking[i]), 1, fp);
		}
		// ファイルを閉じる
		fclose(fp);
	}
}

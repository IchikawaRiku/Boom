#pragma once
#include <DxLib.h>

/*
*   @brief  スコア管理クラス
* 　@tips　	シングルトン
*/
class ScoreManager {
#pragma region  シングルトンのデータ構造
private:	//　静的メンバ変数
	static ScoreManager* pInstance;		//  自身のインスタンスのアドレスを格納

private:	//  コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @tips		外部で生成されないようにアクセス指定子を private にする
	*/
	ScoreManager();

	/*
	*   @brief		デストラクタ
	*/
	~ScoreManager();

public:		//  コピーと譲渡の禁止
	ScoreManager(const ScoreManager&) = delete;
	ScoreManager(ScoreManager&&) = delete;
	ScoreManager& operator = (const ScoreManager&) = delete;
	ScoreManager& operator = (ScoreManager&&) = delete;

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
	*   @return		ScoreManager*	自身のインスタンスのアドレス
	*/
	static ScoreManager* GetInstance();

	/*
	*   @function	DestroyInstance
	*   @brief		自身のインスタンスを破棄する唯一の手段
	*/
	static void DestroyInstance();

#pragma endregion

private:	//　メンバ変数
	int TotalScore;			//  ゲーム中のトータルスコア
	int ScoreRanking[5];	//  スコアをランキング形式に保存する
	int SaveScore;			//  スコアを一時保存する変数
	float count;
	int scoreNum;
	bool NewRecord;			//  新記録フラグ
	bool SE;
	bool scoreRender;		//  スコア表示フラグ

	const int BigFont = CreateFontToHandle("Snap ITC", 70, 7, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	const int NomalFont = CreateFontToHandle("Snap ITC", 40, 7, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);

public:		//　メンバ関数
	struct Score{
		bool isVisible;
		float timer;
		VECTOR scorePos;
		int amount;
	};
	Score score[10];

	/*
	*   @function   Start
	*   @brief		初期化処理
	*/
	void Start();
	/*
	*   @function   Update
	*   @brief		更新処理
	*/
	void Update();
	/*
	*   @function   Render
	*   @brief		描画処理
	*/
	void Render();

	/*
	*   @function   ScoreRankingSave
	*   @brief		スコアランキングのセーブ
	*/
	void ScoreRankingSave();

	/*
	*   @function   ScoreRanking
	*   @brief		スコアランキングのロード
	*/
	void ScoreRankingLoad();

public:		//  Getter と Setter

	/*
	*   @function	GetTotalScore
	*   @brief		トータルスコアの取得
	*   @return		int
	*/
	inline int GetTotalScore() const { return TotalScore; }

	/*
	*   @function	SetTotalScore
	*   @brief		トータルスコアの変更
	*   @param[in]	int _v		初期化する値
	*/
	inline void SetTotalScore(int _v) { TotalScore = _v; }

	/*
	*   @function	GetScoreRender
	*   @brief		スコア表示フラグの取得
	*   @return		int
	*/
	inline int GetScoreRender() const { return scoreRender; }

	/*
	*   @function	SetScoreRender
	*   @brief		スコア表示フラグの変更
	*   @param[in]	int _v		初期化する値
	*/
	inline void SetScoreRender(int _v) { scoreRender = _v; }

};


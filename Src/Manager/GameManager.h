#pragma once
#include <DxLib.h>

class GameManager {
#pragma region  シングルトンのデータ構造
private:	//　静的メンバ変数
	static GameManager* pInstance;		//  自身のインスタンスのアドレスを格納

private:	//  コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @tips		外部で生成されないようにアクセス指定子を private にする
	*/
	GameManager();

	/*
	*   @brief		デストラクタ
	*/
	~GameManager();

public:		//  コピーと譲渡の禁止
	GameManager(const GameManager&) = delete;
	GameManager(GameManager&&) = delete;
	GameManager& operator = (const GameManager&) = delete;
	GameManager& operator = (GameManager&&) = delete;

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
	*   @return		GameManager*	自身のインスタンスのアドレス
	*/
	static GameManager* GetInstance();

	/*
	*   @function	DestroyInstance
	*   @brief		自身のインスタンスを破棄する唯一の手段
	*/
	static void DestroyInstance();

#pragma endregion

private:	//　メンバ変数
	bool GameOverFlag;		//  ゲームオーバーフラグ
	bool changed;			//  フェード用のフラグ
	bool arrow;
	bool mouse;
	bool Exit;
	float ArrowX;
	float ArrowY;
	float Count;
	int HP;
	int ArrowModel;
	int MouseModel;
	int MouseLeftModel;
	int ExitModel;
	int HeartModel;

	const int NomalFont = CreateFontToHandle("Snap ITC", 40, 7, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);

public:		//　メンバ変数
	/*
	*   @function   Update
	*   @brief		更新処理
	*/
	void Update();

	/*
	*   @function   TitleUpdate
	*   @brief		タイトルの更新処理
	*/
	void TitleUpdate();

	/*
	*   @function   Render
	*   @brief		描画処理
	*/
	void Render();

public:		//  Getter と Setter

	/*
	*   @function	GetGameOverFlag
	*   @brief		ゲームオーバーフラグの取得
	*   @return		bool
	*/
	inline bool GetGameOverFlag() const { return GameOverFlag; }

	/*
	*   @function	SetGameOverFlag
	*   @brief		ゲームオーバーフラグの変更
	*   @param[in]	bool _v		初期化する値
	*/
	inline void SetGameOverFlag(bool _v) { GameOverFlag = _v; }

	inline void SetArrowModel(int _v) { ArrowModel = _v; }
	inline void SetMouseModel(int _v) { MouseModel = _v; }
	inline void SetMouseLeftModel(int _v) { MouseLeftModel = _v; }
	inline void SetExitModel(int _v) { ExitModel = _v; }
	inline void SetHeartModel(int _v) { HeartModel = _v; }

	inline void SetArrowFlag(bool _v) { arrow = _v; }

	inline bool GetArrowFlag() const { return arrow; }
	inline bool GetExitFlag() const { return Exit; }

	inline void SetHP(int _v) { HP = _v; }
	inline int GetHP() const { return HP; }
};


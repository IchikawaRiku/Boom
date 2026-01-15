#pragma once
#include <vector>
#include "../GameObject/Bomb/Bomb.h"
class BombManager {
#pragma region  シングルトンのデータ構造
private:	//　静的メンバ変数
	static BombManager* pInstance;		//  自身のインスタンスのアドレスを格納

private:	//  コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @tips		外部で生成されないようにアクセス指定子を private にする
	*/
	BombManager();

	/*
	*   @brief		デストラクタ
	*/
	~BombManager() = default;

public:		//  コピーと譲渡の禁止
	BombManager(const BombManager&) = delete;
	BombManager(BombManager&&) = delete;
	BombManager& operator = (const BombManager&) = delete;
	BombManager& operator = (BombManager&&) = delete;

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
	*   @return		BombManager*	自身のインスタンスのアドレス
	*/
	static BombManager* GetInstance();

	/*
	*   @function	DestroyInstance
	*   @brief		自身のインスタンスを破棄する唯一の手段
	*/
	static void DestroyInstance();

#pragma endregion

private:	//　メンバ変数
	bool CoolTimeFlag;				//  クールタイム用のフラグ
	float CoolTime;					//  爆弾のクールタイム
	bool Generate;					//  爆弾の生成フラグ
	int bombModel;					//  爆弾のモデルの読み込み
	bool BreakFlag;					//  Break用のフラグ
	float Time;						//  時間を計測する
	bool GameStart;					//  ゲームをスタートする変数
	int bombCount;					//  タイトル画面で破壊されたボムの数

	const int BOMB_MAX;				//  爆弾の最大数
	const int COOLTIME_MAX;			//  クールタイムの最大値
	const int TITLE_BOMB;			//	タイトルに用意するボムの数
	const int START_BOMB;			//	最初に生成されるボムの数
	const float GENERATE_POS_Y;		//	生成されるY座標
	const float GENERATE_RAND_X;	//  生成位置のランダム値X
	const float GENERATE_RAND_Z;	//  生成位置のランダム値Z
	const float GENERATE_OFFSET_X;	//  生成位置の補正値X
	const float GENERATE_OFFSET_Z;	//  生成位置の補正値Z
	const float GENERATE_COOLTIME_MAX;	//	生成クールタイムの最大
	const VECTOR TITLE_GENERATE_BASE_POS;	// タイトルの爆弾の初期生成位置
	const VECTOR TITLE_GENERATE_POS_1;		// タイトルに規定値に固定されている位置
	const VECTOR TITLE_GENERATE_POS_2;		// タイトルに規定値に固定されている位置
	const VECTOR TITLE_GENERATE_POS_3;		// タイトルに規定値に固定されている位置

	Bomb* pStartBomb[3];
	std::vector<Bomb*> pBomb;		//  爆弾の管理をする配列

public:		//　メンバ変数

	/*
	*   @function   Start
	*   @brief		初期化処理
	*/
	void Start();
	/*
	*   @function   TitleStart
	*   @brief		タイトルの初期化処理
	*/
	void TitleStart();
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
	/*
	*   @function   TitleRender
	*   @brief		タイトルの描画処理
	*/
	void TitleRender();
	/*
	*   @function   Delete
	*   @brief		解放処理
	*/
	void Delete();

public:		//  Getter と Setter

	/*
	*   @function	GetGenerate
	*   @brief		表示フラグの取得
	*   @return		bool
	*/
	inline bool GetGenerate() const { return Generate; }

	/*
	*   @function	SetGenerate
	*   @brief		表示フラグの変更
	*   @param[in]	bool _v		初期化する値
	*/
	inline void SetGenerate(bool _v) { Generate = _v; }
	
	/*
	*   @function	GetGameStart
	*   @brief		ゲームスタートの合図の取得
	*   @return		int
	*/
	inline int GetGameStart() const { return GameStart; }

	/*
	*   @function	SetGameStart
	*   @brief		ゲームスタートフラグの変更
	*   @param[in]	bool _v		初期化する値
	*/
	inline void SetGameStart(bool _v) { GameStart = _v; }

	/*
	*   @function	SetBombCount
	*   @brief		タイトル画面で破壊されたボムの数の変更
	*   @param[in]	bool _v		初期化する値
	*/
	inline void SetBombCount(bool _v) { bombCount = _v; }

	/*
	*   @function	GetBombCount
	*   @brief		タイトル画面で破壊されたボムの数の取得
	*   @return		int
	*/
	inline int GetBombCount() const { return bombCount; }

};




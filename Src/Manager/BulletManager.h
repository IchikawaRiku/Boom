#pragma once
#include <vector>
#include "../GameObject/Bullet/Bullet.h"
class BulletManager {
#pragma region  シングルトンのデータ構造
private:	//　静的メンバ変数
	static BulletManager* pInstance;		//  自身のインスタンスのアドレスを格納

private:	//  コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @tips		外部で生成されないようにアクセス指定子を private にする
	*/
	BulletManager();

	/*
	*   @brief		デストラクタ
	*/
	~BulletManager();

public:		//  コピーと譲渡の禁止
	BulletManager(const BulletManager&) = delete;
	BulletManager(BulletManager&&) = delete;
	BulletManager& operator = (const BulletManager&) = delete;
	BulletManager& operator = (BulletManager&&) = delete;

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
	*   @return		BulletManager*	自身のインスタンスのアドレス
	*/
	static BulletManager* GetInstance();

	/*
	*   @function	DestroyInstance
	*   @brief		自身のインスタンスを破棄する唯一の手段
	*/
	static void DestroyInstance();

#pragma endregion

private:	//　メンバ変数
	bool CoolTimeFlag;				//  クールタイム用のフラグ
	float CoolTime;					//  弾のクールタイム
	float TitleCoolTime;			//  タイトル用のクールタイム
	bool TitleDown;					//  タイトル用のクリックに置き換わるフラグ
	bool TitleUp;					//  タイトル用のクリックに置き換わるフラグ
	bool Generate;					//  弾の生成フラグ
	int bulletModel;				//  弾のモデルの読み込み
	bool BreakFlag;					//  Break用のフラグ
	bool Release;					//　弾を放つフラグ
	int DownMouseX;					//  ボタンを押したときのマウスX座標
	int DownMouseY;					//  ボタンを押したときのマウスY座標
	int MouseX;						//  マウスのX座標
	int MouseY;						//  マウスのY座標
	bool MouseButton;				//  ボタンを押しているフラグ
	
	const int BULLET_MAX;			//  弾の最大数
	const float COOLTIME_MAX;			//  クールタイムの最大値
	const float TITLE_COOLTIME_MAX;	//  タイトル用のクールタイムの最大値
	
	std::vector<Bullet*> pBullet;	//  弾の管理をする配列
	Bullet* ShotBullet;

public:		//　メンバ関数
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
	*   @function   TitleUpdate
	*   @brief		タイトル画面用の更新処理
	*/
	void TitleUpdate();
	/*
	*   @function   Render
	*   @brief		描画処理
	*/
	void Render();
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
	*   @function	GetRelease
	*   @brief		弾を放つフラグの取得
	*   @return		bool
	*/
	inline bool GetRelease() const { return Release; }

	/*
	*   @function	SetRelease
	*   @brief		弾を放つフラグの変更
	*   @param[in]	bool _v		初期化する値
	*/
	inline void SetRelease(bool _v) { Release = _v; }

	/*
	*   @function	GetMouseButton
	*   @brief		ボタンを押しているフラグの取得
	*   @return		bool
	*/
	inline bool GetMouseButton() const { return MouseButton; }

	/*
	*   @function	GetDownMouseX
	*   @brief		ボタンを押したときのマウスX座標の取得
	*   @return		int
	*/
	inline int GetDownMouseX() const { return DownMouseX; }

	/*
	*   @function	GetDownMouseY
	*   @brief		ボタンを押したときのマウスY座標の取得
	*   @return		int
	*/
	inline int GetDownMouseY() const { return DownMouseY; }

	/*
	*   @function	GetMouseX
	*   @brief		マウスX座標の取得
	*   @return		int
	*/
	inline int GetMouseX() const { return MouseX; }

	/*
	*   @function	GetMouseY
	*   @brief		マウスY座標の取得
	*   @return		int
	*/
	inline int GetMouseY() const { return MouseY; }


};


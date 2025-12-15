#pragma once
#include <vector>

/*
*   @brief  当たり判定の管理
* 　@tips	シングルトン
*/
class CollisionManager {
#pragma region  シングルトンのデータ構造
private:	//　静的メンバ変数
	static CollisionManager* pInstance;		//  自身のインスタンスのアドレスを格納

private:	//  コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @tips		外部で生成されないようにアクセス指定子を private にする
	*/
	CollisionManager();

	/*
	*   @brief		デストラクタ
	*/
	~CollisionManager();

public:		//  コピーと譲渡の禁止
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager(CollisionManager&&) = delete;
	CollisionManager& operator = (const CollisionManager&) = delete;
	CollisionManager& operator = (CollisionManager&&) = delete;

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
	*   @return		CollisionManager*	自身のインスタンスのアドレス
	*/
	static CollisionManager* GetInstance();

	/*
	*   @function	DestroyInstance
	*   @brief		自身のインスタンスを破棄する唯一の手段
	*/
	static void DestroyInstance();

#pragma endregion

private:	//　メンバ変数
	std::vector<class Collider*> pColliderArray;		//  当たり判定の一元管理
	std::vector<std::vector<bool>> prevs;				//  1フレーム前の衝突結果
	std::vector<std::vector<bool>> currents;			//  現在の衝突結果
	int index = 0;

public:		//　メンバ関数
	/*
	*   @function	Update
	*	@brief		更新処理
	*/
	void Update();

	/*
	*	@function	Register
	*	@brief		一元管理に登録する
	*   @param[in]	Collider* _pCol		登録する当たり判定
	*/
	void Register(class Collider* _pCol);

	/*
	*   @function	Unregister
	*	@brief		一元管理から抹消する
	*   @param[in]	Collider* _pCol		抹消する当たり判定
	*/
	void Unregister(class Collider* _pCol);

	/*
	*	@funtion	UnregisterAll
	*	@brief		一元管理から全て抹消する
	*/
	inline void UnregisterAll() {
		pColliderArray.clear();
		currents.clear();
		prevs.clear();
		index = 0;
	}

	/*
	*	@function	CheckHit
	*	@brief		当たり判定の衝突検知
	*	@param[in]	Collider* _pCol1		当たり判定１
	*   @param[in]  Collider* _pCol2		当たり判定２
	*	@return		bool
	*/
	bool CheckHit(class Collider* _pCol1, class Collider* _pCol2);
};


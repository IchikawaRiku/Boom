#pragma once
#include "../GameObject.h"

/*
*	//	爆弾クラス
*	//　GameObjectクラスの派生クラス
*/
class Bomb : public GameObject {
private:	//  メンバ変数

public:		//	コンストラクタとデストラクタ
	/*
	*   @brief	コンストラクタ
	*/
	Bomb(int _handle, VECTOR _pos = VZero);

	/*
	*   @brief	デストラクタ
	*/
	~Bomb();

public:		//  オーバーライドしたメンバ変数
	/*
	*   @function	Start
	*   @brief		初期化処理
	*/
	void Start() override;

	/*
	*   @function	Update
	*   @brief		更新処理
	*/
	void Update() override;

	/*
	*   @function	Render
	*   @brief		描画処理
	*/
	void Render() override;

public:		//  メンバ関数

	/*
	*   @function	TitleUpdate
	*   @brief		タイトルの更新処理
	*/
	void TitleUpdate();

};


#pragma once
#include "../GameObject.h"
/*
*	//	スコアクラス
*	//　GameObjectクラスの派生クラス
*/
class Score : public GameObject {
private:	//  メンバ変数

public:		//	コンストラクタとデストラクタ
	/*
	*   @brief	コンストラクタ
	*/
	Score(VECTOR _pos = VZero);

	/*
	*   @brief	デストラクタ
	*/
	~Score();

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

};


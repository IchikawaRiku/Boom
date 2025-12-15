#pragma once
#include "../GameObject.h"
class CannonBase : public GameObject {
private:	//  メンバ変数

public:		//	コンストラクタとデストラクタ
	/*
	*   @brief	コンストラクタ
	*/
	CannonBase(VECTOR _pos = VZero);

	/*
	*   @brief	デストラクタ
	*/
	~CannonBase();

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



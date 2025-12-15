#pragma once
#include "../GameObject.h"

/*
*   @brief	@エフェクトクラス
*/
class Effect : public GameObject {
private:	//  メンバ変数
	int resourceHandle;		// 
	int playingHandle;		//  再生中のハンドル

public:		//  コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*	@param[in]	int& _resourceHandle
	*/
	Effect(int& _resourceHandle);

	/*
	*	@brief		デストラクタ
	*/
	~Effect();

public:		//　オーバーライドするメンバ関数
	/*
	*   @function	Start
	*	@brief		初期化処理
	*/
	void Start() override;
	
	/*
	*   @function	Update
	*	@brief		更新処理
	*/
	void Update() override;
	
	/*
	*   @function	Render
	*	@brief		描画処理
	*/
	void Render() override;

public:		//　メンバ関数


public:		//  Getter と Setter
};


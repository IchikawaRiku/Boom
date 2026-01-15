#pragma once
#include "BaseScene.h"

/*
*	@brief	ゲーム中のシーン
*/
class GameScene : public BaseScene {
protected:			//  メンバ変数
	int cannonModel;			//  大砲のモデルハンドル
	int cannonBaseModel;		//  大砲の土台のモデルハンドル
	int stageModel;				//  ステージのモデルハンドル
	int backGroundModel;		//  背景のモデルハンドル
	int mouseModel;				//  マウスのモデルハンドル
	int mouseLeftModel;			//  マウスの左クリックモデルハンドル
	int heartModel;				//  ハートのモデルハンドル
	
	const VECTOR CAMERA_POS;

public:		//  コンストラクタとデストラクタ
	/*
	*	@brief	コンストラクタ
	*/
	GameScene();

	/*
	*	@brief	デストラクタ
	*/
	~GameScene();

public:		//  オーバーライドするメンバ関数
	/*
	*   @function	Start
	*	@brief		初期化処理
	*/
	void Start() override;

	/*
	*   @function	Update
	*	@brief		更新処理
	*	@tips		純粋仮想関数で実装
	*/
	void Update() override;

	/*
	*   @function	Render
	*	@brief		描画処理
	*	@tips		純粋仮想関数で実装
	*/
	void Render() override;
};
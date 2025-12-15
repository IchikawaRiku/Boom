#pragma once
#include "BaseScene.h"
class TitleScene : public BaseScene {
protected:			//  メンバ変数
	int cannonModel;			//  大砲のモデルハンドル
	int cannonBaseModel;		//  大砲の土台のモデルハンドル
	int stageModel;				//  ステージのモデルハンドル
	int backGroundModel;		//  背景のモデルハンドル
	int arrowModel;
	int exitModel;
	bool changed;				//  フェード用のフラグ
	bool countUp;				//  カウントアップ開始
	float fadeCount;			//  フェードするまでのカウント用
	float blinking;				//  点滅用
	bool start;
	bool click;

	const int UltoraBigFont = CreateFontToHandle("Snap ITC", 150, 7, DX_FONTTYPE_ANTIALIASING);
	const int BigFont = CreateFontToHandle("Snap ITC", 80, 9, DX_FONTTYPE_ANTIALIASING);
	const int SmallFont = CreateFontToHandle("Snap ITC", 40, 9, DX_FONTTYPE_ANTIALIASING);

public:				//  コンストラクタとデストラクタ
	/*
	*	@brief	コンストラクタ
	*/
	TitleScene();

	/*
	*	@brief	デストラクタ
	*/
	~TitleScene();

public:				//  オーバーライドするメンバ関数
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


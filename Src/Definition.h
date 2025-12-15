#pragma once
//	=================================================================
//		ヘッダーをインクルードする場所
//	=================================================================
#include <DxLib.h>
#include <string>

//	=================================================================
//		マクロを定義する場所
//	=================================================================
#define WINDOW_WIDTH					(1920)	//	ウィンドウの横幅
#define WINDOW_HEIGHT					(1080)	//	ウィンドウの縦幅

#define FPS								(60)	//	FPS

//	引数付きマクロ定義　変換マクロ
#define Deg2Rad(x) ( x * DX_PI_F / 180.0f )	//  デグリー角 -> ラジアン角
#define Rad2Deg(x) ( x * 180.0f / DX_PI_F )	//　ラジアン角 -> デグリー角

//  ワールドベクトル
#define VRight			(VGet(1.0f, 0.0f, 0.0f))	//  右
#define VLeft			(VGet(-1.0f, 0.0f, 0.0f))	//  左
#define VUp				(VGet(0.0f, 1.0f, 0.0f))	//  上
#define VDown			(VGet(0.0f, -1.0f, 0.0f))	//  下
#define VForward		(VGet(0.0f, 0.0f, 1.0f))	//  前
#define VBack			(VGet(0.0f, 0.0f, -1.0f))	//  後

//  ゼロベクトル
#define VZero			(VGet(0.0f, 0.0f, 0.0f))	//  ゼロベクトル
#define VOne			(VGet(1.0f, 1.0f, 1.0f))	//  全ての成分が1のベクトル

//  DxLibの読み込みエラー
#define INVALID			(-1)

//　地面の判定ライン
#define BOTTOM			(-100)	

//	=================================================================
//		定数を定義する場所
//	=================================================================
//	色
const unsigned int red = GetColor(255, 0, 0);			//  赤
const unsigned int green = GetColor(0, 255, 0);			//  緑
const unsigned int blue = GetColor(0, 0, 255);			//  青
const unsigned int magenta = GetColor(255, 0, 255);		//  マゼンタ
const unsigned int cyan = GetColor(0, 255, 255);		//  シアン
const unsigned int yellow = GetColor(255, 255, 0);		//  黄
const unsigned int white = GetColor(255, 255, 255);		//  白
const unsigned int black = GetColor(0, 0, 0);			//  黒
const unsigned int gray = GetColor(128, 128, 128);		//  灰

// FPS
const int FrameTime = 1000 / FPS;

//	=================================================================
//		列挙定数を定義する場所
//	=================================================================

/*
*	@brief	シーンの種類
*/
enum class SceneType {
	Boot,
	Title,
	Game,
};

/*
*  @brief	フェードの状態
*/
enum class FadeState {
	FadeIn = -1,
	FadeOut = 1,
	FadeEnd,
};

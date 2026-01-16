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

// ステージの座標
#define STAGE_POS		(VGet(4000.0f, -380.0f, 3500.0f))

// 引っ張りの最低値
#define MOUSE_PUSH_DOWN_MIN	(-100)

//  マウス感度
#define MOUSE_SENSITIVITY	(10)

// 大砲の戻ってくるときの速度
#define CANNON_RETURN_MOVE_SPEED	(0.1f)

// ボムの生成位置系
#define BOMB_GENERATE_POS_Y		(2000.0f)	//	生成されるY座標
#define BOMB_GENERATE_RAND_X	(1200.0f)	//  生成位置のランダム値X
#define BOMB_GENERATE_RAND_Z	(1000.0f)	//  生成位置のランダム値Z
#define BOMB_GENERATE_OFFSET_X	(600.0f)	//  生成位置の補正値X
#define BOMB_GENERATE_OFFSET_Z	(600.0f)	//  生成位置の補正値Z

// 弾、ボムの影のポリゴン
#define SHADOW_POLYGON			(32)

// ショットエフェクトの拡大率
#define SHOT_EFFECT_SCALE		(2.0f)
// 爆発エフェクトの拡大率
#define EXPLOSION_EFFECT_SCALE	(50.0f)
// クールタイムエフェクトの拡大率
#define COOL_TIME_EFFECT_SCALE	(100.0f)

// ニューレコードSEの音量
#define NEW_RECORD_SE_VOLUME	(0.7f)
// 爆発SEの音量
#define EXPLOSION_SE_VOLUME		(0.8f)
// スタートBGMの音量
#define START_BGM_VOLUME		(0.7f)
// ゲームBGMの音量
#define GAME_BGM_VOLUME			(0.7f)

// モデルパス
#define CANNON_MODEL_PATH		"Res/Model/Cannon.mv1"
#define CANNON_BASE_MODEL_PATH	"Res/Model/CannonBase.mv1"
#define BULLET_MODEL_PATH		"Res/Model/Bullet.mv1"
#define BOMB_MODEL_PATH			"Res/Model/Bomb.mv1"
#define STAGE_MODEL_PATH		"Res/Model/Stage00.mv1"
#define BACK_GROUND_MODEL_PATH	"Res/Model/Stage00_sky.mv1"
#define ARROW_MODEL_PATH		"Res/Model/Arrow.png"
#define EXIT_MODEL_PATH			"Res/Model/BombIrasuto.png"
#define MOUSE_MODEL_PATH		"Res/Model/mouse.png"
#define MOUSE_LEFT_MODEL_PATH	"Res/Model/mouse_left.png"
#define HEART_MODEL_PATH		"Res/Model/heart.png"
// エフェクトパス
#define SHOT_EFFECT_PATH		"Res/Effect/Simple_GeneratingPosition1.efkefc"
#define EXPLOSION_EFFECT_PATH	"Res/Effect/Simple_Sprite_BillBoard.efkefc"
#define COOL_TIME_EFFECT_PATH	"Res/Effect/Sylph5.efkefc"
// サウンドパス
#define SHOT_SE_PATH			"Res/Sound/maou_se_battle_explosion03.mp3"
#define EXPLOSION_SE_PATH		"Res/Sound/maou_se_battle_explosion05.mp3"
#define SCORE_SE_PATH			"Res/Sound/maou_se_system40.mp3"
#define GAME_OVER_SE_PATH		"Res/Sound/maou_se_onepoint29.mp3"
#define NEW_RECORD_SE_PATH		"Res/Sound/maou_se_jingle05.mp3"
#define START_BGM_PATH			"Res/Sound/maou_bgm_fantasy07.mp3"
#define GAME_BGM_PATH			"Res/Sound/maou_bgm_fantasy08.mp3"

// 名前
#define SHOT_NAME				"Shot"
#define EXPLOSION_NAME			"Explosion"
#define COOL_TIME_NAME			"CoolTIme"
#define SCORE_NAME				"Score"
#define GAME_OVER_NAME			"GameOver"
#define NEW_RECORD_NAME			"NewRecord"
#define START_BGM_NAME			"StartBGM"
#define GAME_BGM_NAME			"GameBGM"
#define OK_NAME					"OK"

// タグ
#define BOMB_TAG				"Bomb"


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

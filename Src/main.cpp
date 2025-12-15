//  ========================================================================
//        ヘッダーをインクルードする場所
//  ========================================================================
#include "Definition.h"
#include <vector>
#include <EffekseerForDXLib.h>

#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Manager/TimeManager.h"
#include "Manager/FadeManager.h"
#include "Manager/CollisionManager.h"
#include "Manager/GameManager.h"

#include "Manager/ScoreManager.h"

//  ========================================================================
//        関数を宣言、定義する場所
//  ========================================================================

/*
*   @brief		Windowプログラムのエントリーポイント
*   @param[in]	HINSTANCE	今回は不要
*   @param[in]	HINSTANCE	今回は不要
*   @param[in]	LPSTR		今回は不要
*   @param[in]	int			今回は不要
*   @return		int
*	@tips		WINAPI
*/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//  ========================================================================
	//        DxLibの初期化処理　　理解するまでは触らない
	//  ========================================================================
	//  ウィンドウのサイズを変更する
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, FPS);
	//  起動時のウィンドウのモードを設定する
	ChangeWindowMode(FALSE);		//  true : Windowモード, false : FullScreen
	//  背景色を設定する
	SetBackgroundColor(196, 196, 196);
	//  ウィンドウのタイトルを変更
	SetMainWindowText("Boom!!!");
	//  アイコンを変更
	SetWindowIconID(333);
	//  DirectX を使用します宣言
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//  DxLibの初期化
	if (DxLib_Init() == 1)
		return 0;

	//  Effekseerの初期化
	if (Effekseer_Init(8000) == -1) {
		DxLib_End();
		return 0;
	}

	//  描画する先を設定する	裏画面に変更する
	SetDrawScreen(DX_SCREEN_BACK);

	//  Zバッファを使用するかどうか		デフォルト : FALSE
	SetUseZBuffer3D(TRUE);
	//  Zバッファに書き込みを行うか		デフォルト : FALSE
	SetWriteZBuffer3D(TRUE);

	//  ライティング
	{
		//  ライトの計算をするかどうか		デフォルト : TRUE
		SetUseLighting(TRUE);
		//  標準ライトを使用するかどうか	デフォルト : TRUE
		SetLightEnable(TRUE);
		//  グローバル環境光の設定
		SetGlobalAmbientLight(GetColorF(0.2f, 0.2f, 0.2f, 0.2f));		//  ライトの計算で α値は使わない

		//  反射光の設定		Diffuse
		SetLightDifColor(GetColorF(0.2f, 0.2f, 0.2f, 1.0f));
		//  鏡面反射光の設定	Specular6
		//SetLightSpcColor(GetColorF(1.0f, 0.0f, 1.25f, 1.0f));
		//  環境光の設定		Ambient
		SetLightAmbColor(GetColorF(0.250f, 0.250f, 0.250f, 1.0f));
	}
	int light = CreateDirLightHandle(VGet(-0.5f, -0.6f, -0.25f));

	//  ========================================================================
	//        ゲームの初期化処理
	//  ========================================================================
	SceneManager::GetInstance()->LoadScene();

	//  ========================================================================
	//        ゲームのメインループ
	//  ========================================================================
	while (1) {
		//  FPSの調整
		int startTime = GetNowCount();

		//  ウィンドウのメッセージを処理する
		if (ProcessMessage() == -1)
			break;

		//  DxLibのカメラと Effkseerのカメラを同期する
		Effekseer_Sync3DSetting();

		//  マウスカーソルを表示
		SetMouseDispFlag(TRUE);

		//  ========================================================================
		//        マネージャーの更新処理
		//  ========================================================================
		InputManager::GetInstance()->Update();

#if _DEBUG
		//  エスケープキーでウィンドウを閉じる
		if (InputManager::GetInstance()->IsKeyDown(KEY_INPUT_ESCAPE))
			break;
#endif
		if (GameManager::GetInstance()->GetExitFlag())
			break;

		//  ========================================================================
		//        ゲームの更新処理
		//  ========================================================================
		TimeManager::GetInstance()->Update();
		FadeManager::GetInstance()->Update();
		SceneManager::GetInstance()->Update();

		//  画面をクリアする
		ClearDrawScreen();

		//  ========================================================================
		//        ゲームの描画処理	　処理順に注意
		//  ========================================================================

		SceneManager::GetInstance()->Render();
		FadeManager::GetInstance()->Render();
		TimeManager::GetInstance()->Render();

		//  裏画面と表画面を切り替える
		ScreenFlip();

		// FPSの調整
		int processTime = GetNowCount() - startTime;
		//  処理が速すぎた場合に待つ
		if (processTime < FrameTime) WaitTimer(FrameTime - processTime);
	}

	//  ========================================================================
	//        ゲームの解放処理
	//  ========================================================================

	//  シングルトンのインスタンスの削除
	InputManager::DestroyInstance();
	SceneManager::DestroyInstance();
	TimeManager::DestroyInstance();
	FadeManager::DestroyInstance();
	//  当たり判定の登録も初期化
	//CollisionManager::DestroyInstance();

	//  ========================================================================
	//        DxLibの解放処理
	//  ========================================================================

	DeleteLightHandle(light);

	InitGraph();

	Effkseer_End();
	//  DxLibの終了
	DxLib_End();

	return 0;
}
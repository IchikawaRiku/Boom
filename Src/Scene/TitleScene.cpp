#include "TitleScene.h"
#include "../Definition.h"

#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/FadeManager.h"
#include "../Manager/GameManager.h"
#include "../Manager/TimeManager.h"

#include "../Component/Collider.h"

#include "../GameObject/Bullet/Bullet.h"
#include "../GameObject/Bomb/Bomb.h"
#include "../GameObject/Camera/Camera.h"
#include "../GameObject/Cannon/Cannon.h"
#include "../GameObject/CannonBase/CannonBase.h"
#include "../GameObject/Effect/Effect.h"
#include "../GameObject/Audio/Audio.h"
#include "../GameObject/Stage/Stage.h"


/*
*	@brief	コンストラクタ
*/
TitleScene::TitleScene()
	: changed(false)
	, fadeCount(0.0f)
	, countUp(false)
	, blinking(0.0f)
	, start(false)
	, click(false) {
	Start();
}

/*
*	@brief	デストラクタ
*/
TitleScene::~TitleScene() {
	DeleteFontToHandle(UltoraBigFont);
	DeleteFontToHandle(BigFont);
	DeleteFontToHandle(SmallFont);
	InitGraph();
}

/*
*   @function	Start
*	@brief		初期化処理
*/
void TitleScene::Start() {
	GameManager::GetInstance()->SetGameOverFlag(false);
	SetUseASyncLoadFlag(TRUE);
	//  モデルの読み込み
	cannonModel = MV1LoadModel("Res/Model/Cannon.mv1");
	cannonBaseModel = MV1LoadModel("Res/Model/CannonBase.mv1");
	stageModel = MV1LoadModel("Res/Model/Stage00.mv1");
	backGroundModel = MV1LoadModel("Res/Model/Stage00_sky.mv1");
	arrowModel = LoadGraph("Res/Model/Arrow.png");
	exitModel = LoadGraph("Res/Model/BombIrasuto.png");

	GameManager::GetInstance()->SetArrowModel(arrowModel);
	GameManager::GetInstance()->SetExitModel(exitModel);



	//  ステージのインスタンス化
	Stage* pStage = new Stage(VGet(4000.0f, -380.0f, 3500.0f));
	//  ステージモデルデータのセット
	pStage->SetModelHandle(stageModel);
	//  背景画像のセット
	pStage->SetSkyModel(backGroundModel);
	//  一元管理する配列に追加
	pGameObjectArray.push_back(pStage);

	//  爆弾の初期化
	BombManager::GetInstance()->TitleStart();

	//  弾の初期化
	BulletManager::GetInstance()->Start();

	//  カメラのインスタンス化
	Camera* pCamera = new Camera(VGet(-600.0f, 10.0f, -400.0f));
	pCamera->SetRotation(-12, 30, 0);
	//  一元管理する配列に追加
	pGameObjectArray.push_back(pCamera);

	//  大砲のインスタンス化
	Cannon* pCannon = new Cannon(VGet(0.0f, 0.0f, 0.0f));
	//  大砲のモデルデータのセット
	pCannon->SetModelHandle(cannonModel);
	//  一元管理する配列に追加
	pGameObjectArray.push_back(pCannon);

	//  大砲の土台のインスタンス化
	CannonBase* pCannonBase = new CannonBase(VGet(0.0f, 0.0f, 0.0f));
	//  大砲の土台のモデルデータのセット
	pCannonBase->SetModelHandle(cannonBaseModel);
	//  一元管理する配列に追加
	pGameObjectArray.push_back(pCannonBase);

	SetUseASyncLoadFlag(FALSE);

	//  エフェクトの読み込み
	EffectManager::GetInstance()->Load("Res/Effect/Simple_GeneratingPosition1.efkefc", "Shot", 2.0f);
	EffectManager::GetInstance()->Load("Res/Effect/Simple_Sprite_BillBoard.efkefc", "Explosion", 50.0f);


	// 音の読み込み
	AudioManager::GetInstance()->Load("Res/Sound/maou_bgm_fantasy07.mp3", "StartBGM", true);
	AudioManager::GetInstance()->Load("Res/Sound/maou_se_battle_explosion03.mp3", "Shot", true);
	AudioManager::GetInstance()->Load("Res/Sound/maou_se_battle_explosion05.mp3", "Explosion", true);

	//  BGMの再生
	AudioManager::GetInstance()->PlayOneShot("StartBGM", 0.7f, true);

}

/*
*   @function	Update
*	@brief		更新処理
*	@tips		純粋仮想関数で実装
*/
void TitleScene::Update() {
	//  スタートの合図があれば
	if (BombManager::GetInstance()->GetGameStart() && !click) {
		countUp = true;
		start = true;
		click = true;
		AudioManager::GetInstance()->PlayOneShot("OK");
	}
	//  １秒待ってフェード開始
	if (countUp)
		fadeCount += TimeManager::GetInstance()->GetDeltaTime();
	if (fadeCount >= 1) {
		countUp = false;
		fadeCount = 0;
		changed = true;
		FadeManager::GetInstance()->FadeOut();
	}
	//  フェードが終わったら開始
	if (changed && FadeManager::GetInstance()->GetFadeState() == FadeState::FadeEnd) {
		SceneManager::GetInstance()->SetNextScene(SceneType::Game);
		FadeManager::GetInstance()->FadeIn();
	}

	//　一元管理配列に登録されているオブジェクトの更新
	for (auto pGameObject : pGameObjectArray) {
		pGameObject->Update();
	}
	//　スコアの更新
	ScoreManager::GetInstance()->Update();
	//  爆弾の更新
	BombManager::GetInstance()->TitleUpdate();
	//　弾の更新
	BulletManager::GetInstance()->TitleUpdate();
	//　エフェクトの更新
	EffectManager::GetInstance()->Update();
	//  ゲーム進行関連の更新
	GameManager::GetInstance()->TitleUpdate();
	//  全てのオブジェクトの更新が終わってから当たり判定の更新
	CollisionManager::GetInstance()->Update();
}

/*
*   @function	Render
*	@brief		描画処理
*	@tips		純粋仮想関数で実装
*/
void TitleScene::Render() {
	//　一元管理配列に登録されているオブジェクトの更新
	for (auto pGameObject : pGameObjectArray) {
		pGameObject->Render();
	}
	//  弾の描画
	BulletManager::GetInstance()->Render();
	//  弾の描画
	BombManager::GetInstance()->TitleRender();
	//  スコアの描画
	//ScoreManager::GetInstance()->Render();
	//　エフェクトの描画
	EffectManager::GetInstance()->Render();

	DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle("Boom!!!", 7, UltoraBigFont)) / 2,
		WINDOW_HEIGHT / 4, red, UltoraBigFont, "Boom!!!");
	//  点滅
	blinking += TimeManager::GetInstance()->GetDeltaTime();
	if (blinking > 0.5f && !GameManager::GetInstance()->GetArrowFlag() && !start) {
		DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle("Push!", 5, BigFont)) / 2,
			WINDOW_HEIGHT * 0.55, red, BigFont, "Push!");
		if (blinking > 1)
			blinking = 0;
	}
	else if (blinking > 0.5f &&
		!(InputManager::GetInstance()->GetMouseMoveValueY() < -100) && !start) {
		DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle("Down!", 5, BigFont)) / 2,
			WINDOW_HEIGHT * 0.55, red, BigFont, "Down!");
		if (blinking > 1)
			blinking = 0;
	}
	else if (blinking > 0.5f && !start) {
		DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle("Release!", 8, BigFont)) / 2,
			WINDOW_HEIGHT * 0.55, red, BigFont, "Release!");
		if (blinking > 1)
			blinking = 0;
	}
	//　ゲーム進行関連の描画
	GameManager::GetInstance()->Render();

	DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle("Exit", 4, SmallFont)) * 0.095f,
		WINDOW_HEIGHT * 0.8f, yellow, SmallFont, "Exit");
}
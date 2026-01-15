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
	, click(false)
	, CAMERA_POS(VGet(-600.0f, 10.0f, -400.0f))
	, CAMERA_ROT(VGet(- 12, 30, 0))
	, TEXT_POS_Y_RATIO(0.55f)
	, EXIT_TEXT_POS_X_RATIO(0.095f)
	, EXIT_TEXT_POS_Y_RATIO(0.8f)
	, BOOM_TEXT("Boom!!!")
	, PUSH_TEXT("Push!")
	, DOWN_TEXT("Down!")
	, RELEASE_TEXT("Release!")
	, EXIT_TEXT("Exit")
{
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
	cannonModel = MV1LoadModel(CANNON_MODEL_PATH);
	cannonBaseModel = MV1LoadModel(CANNON_BASE_MODEL_PATH);
	stageModel = MV1LoadModel(STAGE_MODEL_PATH);
	backGroundModel = MV1LoadModel(BACK_GROUND_MODEL_PATH);
	arrowModel = LoadGraph(ARROW_MODEL_PATH);
	exitModel = LoadGraph(EXIT_MODEL_PATH);

	GameManager::GetInstance()->SetArrowModel(arrowModel);
	GameManager::GetInstance()->SetExitModel(exitModel);



	//  ステージのインスタンス化
	Stage* pStage = new Stage(STAGE_POS);
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
	Camera* pCamera = new Camera(CAMERA_POS);
	pCamera->SetRotation(CAMERA_ROT);
	//  一元管理する配列に追加
	pGameObjectArray.push_back(pCamera);

	//  大砲のインスタンス化
	Cannon* pCannon = new Cannon(VZero);
	//  大砲のモデルデータのセット
	pCannon->SetModelHandle(cannonModel);
	//  一元管理する配列に追加
	pGameObjectArray.push_back(pCannon);

	//  大砲の土台のインスタンス化
	CannonBase* pCannonBase = new CannonBase(VZero);
	//  大砲の土台のモデルデータのセット
	pCannonBase->SetModelHandle(cannonBaseModel);
	//  一元管理する配列に追加
	pGameObjectArray.push_back(pCannonBase);

	SetUseASyncLoadFlag(FALSE);

	//  エフェクトの読み込み
	EffectManager::GetInstance()->Load(SHOT_EFFECT_PATH, SHOT_NAME, SHOT_EFFECT_SCALE);
	EffectManager::GetInstance()->Load(EXPLOSION_EFFECT_PATH, EXPLOSION_NAME, EXPLOSION_EFFECT_SCALE);


	// 音の読み込み
	AudioManager::GetInstance()->Load(START_BGM_PATH, START_BGM_NAME, true);
	AudioManager::GetInstance()->Load(SHOT_SE_PATH, SHOT_NAME, true);
	AudioManager::GetInstance()->Load(EXPLOSION_SE_PATH, EXPLOSION_NAME, true);

	//  BGMの再生
	AudioManager::GetInstance()->PlayOneShot(START_BGM_NAME, START_BGM_VOLUME, true);

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
		AudioManager::GetInstance()->PlayOneShot(OK_NAME);
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

	DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle(BOOM_TEXT, _tcslen(BOOM_TEXT), UltoraBigFont)) / 2,
		WINDOW_HEIGHT / 4, red, UltoraBigFont, BOOM_TEXT);
	//  点滅
	blinking += TimeManager::GetInstance()->GetDeltaTime();
	if (blinking > 0.5f && !GameManager::GetInstance()->GetArrowFlag() && !start) {
		DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle(PUSH_TEXT, _tcslen(PUSH_TEXT), BigFont)) / 2,
			WINDOW_HEIGHT * TEXT_POS_Y_RATIO, red, BigFont, PUSH_TEXT);
		if (blinking > 1)
			blinking = 0;
	}
	else if (blinking > 0.5f &&
		!(InputManager::GetInstance()->GetMouseMoveValueY() < MOUSE_PUSH_DOWN_MIN) && !start) {
		DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle(DOWN_TEXT, _tcslen(DOWN_TEXT), BigFont)) / 2,
			WINDOW_HEIGHT * TEXT_POS_Y_RATIO, red, BigFont, DOWN_TEXT);
		if (blinking > 1)
			blinking = 0;
	}
	else if (blinking > 0.5f && !start) {
		DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle(RELEASE_TEXT, _tcslen(RELEASE_TEXT), BigFont)) / 2,
			WINDOW_HEIGHT * TEXT_POS_Y_RATIO, red, BigFont, RELEASE_TEXT);
		if (blinking > 1)
			blinking = 0;
	}
	//　ゲーム進行関連の描画
	GameManager::GetInstance()->Render();

	DrawFormatStringToHandle((WINDOW_WIDTH - GetDrawStringWidthToHandle(EXIT_TEXT, _tcslen(EXIT_TEXT), SmallFont)) * EXIT_TEXT_POS_X_RATIO,
		WINDOW_HEIGHT * EXIT_TEXT_POS_Y_RATIO, yellow, SmallFont, EXIT_TEXT);
}
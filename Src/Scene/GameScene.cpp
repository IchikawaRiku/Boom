#include "GameScene.h"

#include "../Definition.h"

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
GameScene::GameScene()
	: CAMERA_POS(VGet(0.0f, 470.0f, -1000.0f)){
	Start();
}

/*
*	@brief	デストラクタ
*/
GameScene::~GameScene() {
	//  当たり判定の登録も初期化
	CollisionManager::GetInstance()->UnregisterAll();
	InitGraph();
}

/*
*   @function	Start
*	@brief		初期化処理
*/
void GameScene::Start() {
	//  モデルの読み込み
	cannonModel = MV1LoadModel(CANNON_MODEL_PATH);
	cannonBaseModel = MV1LoadModel(CANNON_BASE_MODEL_PATH);
	stageModel = MV1LoadModel(STAGE_MODEL_PATH);
	backGroundModel = MV1LoadModel(BACK_GROUND_MODEL_PATH);
	mouseModel = LoadGraph(MOUSE_MODEL_PATH);
	mouseLeftModel = LoadGraph(MOUSE_LEFT_MODEL_PATH);
	heartModel = LoadGraph(HEART_MODEL_PATH);

	GameManager::GetInstance()->SetMouseModel(mouseModel);
	GameManager::GetInstance()->SetMouseLeftModel(mouseLeftModel);
	GameManager::GetInstance()->SetHeartModel(heartModel);
	
	//  エフェクトの読み込み
	EffectManager::GetInstance()->Load(COOL_TIME_EFFECT_PATH, COOL_TIME_NAME, COOL_TIME_EFFECT_SCALE);
	EffectManager::GetInstance()->Load(SHOT_EFFECT_PATH, SHOT_NAME, SHOT_EFFECT_SCALE);
	EffectManager::GetInstance()->Load(EXPLOSION_EFFECT_PATH, EXPLOSION_NAME, EXPLOSION_EFFECT_SCALE);

	//  サウンドの読み込み
	AudioManager::GetInstance()->Load(EXPLOSION_SE_PATH, EXPLOSION_NAME, true);
	AudioManager::GetInstance()->Load(SHOT_SE_PATH, SHOT_NAME, true);
	AudioManager::GetInstance()->Load(SCORE_SE_PATH, SCORE_NAME, true);
	AudioManager::GetInstance()->Load(GAME_OVER_SE_PATH, GAME_OVER_NAME, true);
	AudioManager::GetInstance()->Load(NEW_RECORD_SE_PATH, NEW_RECORD_NAME, true);
	AudioManager::GetInstance()->Load(GAME_BGM_PATH, GAME_BGM_NAME, true);

	//  ステージのインスタンス化
	Stage* pStage = new Stage(STAGE_POS);
	//  ステージモデルデータのセット
	pStage->SetModelHandle(stageModel);
	//  背景画像のセット
	pStage->SetSkyModel(backGroundModel);
	//  一元管理する配列に追加
	pGameObjectArray.push_back(pStage);

	//  爆弾の初期化
	BombManager::GetInstance()->Start();

	//  弾の初期化
	BulletManager::GetInstance()->Start();

	//  カメラのインスタンス化
	Camera* pCamera = new Camera(CAMERA_POS);
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

	//  BGMの再生
	AudioManager::GetInstance()->PlayOneShot(GAME_BGM_NAME, GAME_BGM_VOLUME, true);

}

/*
*   @function	Update
*	@brief		更新処理
*	@tips		純粋仮想関数で実装
*/
void GameScene::Update() {	
	//　一元管理配列に登録されているオブジェクトの更新
	for (auto pGameObject : pGameObjectArray) {
		pGameObject->Update();
	}
	//　スコアの更新
	ScoreManager::GetInstance()->Update();
	//　弾の更新
	BulletManager::GetInstance()->Update();
	//　爆弾の更新
	BombManager::GetInstance()->Update();
	//　エフェクトの更新
	EffectManager::GetInstance()->Update();
	//  ゲーム進行関連の更新
	GameManager::GetInstance()->Update();
	//  全てのオブジェクトの更新が終わってから当たり判定の更新
	CollisionManager::GetInstance()->Update();
}

/*
*   @function	Render
*	@brief		描画処理
*	@tips		純粋仮想関数で実装
*/
void GameScene::Render() {
	//　一元管理配列に登録されているオブジェクトの更新
	for (auto pGameObject : pGameObjectArray) {
		pGameObject->Render();
	}
	//  弾の描画
	BulletManager::GetInstance()->Render();
	//  爆弾の描画
	BombManager::GetInstance()->Render();
	//  スコアの描画
	ScoreManager::GetInstance()->Render();
	//　ゲーム進行関連の描画
	GameManager::GetInstance()->Render();
	//　エフェクトの描画
	EffectManager::GetInstance()->Render();

}

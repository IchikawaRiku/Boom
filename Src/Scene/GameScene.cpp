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
GameScene::GameScene(){
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
	cannonModel = MV1LoadModel("Res/Model/Cannon.mv1");
	cannonBaseModel = MV1LoadModel("Res/Model/CannonBase.mv1");
	stageModel = MV1LoadModel("Res/Model/Stage00.mv1");
	backGroundModel = MV1LoadModel("Res/Model/Stage00_sky.mv1");
	mouseModel = LoadGraph("Res/Model/mouse.png");
	mouseLeftModel = LoadGraph("Res/Model/mouse_left.png");
	heartModel = LoadGraph("Res/Model/heart.png");

	GameManager::GetInstance()->SetMouseModel(mouseModel);
	GameManager::GetInstance()->SetMouseLeftModel(mouseLeftModel);
	GameManager::GetInstance()->SetHeartModel(heartModel);
	
	//  エフェクトの読み込み
	EffectManager::GetInstance()->Load("Res/Effect/Sylph5.efkefc", "CoolTime", 100.0f);
	EffectManager::GetInstance()->Load("Res/Effect/Simple_GeneratingPosition1.efkefc", "Shot", 2.0f);
	EffectManager::GetInstance()->Load("Res/Effect/Simple_Sprite_BillBoard.efkefc", "Explosion", 50.0f);

	//  サウンドの読み込み
	AudioManager::GetInstance()->Load("Res/Sound/maou_se_battle_explosion05.mp3", "Explosion", true);
	AudioManager::GetInstance()->Load("Res/Sound/maou_se_battle_explosion03.mp3", "Shot", true);
	AudioManager::GetInstance()->Load("Res/Sound/maou_se_system40.mp3", "Score", true);
	AudioManager::GetInstance()->Load("Res/Sound/maou_se_onepoint29.mp3", "GameOver", true);
	AudioManager::GetInstance()->Load("Res/Sound/maou_se_jingle05.mp3", "NewRecord", true);
	AudioManager::GetInstance()->Load("Res/Sound/maou_bgm_fantasy08.mp3", "GameBGM", true);

	//  ステージのインスタンス化
	Stage* pStage = new Stage(VGet(4000.0f, -380.0f, 3500.0f));
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
	Camera* pCamera = new Camera(VGet(0.0f, 470.0f, -1000.0f));
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

	//  BGMの再生
	AudioManager::GetInstance()->PlayOneShot("GameBGM", 0.7f, true);

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

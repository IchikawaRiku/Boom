#include "BombManager.h"
#include "InputManager.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "TimeManager.h"

#pragma region シングルトン

//  静的メンバ変数の初期化
BombManager* BombManager::pInstance = nullptr;

/*
*   @brief		コンストラクタ
*   @tips		外部で生成されないようにアクセス指定子を private にする
*/
BombManager::BombManager()
	: CoolTimeFlag(false)
	, CoolTime(0)
	, Generate(false)
	, BreakFlag(false)
	, BOMB_MAX(10)
	, COOLTIME_MAX(1)
	, Time(0)
	, GameStart(false) 
	, bombCount(0){
	for (int i = 0; i < 3; i++)
		pStartBomb[i] = nullptr;
}

/*
*   @function	CreateInstance
*   @brief		自身のインスタンスを生成する
*/
void BombManager::CreateInstance() {
	pInstance = new BombManager();
}

/*
*   @function	GetInstance
*   @brief		自身のインスタンスを取得する唯一の手段
*   @return		BombManager*	自身のインスタンスのアドレス
*   @tips		生成は１度だけ行う
*/
BombManager* BombManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

/*
*   @function	DestroyInstance
*   @brief		自身のインスタンスを破棄する唯一の手段
*/
void BombManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

#pragma endregion

/*
*   @function   Start
*   @brief		初期化処理
*/
void BombManager::Start() {
	//  モデルの読み込み
	bombModel = MV1LoadModel("Res/Model/Bomb.mv1");

	//  爆弾のインスタンス化
	for (int i = 0; i < 5; i++) {
		pBomb.push_back(new Bomb(MV1DuplicateModel(bombModel)
			, VGet(GetRand(1200.0f) - 600.0f, 2000.0f, GetRand(1000.0f) + 600.0f)));
	}
}

/*
*   @function   TitleStart
*   @brief		タイトルの初期化処理
*/
void BombManager::TitleStart() {
	//  モデルの読み込み
	bombModel = MV1LoadModel("Res/Model/Bomb.mv1");

	for (int i = 0; i < 3; i++)
		pStartBomb[i] = new Bomb(MV1DuplicateModel(bombModel), VGet(-400, 0, 1800));
}

/*
*   @function   Update
*   @brief		更新処理
*/
void BombManager::Update() {
	//  爆弾の更新
	for (auto pGameObject : pBomb) {
		pGameObject->Update();
	}
	//  時間を計測
	Time += TimeManager::GetInstance()->GetDeltaTime();

	//  30秒たったら1つ生成
	if (Time > 15) {
		//Bomb* bom = new Bomb(MV1DuplicateModel(bombModel)
		//	, VGet(GetRand(1200.0f) - 600.0f, 2000.0f, GetRand(1000.0f) + 600.0f));
		//pBomb.push_back(bom);
		//bom->SetVisible(false);
		pBomb.push_back(new Bomb(MV1DuplicateModel(bombModel)
			, VGet(GetRand(1200.0f) - 600.0f, 2000.0f, GetRand(1000.0f) + 600.0f)));
		Time = 0;
	}
}

/*
*   @function   TitleUpdate
*   @brief		タイトルの更新処理
*/
void BombManager::TitleUpdate() {
	if (!GameStart) {
		pStartBomb[0]->SetPosition(VGet(0, 200, 400)); 
		pStartBomb[1]->SetPosition(VGet(0, 300, 800)); 
		pStartBomb[2]->SetPosition(VGet(0, 250, 1200)); 
		for (int i = 0; i < bombCount; i++) {
			pStartBomb[i]->SetVisible(true);
		}
	}
	for (int i = 0; i < 3; i++)
		pStartBomb[i]->TitleUpdate();
}

/*
*   @function   Render
*   @brief		描画処理
*/
void BombManager::Render() {
	//  爆弾の描画
	for (auto pGameObject : pBomb) {
		pGameObject->Render();
	}
}

/*
*   @function   TitleRender
*   @brief		タイトルの描画処理
*/
void BombManager::TitleRender() {
	for (int i = 0; i < 3; i++)
		pStartBomb[i]->Render();
}

/*
*   @function   Delete
*   @brief		解放処理
*/
void BombManager::Delete() {
	//  弾の解放処理
	for (auto pGameObject : pBomb) {
		delete pGameObject;
		pGameObject = nullptr;
	}
}

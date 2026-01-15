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
	, Time(0)
	, GameStart(false) 
	, bombCount(0)
	, BOMB_MAX(10)
	, COOLTIME_MAX(1)
	, TITLE_BOMB(3)
	, START_BOMB(5)
	, GENERATE_POS_Y(2000.0f)
	, GENERATE_RAND_X(1200.0f)
	, GENERATE_RAND_Z(1000.0f)
	, GENERATE_OFFSET_X(600.0f)
	, GENERATE_OFFSET_Z(600.0f)
	, GENERATE_COOLTIME_MAX(15)
	, TITLE_GENERATE_BASE_POS(VGet(-400, 0, 1800))
	, TITLE_GENERATE_POS_1(VGet(0, 200, 400))
	, TITLE_GENERATE_POS_2(VGet(0, 300, 800))
	, TITLE_GENERATE_POS_3(VGet(0, 250, 1200))
{
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
	bombModel = MV1LoadModel(BOMB_MODEL_PATH);

	//  爆弾のインスタンス化
	for (int i = 0; i < START_BOMB; i++) {
		pBomb.push_back(new Bomb(MV1DuplicateModel(bombModel)
			, VGet(GetRand(GENERATE_RAND_X) - GENERATE_OFFSET_X, 
				GENERATE_POS_Y, 
				GetRand(GENERATE_RAND_Z) + GENERATE_OFFSET_Z)));
	}
}

/*
*   @function   TitleStart
*   @brief		タイトルの初期化処理
*/
void BombManager::TitleStart() {
	//  モデルの読み込み
	bombModel = MV1LoadModel(BOMB_MODEL_PATH);

	for (int i = 0; i < TITLE_BOMB; i++)
		pStartBomb[i] = new Bomb(MV1DuplicateModel(bombModel), TITLE_GENERATE_BASE_POS);
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

	//  クールタイムが明けたら1つ生成
	if (Time > GENERATE_COOLTIME_MAX) {
		pBomb.push_back(new Bomb(MV1DuplicateModel(bombModel)
			, VGet(GetRand(GENERATE_RAND_X) - GENERATE_OFFSET_X,
				GENERATE_POS_Y,
				GetRand(GENERATE_RAND_Z) + GENERATE_OFFSET_Z)));
		Time = 0;
	}
}

/*
*   @function   TitleUpdate
*   @brief		タイトルの更新処理
*/
void BombManager::TitleUpdate() {
	if (!GameStart) {
		pStartBomb[0]->SetPosition(TITLE_GENERATE_POS_1);
		pStartBomb[1]->SetPosition(TITLE_GENERATE_POS_2);
		pStartBomb[2]->SetPosition(TITLE_GENERATE_POS_3);
		for (int i = 0; i < bombCount; i++) {
			pStartBomb[i]->SetVisible(true);
		}
	}
	for (int i = 0; i < TITLE_BOMB; i++)
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

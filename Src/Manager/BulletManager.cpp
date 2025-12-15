#include "BulletManager.h"
#include "InputManager.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "TimeManager.h"
#include "CollisionManager.h"
#include "AudioManager.h"
#include "BombManager.h"

#pragma region シングルトン

//  静的メンバ変数の初期化
BulletManager* BulletManager::pInstance = nullptr;

/*
*   @brief		コンストラクタ
*   @tips		外部で生成されないようにアクセス指定子を private にする
*/
BulletManager::BulletManager()
	: Down(false)
	, CoolTimeFlag(false)
	, CoolTime(0)
	, TitleCoolTime(0)
	, TitleDown(false)
	, TitleUp(false)
	, Generate(false)
	, bulletModel(INVALID)
	, BreakFlag(false)
	, BULLET_MAX(10)
	, COOLTIME_MAX(0.3f)
	, TITLE_COOLTIME_MAX(2)
	, Release(false)
	, DownMouseX(0)
	, DownMouseY(0)
	, MouseX(0)
	, MouseY(0)
	, MouseButton(false)
	, ShotBullet(nullptr){
}

/*
*   @brief		デストラクタ
*/
BulletManager::~BulletManager() {
	//  当たり判定の登録も初期化
	CollisionManager::GetInstance()->UnregisterAll();
}

/*
*   @function	CreateInstance
*   @brief		自身のインスタンスを生成する
*/
void BulletManager::CreateInstance() {
	pInstance = new BulletManager();
}

/*
*   @function	GetInstance
*   @brief		自身のインスタンスを取得する唯一の手段
*   @return		BulletManager*	自身のインスタンスのアドレス
*   @tips		生成は１度だけ行う
*/
BulletManager* BulletManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

/*
*   @function	DestroyInstance
*   @brief		自身のインスタンスを破棄する唯一の手段
*/
void BulletManager::DestroyInstance() {
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
void BulletManager::Start() {
	//  モデルの読み込み
	bulletModel = MV1LoadModel("Res/Model/Bullet.mv1");
	//　弾を1つインスタンス化
	pBullet.push_back(new Bullet(MV1DuplicateModel(bulletModel)));
}

/*
*   @function   Update
*   @brief		更新処理
*/
void BulletManager::Update() {
	//  ゲームオーバーならスキップ
	if (GameManager::GetInstance()->GetGameOverFlag()) {
		for (auto pGameObject : pBullet) {
			pGameObject->SetVisible(false);
		}
		return;
	}

	//  入力管理クラスの取得
	InputManager* input = InputManager::GetInstance();

	//  クールタイムが終わったら弾を出す
	if (CoolTime >= COOLTIME_MAX) {
		Release = false;
		//  弾のVisibleがfalseの弾があればtrueに,なければ弾生成用のフラグをtrueに
		for (auto pGameObject : pBullet) {
			if (!pGameObject->IsVisible()) {
				//　初期位置に弾がセットされていたらスキップ
				for (auto G : pBullet) {
					if (G->IsVisible() && input->SameVec(G->GetPosition(), VZero))
						BreakFlag = true;
				}
				if (BreakFlag) {
					BreakFlag = false;
					break;
				}
				//　弾をfalseからtrueに
				pGameObject->SetVisible(true);
				Generate = false;
				CoolTimeFlag = false;
				CoolTime = 0;
				break;
			}
			//  trueの弾が初期位置じゃなかったら
			else if (!input->SameVec(pGameObject->GetPosition(), VZero)) {
				//　初期位置に弾がセットされていたらスキップ
				for (auto pG : pBullet) {
					if (pG->IsVisible() && input->SameVec(pG->GetPosition(), VZero)) {
						BreakFlag = true;
					}
				}
				if (BreakFlag) {
					BreakFlag = false;
					break;
				}
				//　弾生成フラグをtrue
				Generate = true;
				CoolTimeFlag = false;
				CoolTime = 0;
				break;
			}
		}
	}

	//  弾のインスタンス化
	if (Generate) {
		pBullet.push_back(new Bullet(MV1DuplicateModel(bulletModel)));
		Generate = false;
		CoolTime = 0.0f;
	}

	//  マウスが押されたら弾を出す
	if (input->IsMouseButtonDown()) {
		Down = true;
	}
	if (Down && input->IsMouseButtonUp()) {
		CoolTimeFlag = true;
		Down = false;
	}

	//  クールタイムの更新
	if (CoolTimeFlag)
		CoolTime += TimeManager::GetInstance()->GetDeltaTime();

	//  マウスの位置を取得
	GetMousePoint(&MouseX, &MouseY);
	// マウスのボタンが押されたり離されたりしたかどうかの情報を取得する
	if (!Release) {
		if (input->IsMouseButtonDown()) {
			DownMouseX = MouseX;
			DownMouseY = MouseY;
			MouseButton = true;
		}
		if (input->IsMouseButtonUp()) {
			if (MouseButton) {
				Release = true;
				//  エフェクト再生
				EffectManager::GetInstance()->Instantiate("CoolTime", VGet(0, -100, 0));
			}
			MouseButton = false;
			CoolTime = 0.0f;
		}
	}

	//  初期位置にセットされているか
	for (auto pGameObject : pBullet) {
		if (pGameObject->IsVisible() && input->SameVec(pGameObject->GetPosition(), VZero)) {
			ShotBullet = pGameObject;
		}
	}
	if (MouseButton) {
		ShotBullet->SetRotation((DownMouseY - MouseY) / 10,
			(DownMouseX - MouseX) / 10,
			ShotBullet->GetRotation().z);
	}

	//  弾が放たれたらリセット
	if (Release) {
		DownMouseX = 0;
		DownMouseY = 0;
		MouseX = 0;
		MouseY = 0;
	}

	//  弾の更新
	for (auto pGameObject : pBullet) {
		pGameObject->Update();
	}
}

/*
*   @function   TitleUpdate
*   @brief		タイトル画面用の更新処理
*/
void BulletManager::TitleUpdate() {
#if 0
	//  入力管理クラスの取得
	InputManager* input = InputManager::GetInstance();

	MouseButton = true;
	DownMouseY = 250;
	MouseY = 500;

	//  クールタイムが終わったら弾を出す
	//if (CoolTime >= COOLTIME_MAX) {
	//	Release = false;
	//	//  弾のVisibleがfalseの弾があればtrueに,なければ弾生成用のフラグをtrueに
	//	for (auto pGameObject : pBullet) {
	//		if (!pGameObject->IsVisible()) {
	//			//　初期位置に弾がセットされていたらスキップ
	//			for (auto G : pBullet) {
	//				if (G->IsVisible() && input->SameVec(G->GetPosition(), VZero))
	//					BreakFlag = true;
	//			}
	//			if (BreakFlag) {
	//				BreakFlag = false;
	//				break;
	//			}
	//			//　弾をfalseからtrueに
	//			pGameObject->SetVisible(true);
	//			Generate = false;
	//			CoolTimeFlag = false;
	//			CoolTime = 0;
	//			break;
	//		}
	//		//  trueの弾が初期位置じゃなかったら
	//		else if (!input->SameVec(pGameObject->GetPosition(), VZero)) {
	//			//　初期位置に弾がセットされていたらスキップ
	//			for (auto pG : pBullet) {
	//				if (pG->IsVisible() && input->SameVec(pG->GetPosition(), VZero)) {
	//					BreakFlag = true;
	//				}
	//			}
	//			if (BreakFlag) {
	//				BreakFlag = false;
	//				break;
	//			}
	//			//　弾生成フラグをtrue
	//			Generate = true;
	//			CoolTimeFlag = false;
	//			CoolTime = 0;
	//			break;
	//		}
	//	}
	//}

	//  弾のインスタンス化
	//if (Generate) {
	//	pBullet.push_back(new Bullet(MV1DuplicateModel(bulletModel)));
	//	Generate = false;
	//	CoolTime = 0.0f;
	//}

	//  マウスが押されたら弾を出す
	if (input->IsMouseButtonDown()) {
		Down = true;
	}
	if (Down && input->IsMouseButtonUp()) {
		//CoolTimeFlag = true;
		Down = false;
	}

	//  クールタイムの更新
	//if (CoolTimeFlag)
	//	CoolTime += TimeManager::GetInstance()->GetDeltaTime();

	// マウスのボタンが押されたり離されたりしたかどうかの情報を取得する
	if (!Release) {
		if (input->IsMouseButtonDown()) {
			//DownMouseX = MouseX;
			//DownMouseY = MouseY;
			MouseButton = true;
		}
		if (input->IsMouseButtonUp()) {
			if (MouseButton) {
				Release = true;
			}
			MouseButton = false;
			AudioManager::GetInstance()->Stop("All");
			//CoolTime = 0.0f;
		}
	}

	//  初期位置にセットされているか
	for (auto pGameObject : pBullet) {
		if (pGameObject->IsVisible() && input->SameVec(pGameObject->GetPosition(), VZero)) {
			ShotBullet = pGameObject;
		}
	}
	if (MouseButton) {
		ShotBullet->SetRotation((DownMouseY - MouseY) / 10, 0, ShotBullet->GetRotation().z);
	}

	//  弾が放たれたらリセット
	//if (Release) {
	//	DownMouseX = 0;
	//	DownMouseY = 0;
	//	MouseX = 0;
	//	MouseY = 0;
	//}

	//  弾の更新
	for (auto pGameObject : pBullet) {
		pGameObject->Update();
	}

	/*
	//  入力管理クラスの取得
	InputManager* input = InputManager::GetInstance();

	MouseButton = true;
	DownMouseY = 500;
	MouseY = 1000;

	//  クールタイムが終わったら弾を出す
	if (CoolTime >= TITLE_COOLTIME_MAX) {
		Release = false;
		//  弾のVisibleがfalseの弾があればtrueに,なければ弾生成用のフラグをtrueに
		for (auto pGameObject : pBullet) {
			if (!pGameObject->IsVisible()) {
				//　初期位置に弾がセットされていたらスキップ
				for (auto G : pBullet) {
					if (G->IsVisible() && input->SameVec(G->GetPosition(), VZero))
						BreakFlag = true;
				}
				if (BreakFlag) {
					BreakFlag = false;
					break;
				}
				//　弾をfalseからtrueに
				pGameObject->SetVisible(true);
				Generate = false;
				CoolTimeFlag = false;
				CoolTime = 0;
				break;
			}
			//  trueの弾が初期位置じゃなかったら
			else if (!input->SameVec(pGameObject->GetPosition(), VZero)) {
				//　初期位置に弾がセットされていたらスキップ
				for (auto pG : pBullet) {
					if (pG->IsVisible() && input->SameVec(pG->GetPosition(), VZero)) {
						BreakFlag = true;
					}
				}
				if (BreakFlag) {
					BreakFlag = false;
					break;
				}
				//　弾生成フラグをtrue
				Generate = true;
				CoolTimeFlag = false;
				CoolTime = 0;
				break;
			}
		}
	}

	//  弾のインスタンス化
	if (Generate) {
		pBullet.push_back(new Bullet(MV1DuplicateModel(bulletModel)));
		Generate = false;
		CoolTime = 0.0f;
	}

	//  クールタイムの更新
	if (CoolTimeFlag)
		CoolTime += TimeManager::GetInstance()->GetDeltaTime();

	if (!CoolTimeFlag && !Release)
		TitleCoolTime += TimeManager::GetInstance()->GetDeltaTime();

	if (TitleCoolTime >= TITLE_COOLTIME_MAX)
		TitleUp = true;

	//  マウスが押されたら弾を出す
	if (TitleUp)
		CoolTimeFlag = true;

	// マウスのボタンが押されたり離されたりしたかどうかの情報を取得する
	if (!Release) {
		if (TitleUp) {
			Release = true;
			MouseButton = true;
			CoolTime = 0.0f;
			TitleCoolTime = 0.0f;
			TitleUp = false;
		}
	}

	//  初期位置にセットされているか
	for (auto pGameObject : pBullet) {
		if (pGameObject->IsVisible() && input->SameVec(pGameObject->GetPosition(), VZero)) {
			ShotBullet = pGameObject;
		}
	}
	//  角度変更
	if (MouseButton) {
		ShotBullet->SetRotation(-50, 0, ShotBullet->GetRotation().z);
	}

	//  弾の更新
	for (auto pGameObject : pBullet) {
		pGameObject->Update();
	}
	*/
#endif

	//  入力管理クラスの取得
	InputManager* input = InputManager::GetInstance();

	//  クールタイムが終わったら弾を出す
	if (CoolTime >= 0.5f) {
		Release = false;
		//  弾のVisibleがfalseの弾があればtrueに,なければ弾生成用のフラグをtrueに
		for (auto pGameObject : pBullet) {
			if (!pGameObject->IsVisible()) {
				//　初期位置に弾がセットされていたらスキップ
				for (auto G : pBullet) {
					if (G->IsVisible() && input->SameVec(G->GetPosition(), VZero))
						BreakFlag = true;
				}
				if (BreakFlag) {
					BreakFlag = false;
					break;
				}
				//　弾をfalseからtrueに
				pGameObject->SetVisible(true);
				Generate = false;
				CoolTimeFlag = false;
				CoolTime = 0;
				break;
			}
			//  trueの弾が初期位置じゃなかったら
			else if (!input->SameVec(pGameObject->GetPosition(), VZero)) {
				//　初期位置に弾がセットされていたらスキップ
				for (auto pG : pBullet) {
					if (pG->IsVisible() && input->SameVec(pG->GetPosition(), VZero)) {
						BreakFlag = true;
					}
				}
				if (BreakFlag) {
					BreakFlag = false;
					break;
				}
				//　弾生成フラグをtrue
				Generate = true;
				CoolTimeFlag = false;
				CoolTime = 0;
				break;
			}
		}
	}

	//  弾のインスタンス化
	if (Generate) {
		pBullet.push_back(new Bullet(MV1DuplicateModel(bulletModel)));
		Generate = false;
		CoolTime = 0.0f;
	}

	//  マウスが押されたら弾を出す
	if (input->IsMouseButtonDown()) {
		Down = true;
	}
	if (Down && input->IsMouseButtonUp()) {
		CoolTimeFlag = true;
		Down = false;
	}

	//  クールタイムの更新
	if (CoolTimeFlag && !BombManager::GetInstance()->GetGameStart())
		CoolTime += TimeManager::GetInstance()->GetDeltaTime();

	//  マウスの位置を取得
	GetMousePoint(&MouseX, &MouseY);
	// マウスのボタンが押されたり離されたりしたかどうかの情報を取得する
	if (!Release) {
		if (input->IsMouseButtonDown()) {
			GameManager::GetInstance()->SetArrowFlag(true);
			DownMouseX = MouseX;
			DownMouseY = MouseY;
			MouseButton = true;
		}
		if (input->IsMouseButtonUp()) {
			GameManager::GetInstance()->SetArrowFlag(false);
			if (MouseButton) {
				if (MouseY - DownMouseY > 100) {
					Release = true;
					//  エフェクト再生
					EffectManager::GetInstance()->Instantiate("CoolTime", VGet(0, -100, 0));
				}
			}
			MouseButton = false;
			CoolTime = 0.0f;
		}
	}

	//  初期位置にセットされているか
	for (auto pGameObject : pBullet) {
		if (pGameObject->IsVisible() && input->SameVec(pGameObject->GetPosition(), VZero)) {
			ShotBullet = pGameObject;
		}
	}
	if (MouseButton) {
		ShotBullet->SetRotation((DownMouseY - MouseY) / 10,
			(DownMouseX - MouseX) / 10,
			ShotBullet->GetRotation().z);
	}

	//  弾が放たれたらリセット
	if (Release) {
		DownMouseX = 0;
		DownMouseY = 0;
		MouseX = 0;
		MouseY = 0;
	}

	//  弾の更新
	for (auto pGameObject : pBullet) {
		pGameObject->Update();
	}
}

/*
*   @function   Render
*   @brief		描画処理
*/
void BulletManager::Render() {
	//  弾の描画
	for (auto pGameObject : pBullet) {
		pGameObject->Render();
	}

#if _DEBUG
	(0, 160, red, "CoolTime : %f", CoolTime);
	clsDx();
	printfDx("%.2f ", CoolTime);
	DrawFormatString(0, 100, red, "%f", TitleCoolTime);
#endif
}

/*
*   @function   Delete
*   @brief		解放処理
*/
void BulletManager::Delete() {
	//  弾の解放処理
	for (auto pGameObject : pBullet) {
		delete pGameObject;
		pGameObject = nullptr;
	}
}

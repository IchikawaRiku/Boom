#pragma once
#include <vector>
#include "../GameObject/GameObject.h"
#include "../Manager/EffectManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/AudioManager.h"
#include "../Manager/CollisionManager.h"
#include "../Manager/BulletManager.h"
#include "../Manager/BombManager.h"
#include "../Manager/ScoreManager.h"
#include "../Manager/GameManager.h"

/*
*	@brief	シーンクラス
*	@tips	全てのシーンの基底クラス
*/
class BaseScene {
protected:	//  メンバ変数
	std::vector<GameObject*> pGameObjectArray;	//　一元管理配列

public:		//  コンストラクタとデストラクタ
	/*
	*	@brief	コンストラクタ
	*/
	BaseScene() = default;

	/*
	*	@brief	デストラクタ
	*/
	virtual ~BaseScene() {
		//  動的確保したインスタンスの解放
		for (auto pObj : pGameObjectArray) {
			delete pObj;
			pObj = nullptr;
		}
		//  弾の解放
		BulletManager::GetInstance()->Delete();
		BulletManager::DestroyInstance();
		//  爆弾の解放
		BombManager::GetInstance()->Delete();
		BombManager::DestroyInstance();

		CollisionManager::DestroyInstance();
		//  使用中のエフェクトの登録も初期化
		EffectManager::DestroyInstance();
		//  使用中のサウンドの登録も初期化
		AudioManager::DestroyInstance();
		//  シングルトンのインスタンスの削除
		ScoreManager::DestroyInstance();
		GameManager::DestroyInstance();
	}

public:		//  メンバ関数
	/*
	*   @function	Start
	*	@brief		初期化処理
	*/
	virtual void Start() = 0;

	/*
	*   @function	Update
	*	@brief		更新処理
	*/
	virtual void Update() = 0;

	/*
	*   @function	Render
	*	@brief		描画処理
	*/
	virtual void Render() = 0;

};
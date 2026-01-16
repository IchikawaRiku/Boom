#include "Bomb.h"
#include "../../Component/Collider.h"
#include "../../Manager/CollisionManager.h"
#include "../../Manager/GameManager.h"
#include "../../Manager/EffectManager.h"
#include "../../Manager/AudioManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/TimeManager.h"
#include "../Camera/Camera.h"

/*
*   @brief	コンストラクタ
*/
Bomb::Bomb(int _handle, VECTOR _pos)
	: GameObject(_pos, BOMB_TAG) 
	, SPEED_RAND(50.0f)
	, SPEED_OFFSET(50.0f)
	, SHAKE_TIME(0.5f)
	, BOMB_HALF_SIZE(70){
	isVisible = false;
	scale = VScale(scale, 0.7f);
	//  弾のモデルデータのセット
	modelHandle = _handle;
	//  弾の当たり判定をセット
	//SetCollider(new SphereCollider(this, VZero, 80.0f));
	SetCollider(new CapsuleCollider(this, VZero, VScale(VUp, 50.0f), 70.0f));
	//  当たり判定を登録
	CollisionManager::GetInstance()->Register(this->GetCollider());
	Start();
}

/*
*   @brief	デストラクタ
*/
Bomb::~Bomb() {

}

/*
*   @function	Start
*   @brief		初期化処理
*/
void Bomb::Start() {
	speed = GetRand(SPEED_RAND) + SPEED_OFFSET;
	position = VGet(GetRand(BOMB_GENERATE_RAND_X) - BOMB_GENERATE_OFFSET_X,
		BOMB_GENERATE_POS_Y,
		GetRand(BOMB_GENERATE_RAND_Z) + BOMB_GENERATE_OFFSET_Z);
	if (SceneManager::GetInstance()->GetTitleFlag())
		position = VGet(0, BOMB_GENERATE_POS_Y, 0);

}

/*
*   @function	Update
*   @brief		更新処理
*/
void Bomb::Update() {
	//  爆弾管理クラスの取得
	GameManager* game = GameManager::GetInstance();
	if (!game->GetInstance()->GetGameOverFlag())
		isVisible = true;
	//  非表示なら初期化しない
	if (!isVisible)
		return;

	// 時間管理クラスの取得
	TimeManager* time = TimeManager::GetInstance();

	//  ランダムなスピードで降る
	position = VAdd(position, VScale(VDown, speed * time->GetDeltaTime()));

	//  地面に着いたらHPを減らしてリセット,HPが0ならゲームオーバー
	if (position.y <= BOTTOM) {
		EffectManager::GetInstance()->Instantiate(EXPLOSION_NAME, position);
		game->SetHP(game->GetHP() - 1);
		if (game->GetHP() <= 0) {
			isVisible = false;
			AudioManager::GetInstance()->Stop(GAME_BGM_NAME);
			if (!game->GetGameOverFlag()) {
				AudioManager::GetInstance()->PlayOneShot(GAME_OVER_NAME, 1);
				Camera::main->Shake(0, SHAKE_TIME);
			}
			game->SetGameOverFlag(true);
		}
		else {
			AudioManager::GetInstance()->PlayOneShot(EXPLOSION_NAME, 1);
			Bomb::Start();
			Camera::main->Shake(0, SHAKE_TIME);
		}
	}

	//  計算した座標、回転(オイラー角)、拡縮をモデルに反映する
	MV1SetPosition(modelHandle, position);
	MV1SetRotationXYZ(modelHandle, VScale(rotation, DX_PI_F / 180.0f));
	MV1SetScale(modelHandle, scale);

	//  エフェクトの位置を変更
	//EffectManager::GetInstance()->SetEfeStatus("Fire", position);

	GameObject::Update();
	//  当たり判定の更新
	if (pCollider != nullptr) {
		pCollider->SetMatrix(matrix);
		pCollider->Update();
	}
}

/*
*   @function	Render
*   @brief		描画処理
*/
void Bomb::Render() {
	//  非表示なら初期化しない
	if (!isVisible)
		return;

	//  モデルの描画
	MV1DrawModel(modelHandle);

	//  影の描画(平べったい円錐を陰にする)
	DrawCone3D(VGet(position.x, BOTTOM - BOMB_HALF_SIZE, position.z),
		VGet(position.x, BOTTOM - BOMB_HALF_SIZE + 0.1f, position.z),
		BOMB_HALF_SIZE / ((BOMB_GENERATE_POS_Y - BOTTOM) / ((BOMB_GENERATE_POS_Y - BOTTOM) - (position.y - BOTTOM))),
		SHADOW_POLYGON, black, black, TRUE);

	//  当たり判定の更新
	if (pCollider != nullptr)
		pCollider->Render();

	//  当たり判定の描画
	pCollider->Render();

#if _DEBUG
	DrawFormatString(0, 40, red, "Bomb.position : %.2f, %.2f, %.2f", position.x, position.y, position.z);
	DrawFormatString(0, 60, red, "Bullet.rotation : %.2f, %.2f, %.2f", rotation.x, rotation.y, rotation.z);
	DrawFormatString(0, 80, red, "forward : %.2f, %.2f, %.2f", forward.x, forward.y, forward.z);
#endif
}

/*
*   @function	TitleUpdate
*   @brief		タイトルの更新処理
*/
void Bomb::TitleUpdate() {
	if (!GameManager::GetInstance()->GetGameOverFlag())
		isVisible = true;
	//  非表示なら初期化しない
	if (!isVisible)
		return;

	//  計算した座標、回転(オイラー角)、拡縮をモデルに反映する
	MV1SetPosition(modelHandle, position);
	MV1SetRotationXYZ(modelHandle, VScale(rotation, DX_PI_F / 180.0f));
	MV1SetScale(modelHandle, scale);

	GameObject::Update();
	//  当たり判定の更新
	if (pCollider != nullptr) {
		pCollider->SetMatrix(matrix);
		pCollider->Update();
	}
}

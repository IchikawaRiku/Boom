#include "Bullet.h"
#include "../../Component/Collider.h"
#include "../../Manager/CollisionManager.h"
#include "../../Definition.h"

#include "../../Manager/InputManager.h"
#include "../../Manager/BulletManager.h"
#include "../../Manager/ScoreManager.h"
#include "../../Manager/EffectManager.h"
#include "../../Manager/AudioManager.h"
#include "../../Manager/BombManager.h"
#include "../../Manager/TimeManager.h"

/*
*   @brief	コンストラクタ
*/
Bullet::Bullet(int _handle, VECTOR _pos)
	: GameObject(_pos, "Bullet")
	, Gravity(0.2f)
	, Move(false)
	, singleScore(100)
	, scoreTime(1)
	, scoreView(false) 
	, SINGLE_SCORE_BASE(100)
	, GRAVITY_BASE(0.2f)
	, GRAVITY_RATE(350.0f)
	, BULLET_HALF_SIZE(70)
	, SCORE_TIMER(2.0f)
	, SCORE_BONUS_RATE(2){
	for (int i = 0; i < PRE_LINE; i++) {
		PrePos[i] = VZero;
	}
	scale = VScale(scale, 0.8f);
	speed = 850.0f;
	//  弾のモデルデータのセット
	modelHandle = _handle;
	//  弾の当たり判定をセット
	//SetCollider(new SphereCollider(this, VZero, 80.0f));
	SetCollider(new CapsuleCollider(this, VZero, VZero, 80.0f));
	//  当たり判定を登録
	CollisionManager::GetInstance()->Register(this->GetCollider());
}

/*
*   @brief	デストラクタ
*/
Bullet::~Bullet() {

}

/*
*   @function	Start
*   @brief		初期化処理
*/
void Bullet::Start() {
	//  非表示なら初期化しない
	if (!isVisible)
		return;

	isVisible = false;
	Move = false;
	position = VZero;
	rotation = VZero;
	Gravity = GRAVITY_BASE;
	moveTime = 0.0f;
	singleScore = SINGLE_SCORE_BASE;
	scoreTime = 1;
	scoreView = false;
	for (int i = 0; i < PRE_LINE; i++) {
		PrePos[i] = VZero;
	}
}

/*
*   @function	Update
*   @brief		更新処理
*/
void Bullet::Update() {
	//  非表示なら初期化しない
	if (!isVisible)
		return;

	//  入力管理クラスの取得
	InputManager* input = InputManager::GetInstance();

	//  弾管理クラスの取得
	BulletManager* BMana = BulletManager::GetInstance();

	//  爆弾管理クラスの取得
	BombManager* bomb = BombManager::GetInstance();

	//  時間管理クラスの取得
	TimeManager* time = TimeManager::GetInstance();

	GameObject::Update();

	//  弾が放たれたら進み続ける用
	if (BMana->GetRelease()) {
		Move = true;
	}

	//　弾の予測線を引く
	if (!Move) {
		float g = Gravity;
		PrePos[0] = position;
		for (int i = 1; i < PRE_LINE; i++)
		{
			PrePos[i] = VAdd(PrePos[i - 1], VScale(forward, speed * SIM_DT));
			PrePos[i].y -= g * SIM_DT;
			g += GRAVITY_RATE * SIM_DT;
		}
	}

	//  弾を放つ
	if (Move) {
		position = VAdd(position, VScale(forward, speed * time->GetDeltaTime()));
		position.y -= Gravity * time->GetDeltaTime();
		Gravity += GRAVITY_RATE * time->GetDeltaTime();
		moveTime += time->GetDeltaTime();
	}

	//  弾が通った後の予測点を0にする
	for (int i = 1; i < PRE_LINE; i++) {
		if (input->SameVec(PrePos[i], position)) {
			PrePos[i] = VZero;
		}
	}

	if (position.y <= BOTTOM) {
		Bullet::Start();
		//  弾が地面に着いたときゲームがスタートされていなかったらボムカウントリセット
		if (!bomb->GetGameStart())
			bomb->SetBombCount(0);
	}

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

/*
*   @function	Render
*   @brief		描画処理
*/
void Bullet::Render() {
	//  非表示なら初期化しない
	if (!isVisible)
		return;

	//  入力管理クラスの取得
	InputManager* input = InputManager::GetInstance();

	//  モデルの描画
	MV1DrawModel(modelHandle);
#if _DEBUG
	DrawFormatString(0, 40, red, "Bullet.position : %.2f, %.2f, %.2f", position.x, position.y, position.z);
	DrawFormatString(0, 60, red, "Bullet.rotation : %.2f, %.2f, %.2f", rotation.x, rotation.y, rotation.z);
	DrawFormatString(0, 80, red, "forward : %.2f, %.2f, %.2f", forward.x, forward.y, forward.z);
#endif

	for (int i = 0; i < PRE_LINE - 1; i++) {
		int moveCount = static_cast<int>(moveTime / SIM_DT);
		//　もう弾が通ることのない部分は線を引かない
		if (i <= moveCount || PrePos[i].y < BOTTOM - BULLET_HALF_SIZE)
			continue;

		//  弾の軌道線を描画する
		DrawLine3D(PrePos[i], PrePos[i + 1], cyan);

		//  弾の軌道線を描画する
		DrawLine3D(VGet(PrePos[i].x, BOTTOM - BULLET_HALF_SIZE, PrePos[i].z), VGet(PrePos[i + 1].x, BOTTOM - BULLET_HALF_SIZE, PrePos[i + 1].z), black);
	}

	//  影の描画
	DrawCone3D(VGet(position.x, BOTTOM - BULLET_HALF_SIZE, position.z),
		VGet(position.x, BOTTOM - BULLET_HALF_SIZE + 0.1f, position.z),
		BULLET_HALF_SIZE / ((BOMB_GENERATE_POS_Y - BOTTOM) / ((BOMB_GENERATE_POS_Y - BOTTOM) - (position.y - BOTTOM))),
		SHADOW_POLYGON, black, black, TRUE);

	//  当たり判定の更新
	if (pCollider != nullptr)
		pCollider->Render();
}

/*
	*	@function	OnTriggerEnter
	*   @brief		当たった瞬間
	*	@param[in]	Collider* _pCpl
	*/
void Bullet::OnTriggerEnter(Collider* _pCol) {
	//  当たった相手のタグが"Bomb"だったら
	if (_pCol->GetGameObject()->GetTag() == BOMB_TAG) {
		//  スコア管理クラスの取得
		ScoreManager* ScoreM = ScoreManager::GetInstance();
		//  爆弾管理クラスの取得
		BombManager* bomb = BombManager::GetInstance();
		//　スコアセット
		for (int i = 0; i < ScoreM->SCORE_VISIBLE_MAX; i++) {
			//  表示されているものはスキップ
			if (ScoreM->score[i].isVisible)
				continue;

			ScoreM->score[i] = { true, SCORE_TIMER, _pCol->GetGameObject()->GetPosition(), singleScore };
			break;
		}
		//  スコア表示
		scoreView = true;
		//  スコア増やす
		ScoreM->SetTotalScore(ScoreM->GetTotalScore() + singleScore);
		//  スコアにボーナスを掛ける
		singleScore = singleScore * SCORE_BONUS_RATE;

		//  エフェクト再生
		EffectManager::GetInstance()->Instantiate(EXPLOSION_NAME, _pCol->GetGameObject()->GetPosition());

		//　サウンド再生
		AudioManager::GetInstance()->PlayOneShot(EXPLOSION_NAME, EXPLOSION_SE_VOLUME);

		//  爆弾に当たったらカウント+1
		bomb->SetBombCount(bomb->GetBombCount() + 1);

		//　ゲームスタート
		bomb->SetGameStart(true);

		//  当たった相手を初期化させる
		_pCol->GetGameObject()->Start();
	}
}

/*
	*	@function	OnTriggerStay
	*   @brief		当たっている間
	*	@param[in]	Collider* _pCpl
	*/
void Bullet::OnTriggerStay(Collider* _pCol) {}

/*
	*	@function	OnTriggerExit
	*   @brief		離れた瞬間
	*	@param[in]	Collider* _pCpl
	*/
void Bullet::OnTriggerExit(Collider* _pCol) {}

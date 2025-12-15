#pragma once
#include "../GameObject.h"

// 弾の軌道線の最大数
#define PRE_LINE	(255)

/*
*	//	弾クラス
*	//　GameObjectクラスの派生クラス 
*/
class Bullet : public GameObject {
private:	//  メンバ変数
	float Gravity;					//  重力
	float moveTime;					//  動いた時間
	bool Move;						//  進むフラグ
	int singleScore;				//  弾1つが持つスコアのボーナス
	float scoreTime;				//  スコアを表示しておく時間をカウントする用
	bool scoreView;					//  スコアを表示するためのフラグ
	VECTOR PrePos[PRE_LINE];		//  弾の軌道座表

	const float SIM_DT = 1.0f / FPS;

public:		//	コンストラクタとデストラクタ
	bool fired = false;
	/*
	*   @brief	コンストラクタ
	*/
	Bullet(int _handle, VECTOR _pos = VZero);

	/*
	*   @brief	デストラクタ
	*/
	~Bullet();

public:		//  オーバーライドしたメンバ変数
	/*
	*   @function	Start
	*   @brief		初期化処理
	*/
	void Start() override;

	/*
	*   @function	Update
	*   @brief		更新処理
	*/
	void Update() override;

	/*
	*   @function	Render
	*   @brief		描画処理
	*/
	void Render() override;

public:		//  オーバーライドした衝突検知
	/*
	*	@function	OnTriggerEnter
	*   @brief		当たった瞬間
	*	@param[in]	Collider* _pCpl
	*/
	void OnTriggerEnter(Collider* _pCol) override;

	/*
	*	@function	OnTriggerStay
	*   @brief		当たっている間
	*	@param[in]	Collider* _pCpl
	*/
	void OnTriggerStay(Collider* _pCol) override;

	/*
	*	@function	OnTriggerExit
	*   @brief		離れた瞬間
	*	@param[in]	Collider* _pCpl
	*/
	void OnTriggerExit(Collider* _pCol) override;

public:		//  Getter と Setter

};


#pragma once
#include "../GameObject.h"
class Cannon : public GameObject {
private:	//  メンバ変数
	bool EffectFlag;		//  エフェクト再生のフラグ
	bool ReleaseFlag;		//  リリースのためのフラグ

	const float CANNON_MUZZLE_LENGTH;	// 大砲の銃口までの距離

public:		//	コンストラクタとデストラクタ
	/*
	*   @brief	コンストラクタ
	*/
	Cannon(VECTOR _pos = VZero);

	/*
	*   @brief	デストラクタ
	*/
	~Cannon();

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

public:		//　Getter と Setter

	/*
	*   @function	GetEffectFlag
	*   @brief		エフェクト再生のフラグの取得
	*   @return		VECTOR
	*/
	inline bool GetEffectFlag() const { return EffectFlag; }

	/*
	*   @function	SetEffectFlag
	*   @brief		エフェクト再生のフラグの変更
	*   @param[in]	bool _v		初期化する値
	*/
	inline void SetEffectFlag(bool _v) { EffectFlag = _v; }
};



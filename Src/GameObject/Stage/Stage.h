#pragma once
#include "../GameObject.h"
class Stage : public GameObject {
private:	//  メンバ変数
	int SkyModel;		

public:		//	コンストラクタとデストラクタ
	/*
	*   @brief	コンストラクタ
	*/
	Stage(VECTOR _pos = VZero);

	/*
	*   @brief	デストラクタ
	*/
	~Stage();

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

public:		//  Getter とSetter

	/*
	*   @function	SetSkyModel
	*   @brief		グラフィックの変更
	*   @param[in]	int _v		初期化する値
	*/
	inline void SetSkyModel(int _v) { SkyModel = _v; }

};


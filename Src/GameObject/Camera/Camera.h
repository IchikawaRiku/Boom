#pragma once
#include "../GameObject.h"

/*
*   @brief	カメラクラス
*   @tips	GameObjectクラスの派生クラス
*/
class Camera : public GameObject {
private:	//  メンバ変数
	VECTOR shakeOffset;			//  揺れ調整用
	VECTOR shakePattern;		//  実際の揺れ
	float timer;				//  時間計測用
	float shakeTime;			//  揺れ時間
	bool isShaking;				//　揺れ中かどうか

public:		//  静的メンバ変数
	static Camera* main;

public:		//  コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @param[in]	VECTOR _pos = VZero			初期化する座標
	*/
	Camera(VECTOR _pos = VZero);

	/*
	*   @brief		デストラクタ
	*/
	~Camera();

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

public:		//　メンバ関数
	/*
	*	@function	Shake
	*	@brief		カメラシェイク、カメラの揺れ
	*	@param[in]	int _direction			揺れの方向  0:横, 1:縦, 2:両方
	*	@param[in]	float _time				揺れの長さ
	*	@param[in]	float _power = 0.25f	揺れの強さ
	*/
	void Shake(int _direction, float _time, float _power = 0.25f);

public:		//  Getter と Setter

};


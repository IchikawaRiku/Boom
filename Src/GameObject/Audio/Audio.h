#pragma once
#include "../GameObject.h"

/*
*	@brief	音源オブジェクト
*/
class Audio : public GameObject {
private:		//  メンバ変数
	bool is3Dsound;				//  3Dサウンドするかどうか
	bool isLoop;				//  ループするかどうか
	int soundHandle;			//  音のハンドル
	float volume;				//  音量
	float distance;				//  聞こえる範囲

public:			//  コンストラクタとデストラクタ
	/*
	*	@brief		コンストラクタ
	*	@param[in]	int _soundHandle	音ハンドル
	*	@param[in]	bool _is3D = false	3D音源かどうか
	*/
	Audio(int _soundHandle, bool _is3D = false);

	/*
	*	@brief		デストラクタ
	*/
	~Audio();

public:			//  オーバーライドしたメンバ関数
	/*
	*   @function	Start
	*	@brief		初期化処理
	*/
	void Start() override;

	/*
	*   @function	Update
	*	@brief		更新処理
	*/
	void Update() override;

	/*
	*   @function	Render
	*	@brief		描画処理
	*/
	void Render() override;

public:			//  メンバ関数

public:			//  Getter と Setter
	/*
	*	@function	GetSoundHandle
	*	@brief		音ハンドルの取得
	*	@return		int
	*/
	inline int GetSoundHandle() const { return soundHandle; }

	/*
	*	@function	SetLoop
	*	@brief		ループ再生の変更
	*	@param[in]	bool _v
	*/
	inline void SetLoop(bool _v) { isLoop = _v; }

	/*
	*	@function	SetVolum
	*	@brief		音量の変更
	*	@param[in]	float _v
	*/
	inline void SetVolume(float _v) { volume = _v; }
};


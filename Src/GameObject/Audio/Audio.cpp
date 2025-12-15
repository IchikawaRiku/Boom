#include "Audio.h"

/*
*	@brief		コンストラクタ
*	@param[in]	int _soundHandle	音ハンドル
*	@param[in]	bool _is3D = false	3D音源かどうか
*/
Audio::Audio(int _soundHandle, bool _is3D)
	: GameObject()
	, is3Dsound(_is3D)
	, isLoop(false)
	, soundHandle(_soundHandle)
	, volume(255)
	, distance(800.0f){
}

/*
*	@brief		デストラクタ
*/
Audio::~Audio() {
	DeleteSoundMem(soundHandle);
}

/*
*   @function	Start
*	@brief		初期化処理
*/
void Audio::Start() {
	//  音量の設定
	ChangeVolumeSoundMem((int)volume, soundHandle);

	//  音の再生
	if (isLoop)
		PlaySoundMem(soundHandle, DX_PLAYTYPE_LOOP);
	else
		PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK);
}

/*
*   @function	Update
*	@brief		更新処理
*/
void Audio::Update() {
	//  非表示なら更新しない
	if (!isVisible)
		return;

	//  再生中かどうか
	if (CheckSoundMem(soundHandle) == 0)
		isVisible = false;

	//  音量の設定
	ChangeVolumeSoundMem((int)volume, soundHandle);

	//  3Dサウンドの設定
	if (is3Dsound) {
		//  音源の位置の設定
		Set3DPositionSoundMem(position, soundHandle);
		//  音が聞こえる範囲の設定
		Set3DRadiusSoundMem(distance, soundHandle);
	}
}

/*
*   @function	Render
*	@brief		描画処理
*/
void Audio::Render() {

}


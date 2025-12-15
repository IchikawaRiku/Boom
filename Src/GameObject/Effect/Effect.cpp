#include "Effect.h"
#include <EffekseerForDXLib.h>

/*
*   @brief		コンストラクタ
*	@param[in]	int& _resourceHandle
*/
Effect::Effect(int& _resourceHandle)
	: GameObject()
	, resourceHandle(_resourceHandle)
	, playingHandle(INVALID) {
}

/*
*	@brief		デストラクタ
*/
Effect::~Effect() {

}

/*
*   @function	Start
*	@brief		初期化処理
*/
void Effect::Start() {

}

/*
*   @function	Update
*	@brief		更新処理
*/
void Effect::Update() {
	//  非表示なら表示しない
	if (!isVisible)
		return;

	//  エフェクトの再生
	if (playingHandle == INVALID)
		playingHandle = PlayEffekseer3DEffect(resourceHandle);

	//  再生が終わったら非表示にする
	if (IsEffekseer3DEffectPlaying(playingHandle) == INVALID)
		isVisible = false;

	//  エフェクトの座標の変更
	SetPosPlayingEffekseer3DEffect(playingHandle, position.x, position.y, position.z);
	//  エフェクトの角度の変更
	SetRotationPlayingEffekseer3DEffect(playingHandle, Deg2Rad(rotation.x), Deg2Rad(rotation.y), Deg2Rad(rotation.z));
}

/*
*   @function	Render
*	@brief		描画処理
*/
void Effect::Render() {
	//  非表示なら表示しない
	if (!isVisible)
		return;
#if _DEBUG
	DrawString(200, 200, "red", red);
#endif
}

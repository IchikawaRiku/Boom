#include "Stage.h"
#include "../../Manager/BulletManager.h"

/*
*   @brief	コンストラクタ
*/
Stage::Stage(VECTOR _pos)
	: GameObject(_pos) 
	, BACK_GROUND_POS(VGet(0, 700, 0))
	, BACK_GROUND_ROT(VGet(0, -90, 0)){
	scale = VScale(scale, 2.0f);
}

/*
*   @brief	デストラクタ
*/
Stage::~Stage() {

}

/*
*   @function	Start
*   @brief		初期化処理
*/
void Stage::Start() {

}

/*
*   @function	Update
*   @brief		更新処理
*/
void Stage::Update() {
	//  計算した座標、回転(オイラー角)、拡縮をモデルに反映する
	MV1SetPosition(modelHandle, position);
	MV1SetRotationXYZ(modelHandle, VScale(rotation, DX_PI_F / 180.0f));
	MV1SetScale(modelHandle, scale);

	MV1SetPosition(SkyModel, BACK_GROUND_POS);
	MV1SetRotationXYZ(SkyModel, VScale(BACK_GROUND_ROT, DX_PI_F / 180.0f));
}

/*
*   @function	Render
*   @brief		描画処理
*/
void Stage::Render() {
	//  背景画像の描画
	//DrawGraph(0, 0, Graph, true);
	MV1DrawModel(SkyModel);
	//  モデルの描画
	MV1DrawModel(modelHandle);

}

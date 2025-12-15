#include "TimeManager.h"
#include <DxLib.h>
#include "../Definition.h"

#pragma region シングルトン

//  静的メンバ変数の初期化
TimeManager* TimeManager::pInstance = nullptr;

/*
*   @brief		コンストラクタ
*/
TimeManager::TimeManager()
	: prevTime(GetNowCount())
	, currentTime(prevTime)
	, deltaTime(currentTime - prevTime)
	, m(0)
	, s(0)
	, ms(0) {
	Start();
}

/*
*   @brief		デストラクタ
*/
TimeManager::~TimeManager() {
}

/*
	*   @function	CreateInstance
	*   @brief		自身のインスタンスを生成する
	*/
void TimeManager::CreateInstance() {
	pInstance = new TimeManager();
}

/*
	*   @function	GetInstance
	*   @brief		自身のインスタンスを取得する唯一の手段
	*   @return		TimeManager*	自身のインスタンスのアドレス
	*/
TimeManager* TimeManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();
	return pInstance;
}

/*
	*   @function	DestroyInstance
	*   @brief		自身のインスタンスを破棄する唯一の手段
	*/
void TimeManager::DestroyInstance() {
	if (pInstance == nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}
#pragma endregion
/*
*   @function	Start
*	@brief		初期化処理
*/
void TimeManager::Start(){
}

/*
*   @function	Update
*	@brief		更新処理
*/
void TimeManager::Update() {
	//  現在のフレームの時間を取得
	currentTime = GetNowCount();

	//  ウィンドウがアクティブな場合に処理する
	if (!GetNoActiveState()) {
		//  1フレーム前の時間との差分を計算
		deltaTime = currentTime - prevTime;

		//  時間の計算
		ms += deltaTime;
		if (ms >= 1000) {
			ms -= 1000;
			s++;
		}
		if (s >= 60) {
			s -= 60;
			m++;
		}

		//  deltaTime を Unity のように扱うために単位を ms -> s に変換
		deltaTime /= 1000.0f;
	}
	//　1フレーム前の時間を現在の時間に更新
	prevTime = currentTime;
}

/*
*   @function	Render
*	@brief		描画処理
*/
void TimeManager::Render() {
#if _DEBUG
	DrawFormatString(500, 0, red, "%02d:%02d:%03d", m, s, ms);
	DrawFormatString(500, 20, red, "FPS:%.0f", 1.0f / deltaTime);
#endif
}
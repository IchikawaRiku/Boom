#include "EffectManager.h"
#include "EffekseerForDXLib.h"
#include "../GameObject/Effect/Effect.h"

//  静的メンバ変数の初期化
EffectManager* EffectManager::pInstance = nullptr;

/*
*   @brief		コンストラクタ
*/
EffectManager::EffectManager()
	: effectResourceMap()
	, pEffectList() {

}

/*
*   @brief		デストラクタ
*/
EffectManager::~EffectManager() {
	//  リソースの解放
	for (auto itr : effectResourceMap) {
		DeleteEffekseerEffect(itr.second);
	}
	effectResourceMap.clear();

	for (auto pEffe : pEffectList) {
		if (pEffe != nullptr) {
			delete pEffe;
			pEffe = nullptr;
		}
	}
	pEffectList.clear();
}

/*
*   @function	CreateInstance
*   @brief		自身のインスタンスを生成する
*   @return		EffectManager*
*/
void EffectManager::CreateInstance() {
	pInstance = new EffectManager();
}

/*
*   @function	GetInstance
*   @brief		自身のインスタンスを取得する唯一の手段
*   @return		EffectManager*	自身のインスタンスのアドレス
*/
EffectManager* EffectManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();
	return pInstance;
}

/*
*   @function	DestroyInstance
*   @brief		自身のインスタンスを破棄する唯一の手段
*/
void EffectManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}

/*
*   @function	Load
*   @brief		エフェクトの読み込み
*   @param[in]	std::string _filePath		ファイルパス
* 　@param[in]  std::string _name			エフェクトにつける名前
* 　@param[in]  float _magnification = 1.0f 拡大率
*/
void EffectManager::Load(std::string _filePath, std::string _name, float _magnification) {
	int res = LoadEffekseerEffect(_filePath.c_str(), _magnification);

	//  リソースの管理
#if 0
	effectResourceMap[_filePath.c_str()] = res;
#else
	//  連想配列の中にすでに登録されているファイルか検索する
	auto itr = effectResourceMap.find(_filePath.c_str());

	// 登録されていなければ
	if (itr == effectResourceMap.end()) {
		//  登録する
		effectResourceMap.emplace(_name.c_str(), res);
	}
#endif
}

/*
*   @function	Instantiate
*   @brief		エフェクトの発生
*   @param[in]	std::string _name			エフェクトの名前
* 　@param[in]  VECTOR _pos					発生させる場所
* 　@param[in]  VECTOR _rot					発生させる角度
* 　@param[in]  Effect*
*/
Effect* EffectManager::Instantiate(std::string _name, VECTOR _pos, VECTOR _rot) {
	Effect* pEffect = new Effect(effectResourceMap[_name]);
	pEffect->SetTag(_name);
	pEffect->SetPosition(_pos);
	pEffect->SetRotation(_rot.x, _rot.y, _rot.z);
	pEffectList.push_back(pEffect);
	return pEffect;
}

/*
*   @function	SetEfeStatus
*   @brief		エフェクトの位置変更
*   @param[in]	std::string _name			エフェクトの名前
* 　@param[in]  VECTOR _pos					変更場所
* 　@param[in]  VECTOR _rot = VZero			変更角度
*/
void EffectManager::SetEfeStatus(std::string _name, VECTOR _pos, VECTOR _rot) {
	for (auto pA : pEffectList) {
		if (pA->GetTag() == _name) {
			pA->SetPosition(_pos);
			pA->SetRotation(_rot.x, _rot.y, _rot.z);
		}
	}
}

/*
*   @function	Update
*   @brief		更新処理
*/
void EffectManager::Update() {
	for (auto pEffe : pEffectList) {
		if (pEffe == nullptr || !pEffe->IsVisible())
			continue;

		pEffe->Update();
	}

	//  STLの要素を削除(条件付き)
	//  std::erase_if ( コンテナ. ラムダ ) C++20↑
	//  isVisible = false のエフェクトを消す
	std::erase_if(
		pEffectList,
		[](Effect* _pE) { return !_pE->IsVisible(); }
	);

	UpdateEffekseer3D();
}

/*
*   @function	Render
*   @brief		描画処理
*/
void EffectManager::Render() {
	for (auto pEffe : pEffectList) {
		if (pEffe == nullptr || !pEffe->IsVisible())
			continue;

		pEffe->Render();
	}
	DrawEffekseer3D();
}
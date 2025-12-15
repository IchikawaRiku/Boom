#include "AudioManager.h"
#include "../GameObject/Audio/Audio.h"

#pragma region シングルトン

//  静的メンバ変数の初期化
AudioManager* AudioManager::pInstance = nullptr;

/*
*   @brief		コンストラクタ
*   @tips		外部で生成されないようにアクセス指定子を private にする
*/
AudioManager::AudioManager()
	: audioResourceMap()
	, pAudioList() {
}

/*
*   @brief		デストラクタ
*/
AudioManager::~AudioManager() {
	for (auto itr : audioResourceMap) {
		DeleteSoundMem(itr.second);
	}
	audioResourceMap.clear();

	for (auto pA : pAudioList) {
		if (pA != nullptr) {
			delete pA;
			pA = nullptr;
		}
	}
	pAudioList.clear();
}

/*
*   @function	CreateInstance
*   @brief		自身のインスタンスを生成する
*/
void AudioManager::CreateInstance() {
	pInstance = new AudioManager();
}

/*
*   @function	GetInstance
*   @brief		自身のインスタンスを取得する唯一の手段
*   @return		AudioManager*	自身のインスタンスのアドレス
*   @tips		生成は１度だけ行う
*/
AudioManager* AudioManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();

	return pInstance;
}

/*
*   @function	DestroyInstance
*   @brief		自身のインスタンスを破棄する唯一の手段
*/
void AudioManager::DestroyInstance() {
	if (pInstance != nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}
}
#pragma endregion

/*
*   @function	Load
*   @brief		音の読み込み
*   @param[in]	std::string _filePath		ファイルパス
* 　@param[in]  std::string _name			音につける名前
* 　@param[in]  bool _is3D					3Dサウンドかどうか
*/
void AudioManager::Load(std::string _filePath, std::string _name, bool _is3D) {
	//  読み込み
	SetCreate3DSoundFlag(_is3D);
	int res = LoadSoundMem(_filePath.c_str());

	//  連想配列の中にすでに登録されているファイル化検索する
	auto itr = audioResourceMap.find(_filePath.c_str());

	// 登録されていなければ
	if (itr == audioResourceMap.end()) {
		//  登録する
		audioResourceMap.emplace(_name.c_str(), res);
	}
}

/*
*	@function	ChangeVolume
*	@brief		音量の変更
*	@param[in]	std::string _name = ""	変更する音の名前
*	@param[in]	float _volum			変更する音量 0.0f ~ 1.0f
*/
void AudioManager::ChangeVolume(std::string _name, float _volume) {
	//  全ての音量の変更
	if (_name == "All") {
		for (auto pA : pAudioList) {
			pA->SetVolume(_volume * 255);
		}
	}
	//  一部の音量の変更
	else {
		for (auto pA : pAudioList) {
			if (pA->GetTag() == _name) {
				pA->SetVolume(_volume * 255);
			}
		}
	}
}

/*
*	@function	PlayOneShot
*	@brief		音の再生
*	@param[in]	std::string _name		再生する音の名前
*	@param[in]	float _volume = 1.0f	音量
*	@param[in]  bool _loop = false		ループするかどうか
*/
void AudioManager::PlayOneShot(std::string _name, float _volume, bool _loop) {
	Audio* pAudioObj = new Audio(DuplicateSoundMem(audioResourceMap[_name]));
	pAudioObj->SetTag(_name);
	pAudioObj->SetVolume(_volume * 255);
	pAudioObj->SetLoop(_loop);
	pAudioObj->Start();
	pAudioList.push_back(pAudioObj);
}

/*
*	@function	Stop
*	@brief		音の停止
*	@param[in]	std::string _name		停止する音の名前
*/
void AudioManager::Stop(std::string _name) {
	//  全ての音量の変更
	if (_name == "All") {
		for (auto pA : pAudioList) {
			StopSoundMem(pA->GetSoundHandle());
		}
	}
	//  一部の音量の変更
	else {
		for (auto pA : pAudioList) {
			if (pA->GetTag() == _name) {
				StopSoundMem(pA->GetSoundHandle());
			}
		}
	}
}

/*
*   @function	Update
*   @brief		更新処理
*/
void AudioManager::Update() {
	for (auto pA : pAudioList) {
		if (pA == nullptr || !pA->IsVisible())
			continue;

		pA->Update();
	}

	//  STLの要素を削除(条件付き)
	//  std::erase_if ( コンテナ. ラムダ ) C++20↑
	//  isVisible = false の音を消す
	std::erase_if(
		pAudioList,
		[](Audio* _pA) { return !_pA->IsVisible(); }
	);
}
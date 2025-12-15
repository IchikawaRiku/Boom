#pragma once
#include <string>
#include <list>
#include <map>
#include <DxLib.h>

/*
*	@brief	音管理クラス
*/
class AudioManager {
#pragma region  シングルトンのデータ構造
private:	//　静的メンバ変数
	static AudioManager* pInstance;		//  自身のインスタンスのアドレスを格納

private:	//  コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @tips		外部で生成されないようにアクセス指定子を private にする
	*/
	AudioManager();

	/*
	*   @brief		デストラクタ
	*/
	~AudioManager();

public:		//  コピーと譲渡の禁止
	AudioManager(const AudioManager&) = delete;
	AudioManager(AudioManager&&) = delete;
	AudioManager& operator = (const AudioManager&) = delete;
	AudioManager& operator = (AudioManager&&) = delete;

private:	//  静的メンバ変数
	/*
	*   @function	CreateInstance
	*   @brief		自身のインスタンスを生成する
	*/
	static void CreateInstance();

public:		//	静的メンバ変数
	/*
	*   @function	GetInstance
	*   @brief		自身のインスタンスを取得する唯一の手段
	*   @return		AudioManager*	自身のインスタンスのアドレス
	*/
	static AudioManager* GetInstance();

	/*
	*   @function	DestroyInstance
	*   @brief		自身のインスタンスを破棄する唯一の手段
	*/
	static void DestroyInstance();

#pragma endregion

	std::map<std::string, int> audioResourceMap;		//  リソースの連想配列
	std::list<class Audio*> pAudioList;					//  音の一元管理

public:		//　メンバ関数
	/*
	*   @function	Load
	*   @brief		音の読み込み
	*   @param[in]	std::string _filePath		ファイルパス
	* 　@param[in]  std::string _name			音につける名前
	* 　@param[in]  bool _is3D					3Dサウンドかどうか
	*/
	void Load(std::string _filePath, std::string _name, bool _is3D);

	/*
	*	@function	ChangeVolume
	*	@brief		音量の変更
	*	@param[in]	std::string _name = ""	変更する音の名前
	*	@param[in]	float _volum			変更する音量 0.0f ~ 1.0f
	*/
	void ChangeVolume(std::string _name, float _volume);

	/*
	*	@function	PlayOneShot
	*	@brief		音の再生
	*	@param[in]	std::string _name		再生する音の名前
	*	@param[in]	float _volume = 1.0f	音量
	*	@param[in]  bool _loop = false		ループするかどうか
	*/
	void PlayOneShot(std::string _name, float _volume = 1.0f, bool _loop = false);
	
	/*
	*	@function	Stop
	*	@brief		音の停止
	*	@param[in]	std::string _name		停止する音の名前
	*/
	void Stop(std::string _name);

	/*
	*   @function	Update
	*   @brief		更新処理
	*/
	void Update();

};


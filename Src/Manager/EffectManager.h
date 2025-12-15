#pragma once
#include <string>
#include <list>
#include <map>
#include <DxLib.h>
#include "../Definition.h"

/*
*   @brief  入力管理クラス
* 　@tips　	管理するクラスは１つであるべき -> シングルトンのデータ構造
*/
class EffectManager {
#pragma region  シングルトンのデータ構造
private:	//　静的メンバ変数
	static EffectManager* pInstance;		//  自身のインスタンスのアドレスを格納

private:	//  コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @tips		外部で生成されないようにアクセス指定子を private にする
	*/
	EffectManager();

	/*
	*   @brief		デストラクタ
	*/
	~EffectManager();

public:		//  コピーと譲渡の禁止
	EffectManager(const EffectManager&) = delete;
	EffectManager(EffectManager&&) = delete;
	EffectManager& operator = (const EffectManager&) = delete;
	EffectManager& operator = (EffectManager&&) = delete;

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
	*   @return		EffectManager*	自身のインスタンスのアドレス
	*/
	static EffectManager* GetInstance();

	/*
	*   @function	DestroyInstance
	*   @brief		自身のインスタンスを破棄する唯一の手段
	*/
	static void DestroyInstance();

#pragma endregion

private:		//  メンバ変数
	std::map<std::string, int> effectResourceMap;		//  リソースの連想配列
	std::list<class Effect*> pEffectList;			//  エフェクトの一元管理

public:		//　メンバ関数
	/*
	*   @function	Load
	*   @brief		エフェクトの読み込み
	*   @param[in]	std::string _filePath		ファイルパス
	* 　@param[in]  std::string _name			エフェクトにつける名前
	* 　@param[in]  float _magnification = 1.0f 拡大率
	*/
	void Load(std::string _filePath, std::string _name, float _magnification = 1.0f);

	/*
	*   @function	Instantiate
	*   @brief		エフェクトの発生
	*   @param[in]	std::string _name			エフェクトの名前
	* 　@param[in]  VECTOR _pos					発生させる場所	
	* 　@param[in]  VECTOR _rot = VZero			発生させる角度
	* 　@param[in]  Effect*
	*/
	class Effect* Instantiate(std::string _name, VECTOR _pos, VECTOR _rot = VZero);

	/*
	*   @function	SetEfeStatus
	*   @brief		エフェクトの位置変更
	*   @param[in]	std::string _name			エフェクトの名前
	* 　@param[in]  VECTOR _pos					変更場所
	* 　@param[in]  VECTOR _rot = VZero			変更角度
	*/
	void SetEfeStatus(std::string _name, VECTOR _pos, VECTOR _rot = VZero);

	/*
	*   @function	Update
	*   @brief		更新処理
	*/
	void Update();

	/*
	*   @function	Render
	*   @brief		描画処理
	*/
	void Render();
		

public:		//  Getter と Setter

};


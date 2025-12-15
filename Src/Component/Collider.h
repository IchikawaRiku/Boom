#pragma once
#include "../GameObject/GameObject.h"
#include <DxLib.h>

/*
*   @brief	当たり判定の基底クラス
*/
class Collider {
protected:	//  メンバ変数
	bool isEnable;				//  当たり判定の有効、無効フラグ
	GameObject* pGameObject;	//  当たり判定をつけるオブジェクト
	MATRIX matrix;				//  当たり判定をつけるオブジェクト行列

public:		//  コンストラクタとデストラクタ
	/*
	*   @brief	コンストラクタ
	* 　@param[in]	GameObject* _pObj	当たり判定をつけるオブジェクト
	*/
	Collider(GameObject* _pObj);

	/*
	*   @brief	デストラクタ
	* 　@tips	基底クラスなので仮想関数で実装
	*/
	virtual ~Collider();

public:		//　メンバ関数
	/*
	*   @function	Update
	*   @brief		更新処理
	* 　@tips		純粋仮想関数で実装
	*/
	virtual void Update() = 0;

	/*
	*   @function	Render
	*   @brief		描画処理
	* 　@tips		純粋仮想関数で実装
	*/
	virtual void Render() = 0;

public:		//  Getter と Setter
	/*
	*   @function	IsEnable
	*   @brief		有効フラグの取得
	*   @return		bool
	*/
	inline bool IsEnable() const { return isEnable; }

	/*
	*   @function	SetEnable
	*   @brief		有効フラグの変更
	*   @param[in]	bool _v
	*/
	inline bool SetEnable(bool _v) { isEnable = _v; }

	/*
	*   @function	GetGameObject
	*   @brief		当たり判定をつけるオブジェクトの取得
	*   @return		GameObject*
	*/
	inline GameObject* GetGameObject() const { return pGameObject; }

	/*
	*   @function	SetGameObject
	*   @brief		当たり判定をつけるオブジェクトの変更
	*   @param[in]	GameObject* _pObj
	*/
	inline void SetGameObject(GameObject* _pObj) { isEnable = _pObj; }

	/*
	*   @function	SetMatrix
	*   @brief		行列の変更
	*   @param[in]	GameObject* _pObj
	*/
	inline void SetMatrix(MATRIX _matrix) { matrix = _matrix; }
};


/*
*	@brief	当たり判定　Box
*/
class BoxCollider : public Collider {
private:	//  メンバ変数
	VECTOR originMinPoint;		//  AABBを構成する最小の点
	VECTOR originMaxPoint;		//  AABBを構成する最大の点

	VECTOR originVertex[8];		//  頂点データ
	VECTOR localVertex[8];		//  頂点データ
	VECTOR worldVertex[8];		//  頂点データ

public:		//  コンストラクタとデストラクタ
	/*
	*	@brief	コンストラクタ
	*	@param[in]	GameObject* _pObj	当たり判定をつけるオブジェクト
	*	@param[in]  VECTOR min			最小の点
	*	@param[in]  VECTOR max			最大の点
	*/
	BoxCollider(GameObject* _pObj, VECTOR _min, VECTOR _max);

	/*
	*  @brief  デストラクタ
	*/
	~BoxCollider() = default;

public:		//  オーバーライドしたメンバ関数
	/*
	*   @function	Update
	*   @brief		更新処理
	*/
	void Update() override;

	/*
	*   @function	Render
	*   @berief		描画処理
	*/
	void Render() override;

public:		//  Getter と Setter
	/*
	*   @functin	GetMinPoint
	*	@brief		最小の点を取得
	*   @reurn		VECTOR
	*/
	inline VECTOR GetMinPoint() const { return worldVertex[0]; }

	/*
	*   @functin	GetMaxPoint
	*	@brief		最大の点を取得
	*   @reurn		VECTOR
	*/
	inline VECTOR GetMaxPoint() const { return worldVertex[7]; }

};

/*
*   @brief	当たり判定 Sphere
*/
class SphereCollider : public Collider {
private:	//　メンバ変数
	VECTOR localCenter;		//  中心点
	VECTOR worldCenter;		//  中心点
	float radius;			//  半径

public:		//　コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @param[in]	GameObject			当たり判定をつけるオブジェクト
	* 　@param[in]　VECTOR _offset		位置調整
	*   @param[in]  float _radius		半径
	*/
	SphereCollider(GameObject* _pObj, VECTOR _offset, float _radius);

	/*
	*  @brief  デストラクタ
	*/
	~SphereCollider() = default;

public:		//  オーバーライドしたメンバ関数
	/*
	*   @function	Update
	*   @brief		更新処理
	*/
	void Update() override;

	/*
	*   @function	Render
	*   @berief		描画処理
	*/
	void Render() override;

public:		//  Getter と Setter
	/*
	*	@function	GetWorldCenter
	*	@brief		中心点の取得
	*   @return		VECTOR
	*/
	inline VECTOR GetWorldCenter() const { return worldCenter; }

	/*
	*	@function	GetRadius
	*	@brief		半径の取得
	*	@return		float
	*/
	inline float GetRadius() const { return radius; }
};

/*
*   @crief	当たり判定 Capsule
*/
class CapsuleCollider : public Collider {
private:	//  メンバ変数
	VECTOR originPoint1, originPoint2;	//  線分を構成する2点
	float radius;						//　半径

	VECTOR offset;						//　位置調整
	VECTOR localPoint1, localPoint2;	//　ローカル座標用
	VECTOR worldPoint1, worldPoint2;	//　ワールド座標用

public:		//　コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @param[in]	GameObject* _pObj		当たり判定をつけるオブジェクト
	* 　@param[in]　VECTOR _p1				線分を構成する点1
	* 　@param[in]　VECTOR _p2				線分を構成する点2
	*   @param[in]  float _radius			半径
	*/
	CapsuleCollider(GameObject* _pObj, VECTOR _p1, VECTOR _p2, float _radius);

	/*
	*  @brief  デストラクタ
	*/
	~CapsuleCollider() = default;

public:		//  オーバーライドしたメンバ関数
	/*
	*   @function	Update
	*   @brief		更新処理
	*/
	void Update() override;

	/*
	*   @function	Render
	*   @berief		描画処理
	*/
	void Render() override;

public:		//  Getter と Setter
	/*
	*	@function	GetWorldPoint1
	*	@brief		線分の始点を取得
	*	@return		VECTOR
	*/
	inline VECTOR GetWorldPoint1() const { return worldPoint1; }

	/*
	*	@function	GetWorldPoint2
	*	@brief		線分の終点を取得
	*	@return		VECTOR
	*/
	inline VECTOR GetWorldPoint2() const { return worldPoint2; }

	/*
	*	@function	GetRadius
	*	@brief		半径の取得
	*	@return		float
	*/
	inline float GetRadius() const { return radius; }
};
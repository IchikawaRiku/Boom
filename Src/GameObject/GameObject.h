#pragma once
#include "../Definition.h"

/*
*   @brief  ゲームオブジェクトクラス
* 　@tips	ゲーム中に存在するモノのほぼ全ての基底クラス
*/
class GameObject {
protected:	//	メンバ変数
	int modelHandle;	//  3Dモデルのハンドル
	bool isVisible;		//  表示、非表示フラグ
	float speed;		//  速さ
	VECTOR position;	//  座標
	VECTOR rotation;	//  回転 ( オイラー角 )
	VECTOR scale;		//  拡縮

	MATRIX matrix;		//  座標、回転(ベクトル)、拡縮を併せ持つ4x4行列

	VECTOR forward;		//  前
	VECTOR right;		//  右
	VECTOR up;			//  上

	class Collider* pCollider;		//  当たり判定のポインタ

	std::string tag;				//　タグ

public:		//  コンストラクタとデストラクタ
	/*
	*   @brief		コンストラクタ
	*   @param[in]	VECTOR _pos = V_Zero	初期化する座標
	*/
	GameObject(VECTOR _pos = VZero, std::string _tag = "");

	/*
	*   @brief		デストラクタ
	*   @tips		基底クラスなので仮想関数で実装する
	*/
	virtual ~GameObject();

public:		//　メンバ変数
	/*
	*   @function	Start
	*   @brief		初期化処理
	* 　@tips		純粋仮想関数で実装する
	*/
	virtual void Start() = 0;

	/*
	*   @function	Update
	*   @brief		更新処理
	* 　@tips		純粋仮想関数で実装する
	*/
	virtual void Update();

	/*
	*   @function	Render
	*   @brief		描画処理
	* 　@tips		純粋仮想関数で実装する
	*/
	virtual void Render() = 0;

public:		//  衝突検知関数
	/*
	*   @function	OnTriggerEnter
	*	@brief		当たった瞬間
	*	@param[in]  Collider* _pOther	当たった相手
	*/
	virtual void OnTriggerEnter(class Collider* _pOther);

	/*
	*   @function	OnTriggerStay
	*	@brief		当たっている間
	*	@param[in]  Collider* _pOther	当たった相手
	*/
	virtual void OnTriggerStay(class Collider* _pOther);

	/*
	*   @function	OnTriggerExit
	*	@brief		離れた瞬間
	*	@param[in]  Collider* _pOther	当たった相手
	*/
	virtual void OnTriggerExit(class Collider* _pOther);

public:		//  Getter と Setter

	/*
	*   @function	IsVisible
	*   @brief		表示フラグの取得
	*   @return		bool
	*/
 	inline bool IsVisible() const { return isVisible; }

	/*
	*   @function	SetVisible
	*   @brief		表示フラグの変更
	*   @param[in]	bool _v		初期化する値
	*/
	inline void SetVisible(bool _v) { isVisible = _v; }

	/*
	*   @function	IsPosition
	*   @brief		座標の取得
	*   @return		VECTOR
	*/
	inline VECTOR GetPosition() const { return position; }

	/*
	*   @function	SetPosition
	*   @brief		座標の変更
	*   @param[in]	VECTOR _v		初期化する値
	*/
	inline void SetPosition(VECTOR _v) { position = _v; }

	/*
	*   @function	SetPosition
	*   @brief		座標の変更
	*   @param[in]	float _x		初期化するxの成分
	*   @param[in]	float _y		初期化するyの成分
	*   @param[in]	float _z		初期化するzの成分
	*/
	inline void SetPosition(float _x, float _y, float _z) { position = VGet(_x, _y, _z); }

	/*
	*   @function	IsRotation
	*   @brief		回転( オイラー角 )の取得
	*   @return		VECTOR
	*/
	inline VECTOR GetRotation() const { return rotation; }

	/*
	*   @function	SetRotation
	*   @brief		回転( オイラー角 )の変更
	*   @param[in]	VECTOR _v		初期化する回転( オイラー角 )
	*/
	inline void SetRotation(VECTOR _v) { rotation = _v; }

	/*
	*   @function	SetRotation
	*   @brief		座標の変更
	*   @param[in]	float _roll		初期化するxの回転
	*   @param[in]	float _pitch	初期化するyの回転
	*   @param[in]	float _yaw		初期化するzの回転
	*/
	inline void SetRotation(float _roll, float _pitch, float _yaw) { rotation = VGet(_roll, _pitch, _yaw); }

	/*
	*   @function	IsScale
	*   @brief		拡縮の取得
	*   @return		VECTOR
	*/
	inline VECTOR GetScale() const { return scale; }

	/*
	*   @function	SetScale
	*   @brief		拡縮の変更
	*   @param[in]	VECTOR _v		初期化する拡縮
	*/
	inline void SetScale(VECTOR _v) { scale = _v; }

	/*
	*   @function	SetScale
	*   @brief		座標の変更
	*   @param[in]	float _x		初期化するxの成分
	*   @param[in]	float _y		初期化するyの成分
	*   @param[in]	float _z		初期化するzの成分
	*/
	inline void SetScale(float _x, float _y, float _z) { scale = VGet(_x, _y, _z); }

	/*
	*	@function	GetMatrix
	*	@brief		4x4の行列の取得
	*	@return		MATRIX
	*/
	inline MATRIX GetMatrix() const { return matrix; }

	/*
	*   @function	GetCollider
	*   @brief		当たり判定の取得
	*   @return		Collider*
	*/
	inline Collider* GetCollider() const { return pCollider; }

	/*
	*   @function	SetCollider
	*   @brief		当たり判定の変更
	*   @param[in]	Collider* _pCol
	*/
	inline void SetCollider(Collider* _pCol) { pCollider = _pCol; }

	/*
	*   @function	GetModelHandle
	*   @brief		モデルハンドルの取得
	*   @return		int
	*/
	inline int GetModelHandele() const { return modelHandle; }

	/*
	*   @function	SetModelHandle
	*   @brief		モデルハンドルの変更
	*   @param[in]	int _v　初期化するモデルハンドル
	*/
	inline void SetModelHandle(int _v) { modelHandle = _v; }

	/*
	*   @function	GetTag
	*   @brief		タグの取得
	*	@return		std::string
	*/
	inline std::string GetTag() const { return tag; }

	/*
	*	@function	SetTag
	*	@brief		タグの変更
	*	@return		std::string
	*/
	inline void SetTag(std::string _tag) { tag = _tag; }

};


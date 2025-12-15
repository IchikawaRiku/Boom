#include "CollisionManager.h"
#include "../Component/Collider.h"

#pragma region シングルトン

//  静的メンバ変数の初期化
CollisionManager* CollisionManager::pInstance = nullptr;

/*
*   @brief		コンストラクタ
*/
CollisionManager::CollisionManager()
	: pColliderArray()
	, prevs()
	, currents() {
}

/*
*   @brief		デストラクタ
*/
CollisionManager::~CollisionManager() {
	//for (auto pCol : pColliderArray) {
	//	if (pCol != nullptr) {
	//		delete pCol;
	//		pCol = nullptr;
	//	}
	//}

	pColliderArray.clear();
	pColliderArray.shrink_to_fit();
	prevs.clear();
	prevs.shrink_to_fit();
	currents.clear();
	currents.shrink_to_fit();

}

/*
	*   @function	CreateInstance
	*   @brief		自身のインスタンスを生成する
	*/
void CollisionManager::CreateInstance() {
	pInstance = new CollisionManager();
}

/*
	*   @function	GetInstance
	*   @brief		自身のインスタンスを取得する唯一の手段
	*   @return		CollisionManager*	自身のインスタンスのアドレス
	*/
CollisionManager* CollisionManager::GetInstance() {
	if (pInstance == nullptr)
		CreateInstance();
	return pInstance;
}

/*
	*   @function	DestroyInstance
	*   @brief		自身のインスタンスを破棄する唯一の手段
	*/
void CollisionManager::DestroyInstance() {
	if (pInstance == nullptr) {
		delete pInstance;
		pInstance = nullptr;
	}

}

/*
*   @function	Update
*	@brief		更新処理
*/
void CollisionManager::Update() {
	for (int i = 0; i < currents.size(); i++) {
		for (int j = i + 1; j < currents[i].size(); j++) {

			if (pColliderArray[i] == nullptr ||
				pColliderArray[j] == nullptr ||
				!pColliderArray[i]->GetGameObject()->IsVisible() || !pColliderArray[i]->IsEnable() ||
				!pColliderArray[j]->GetGameObject()->IsVisible() || !pColliderArray[j]->IsEnable()) {
				currents[i][j] = false;
				continue;
			}
			currents[i][j] = CheckHit(pColliderArray[i], pColliderArray[j]);

			//  当たった瞬間	(!前 && 今)
			if ((!prevs[i][j] && currents[i][j])) {
				pColliderArray[i]->GetGameObject()->OnTriggerEnter(pColliderArray[j]);
				pColliderArray[j]->GetGameObject()->OnTriggerEnter(pColliderArray[i]);
			}
			//  当たっている間	(今)
			else if (currents[i][j]) {
				pColliderArray[i]->GetGameObject()->OnTriggerEnter(pColliderArray[i]);
				pColliderArray[j]->GetGameObject()->OnTriggerEnter(pColliderArray[j]);
			}
			//  離れた瞬間	(前 && !今)
			else if ((prevs[i][j] && !currents[i][j])) {
				pColliderArray[i]->GetGameObject()->OnTriggerEnter(pColliderArray[i]);
				pColliderArray[j]->GetGameObject()->OnTriggerEnter(pColliderArray[j]);
			}
		}
	}

	for (int i = 0; i < currents.size(); i++) {
		for (int j = 0; j < currents[i].size(); j++)
		{
			prevs[i][j] = currents[i][j];
		}
	}
}

/*
*	@function	Register
*	@brief		一元管理に登録する
*   @param[in]	Collider* _pCol		登録する当たり判定
*/
void CollisionManager::Register(Collider* _pCol) {
	//  一元管理配列に追加する
	pColliderArray.push_back(_pCol);

	prevs.push_back(std::vector<bool>());
	currents.push_back(std::vector<bool>());

	index++;

	int i = 0;
	while (1) {
		if (i == index)
			break;

		while (1) {
			if (prevs[i].size() == index)
				break;

			prevs[i].push_back(false);
			currents[i].push_back(false);
		}
		i++;
	}
}

/*
*   @function	Unregister
*	@brief		一元管理から抹消する
*   @param[in]	Collider* _pCol		抹消する当たり判定
*/
void CollisionManager::Unregister(Collider* _pCol) {
	//  2種類の考え方を記載

	//  イデレータの考え方
	//  vector の先頭から末尾までの中に _pCol があるかどうか調べる
	auto itr = std::find(pColliderArray.begin(), pColliderArray.end(), _pCol);

	//  イデレータが末尾まで進んだ場合が _pCol がなかった時
	if (itr == pColliderArray.end())
		return;

	//  配列の要素の考え方
	int index = -1;		//  要素番号

	//  配列の中に検索する要素があるかどうか
	for (int i = 0; i < pColliderArray.size(); i++) {
		if (*itr == _pCol) {
			index = i;		//  あったら要素番号を保存しておく
			break;
		}
	}

	//  指定された要素を削除する
	pColliderArray.erase(itr);
	//  衝突結果配列の要素も削除する
	prevs.erase(prevs.begin() + index);
	currents.erase(currents.begin() + index);
}
#pragma endregion

/*
*	@function	CheckHit
*	@brief		当たり判定の衝突検知
*	@param[in]	Collider* _pCol1		当たり判定１
*   @param[in]  Collider* _pCol2		当たり判定２
*	@return		bool
*/
bool CollisionManager::CheckHit(Collider* _pCol1, Collider* _pCol2) {
	//  Box-○○
	{
		//  _pCol1 が BoxColider* なのかを調べる
		auto tmp1 = dynamic_cast<BoxCollider*>(_pCol1);

		if (tmp1 != nullptr) {
			//  pCol1 -> BoxCollider* !!!!!

			//　Box-Box
			{
				//  _pCol2 が BoxCollider* なのかを調べる
				auto tmp2 = dynamic_cast<BoxCollider*>(_pCol2);

				if (tmp2 != nullptr) {
					//  _pCol2 -> BoxCollider* !!!!!

					//  AABB 計算 -> 回転は考慮しない
					VECTOR minPoint1 = tmp1->GetMinPoint();
					VECTOR minPoint2 = tmp2->GetMinPoint();
					VECTOR maxPoint1 = tmp1->GetMaxPoint();
					VECTOR maxPoint2 = tmp2->GetMaxPoint();

					//  当たりの検知
					if ((minPoint1.x <= maxPoint2.x && minPoint2.x <= maxPoint1.x) &&
						(minPoint1.y <= maxPoint2.y && minPoint2.y <= maxPoint1.y) &&
						(minPoint1.z <= maxPoint2.z && minPoint2.z <= maxPoint1.z)) {
						return true;
					}
				}
			}

			//　Box-Sphere
			{
				//  _pCol2 が SphereCollider* なのかを調べる
				auto tmp2 = dynamic_cast<SphereCollider*>(_pCol2);

				if (tmp2 != nullptr) {
					//  _pCol2 -> SphereCollider* !!!!!

					//  計算
				}
			}

			//　Box-Capsule
			{
				//  _pCol2 が CapsuleCollider* なのかを調べる
				auto tmp2 = dynamic_cast<CapsuleCollider*>(_pCol2);

				if (tmp2 != nullptr) {
					//  _pCol2 -> CapsuleCollider* !!!!!

					//  計算
				}
			}

		}
	}

	//  Sphere-○○
	{

		//  _pCol1 が BoxColider* なのかを調べる
		auto tmp1 = dynamic_cast<SphereCollider*>(_pCol1);

		if (tmp1 != nullptr) {
			//  pCol1 -> BoxCollider* !!!!!

			//　Sphere-Box
			{
				//  _pCol2 が BoxCollider* なのかを調べる
				auto tmp2 = dynamic_cast<BoxCollider*>(_pCol2);

				if (tmp2 != nullptr) {
					//  _pCol2 -> BoxCollider* !!!!!

					//  計算
				}
			}

			//　Sphere-Sphere
			{
				//  _pCol2 が SphereCollider* なのかを調べる
				auto tmp2 = dynamic_cast<SphereCollider*>(_pCol2);

				if (tmp2 != nullptr) {
					//  _pCol2 -> SphereCollider* !!!!!

					//  計算
					//  2つの中心点を取得
					VECTOR center1 = tmp1->GetWorldCenter();
					VECTOR center2 = tmp2->GetWorldCenter();

					//  2点間の差分を求める
					VECTOR delta = VSub(center1, center2);

					//  当たりの検知
					//  差分の長さ <= 半径1 + 半径2
					if (VSize(delta) <= tmp1->GetRadius() + tmp2->GetRadius()) {
						return true;
					}
				}
			}

			//　Sphere-Capsule
			{
				//  _pCol2 が CapsuleCollider* なのかを調べる
				auto tmp2 = dynamic_cast<CapsuleCollider*>(_pCol2);

				if (tmp2 != nullptr) {
					//  _pCol2 -> CapsuleCollider* !!!!!

					//  計算
				}
			}

		}
	}

	//  Capsule-○○
	{

		//  _pCol1 が CapsuleColider* なのかを調べる
		auto tmp1 = dynamic_cast<CapsuleCollider*>(_pCol1);

		if (tmp1 != nullptr) {
			//  pCol1 -> CapsuleCollider* !!!!!

			//　Capsule-Box
			{
				//  _pCol2 が BoxCollider* なのかを調べる
				auto tmp2 = dynamic_cast<BoxCollider*>(_pCol2);

				if (tmp2 != nullptr) {
					//  _pCol2 -> BoxCollider* !!!!!

					//  計算
				}
			}

			//　Capsule-Sphere
			{
				//  _pCol2 が SphereCollider* なのかを調べる
				auto tmp2 = dynamic_cast<SphereCollider*>(_pCol2);

				if (tmp2 != nullptr) {
					//  _pCol2 -> SphereCollider* !!!!!

					//  計算
				}
			}

			//　Capsule-Capsule
			{
				//  _pCol2 が CapsuleCollider* なのかを調べる
				auto tmp2 = dynamic_cast<CapsuleCollider*>(_pCol2);

				if (tmp2 != nullptr) {
					//  _pCol2 -> CapsuleCollider* !!!!!

					//  計算
					//  線分と線分の最短距離を求める -> 余裕があれば自作！
					float distance = Segment_Segment_MinLength(tmp1->GetWorldPoint1(), tmp1->GetWorldPoint2(),
						tmp2->GetWorldPoint1(), tmp2->GetWorldPoint2());

					//  当たりの検知
					//  線分と線分の最短距離 < 半径1 + 半径2
					if (distance <= tmp1->GetRadius() + tmp2->GetRadius()) {
						return true;
					}
				}
			}
		}
	}

	return false;
}


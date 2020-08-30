#pragma once
#include<memory>
#include<list>
class Collider;
/// <summary>
/// 衝突判定を全般的に管理する
/// </summary>
class CollisionManager
{
private:
	std::list<std::shared_ptr<Collider>> colliders_;///<当たり判定集合体
public:
	/// <summary>
	/// 全コリジョンを判定する(毎フレーム)
	/// </summary>
	void Update();

	/// <summary>
	/// 全コリジョンを表示する(デバッグ用)
	/// </summary>
	void DebugDraw();

	/// <summary>
	/// コリジョンを判定対象に追加する
	/// </summary>
	/// <param name="collider">判定対象オブジェクト</param>
	std::shared_ptr<Collider>  AddCollider(Collider* collider);
};


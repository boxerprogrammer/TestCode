#pragma once

#include"../../Geometry.h"
#include<memory>
class Camera;
class Player;
class Input;
class CollisionManager;
constexpr size_t sword_equip_no = 0;//刀
constexpr size_t bomb_equip_no = 1;//爆弾
constexpr size_t shuriken_equip_no = 2;//手裏剣
constexpr size_t chain_equip_no = 3;//鎖鎌
/// <summary>
/// 装備品基底クラス
/// </summary>
class Equipment
{
protected:
	std::shared_ptr<CollisionManager> collisionManager_;
	std::shared_ptr<Camera> camera_;
public:
	/// <param name="collisionMgr">当たり判定マネージャ</param>
	/// <param name="camera">カメラへの参照</param>
	Equipment(std::shared_ptr<CollisionManager> collisionMgr ,
		std::shared_ptr<Camera> camera);
	virtual ~Equipment() = default;
	/// <summary>
	/// 今の武器で攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
	virtual void Attack(const Player& p, const Input& i,Vector2f offset = Vector2f::ZERO) = 0;
	/// <summary>
	/// 追加入力(ほぼ鎖鎌用)
	/// </summary>
	/// <param name="i">現在のinput</param>
	virtual void AdditionalInput(const Input& i) {};
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update() {};

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() {};
};


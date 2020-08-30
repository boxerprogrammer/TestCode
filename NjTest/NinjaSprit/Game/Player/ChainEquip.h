#pragma once
#include "Equipment.h"
#include"../../Geometry.h"
class Player;
class CapsuleCollider;
class Camera;
class ShadowClone;
/// <summary>
/// 鎖鎌装備
/// 伸び縮みする。伸びている最中に先行入力すると振り回すことができる
/// </summary>
class ChainEquip : public Equipment
{
private:
	int throwSE_ = -1;
	int chainH_ = -1;
	ShadowClone* shadow_=nullptr;
	CapsuleCollider* capsuleCollider_ = nullptr;
	int frame_;
	std::shared_ptr<Player>& player_;
	float swingTargetAngle_=0.0f;
	float swingPerAngle_ = 0.0f;
	float currentAngle_ = 0.0f;
	int swingFrame_ = 0;
	Vector2f direction_;
	float GetCurrentChainLength()const;
	bool CanBeAdditionalInput()const;
public:
	/// <summary>
	/// 鎖鎌装備
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="collisionMgr">当たり判定マネージャ</param>
	/// <param name="camera">カメラ</param>
	ChainEquip(std::shared_ptr<Player>& player, 
		std::shared_ptr<CollisionManager> collisionMgr, 
		std::shared_ptr<Camera> camera, ShadowClone* shadow=nullptr);
	/// <summary>
	/// 鎖鎌攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
	void Attack(const Player& player, const Input& input,Vector2f offset=Vector2f::ZERO)override;
	/// <summary>
	/// 追加入力(振り回しのためだけのメソッド)
	/// </summary>
	/// <param name="input">入力情報</param>
	void AdditionalInput(const Input& input);
	/// <summary>
	/// 毎フレーム更新用
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画用
	/// </summary>
	void Draw()override;
};


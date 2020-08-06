#pragma once
#include "Equipment.h"
#include"../../Geometry.h"
class Player;
class CapsuleCollider;
class Camera;
/// <summary>
/// 鎖鎌装備
/// 伸び縮みする。伸びている最中に先行入力すると振り回すことができる
/// </summary>
class ChainEquip : public Equipment
{
private:
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
	ChainEquip(std::shared_ptr<Player>& player, std::shared_ptr<CollisionManager> cm, std::shared_ptr<Camera> camera);
	/// <summary>
	/// 鎖鎌攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
		void Attack(const Player& player, const Input& input)override;
		void AdditionalInput(const Input& input);
		void Update()override;
		void Draw()override;
};


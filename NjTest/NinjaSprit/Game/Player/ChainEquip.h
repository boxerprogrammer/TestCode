#pragma once
#include "Equipment.h"
#include"../../Geometry.h"
class Player;
class CapsuleCollider;
class Camera;
/// <summary>
/// ½Š™‘•”õ
/// L‚Ñk‚İ‚·‚éBL‚Ñ‚Ä‚¢‚éÅ’†‚Éæs“ü—Í‚·‚é‚ÆU‚è‰ñ‚·‚±‚Æ‚ª‚Å‚«‚é
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
	/// ½Š™UŒ‚‚ğ‰Á‚¦‚Ü‚·
	/// </summary>
	/// <param name="player">ƒvƒŒƒCƒ„[‚Ö‚ÌQÆ</param>
	/// <param name="input">“ü—ÍQÆ</param>
		void Attack(const Player& player, const Input& input)override;
		void AdditionalInput(const Input& input);
		void Update()override;
		void Draw()override;
};


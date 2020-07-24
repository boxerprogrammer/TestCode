#pragma once

#include<memory>
class Camera;
class Player;
class Input;
class CollisionManager;
class Equipment
{
protected:
	std::shared_ptr<CollisionManager> collisionManager_;
	std::shared_ptr<Camera> camera_;
public:
	Equipment(std::shared_ptr<CollisionManager> cm , std::shared_ptr<Camera> camera);
	virtual ~Equipment() = default;
	/// <summary>
	/// 今の武器で攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
	virtual void Attack(const Player& p, const Input& i) = 0;
	/// <summary>
	/// 追加入力(ほぼ鎖鎌用)
	/// </summary>
	/// <param name="i">現在のinput</param>
	virtual void AdditionalInput(const Input& i) {};
	virtual void Update() {};
	virtual void Draw() {};
};


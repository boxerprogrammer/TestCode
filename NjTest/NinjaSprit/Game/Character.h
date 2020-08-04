#pragma once
#include<memory>
#include"../Geometry.h"


class Camera;
class Collider;
class Input;
struct CollisionInfo;
///キャラクター基底クラス
class Character
{
	friend Collider;
protected:
	Position2f pos_;//キャラ中心座標
	std::shared_ptr<Camera> camera_;
public:
	Character(std::shared_ptr<Camera> camera);

	///座標アクセス
	virtual const Vector2f& GetPosition()const;
	virtual void SetPosition(const Vector2f& vel);

	virtual ~Character() = default;
	///<summary>キャラクターの更新を行う</summary>
	virtual void Update() = 0;
	///<summary>キャラクターの表示を行う</summary>
	virtual void Draw() = 0;
	virtual void OnHit(CollisionInfo& ) = 0;
};

inline bool is_falling(const Vector2f& velocity) {
	return velocity.y >= 0;
}
#pragma once
#include"../Character.h"
#include<vector>
#include<array>
#include<list>
///ÉvÉåÉCÉÑÅ[ÉNÉâÉX
class GameplayingScene;
class Equipment;
class Input;
class PlayerInputListener;

class CollisionManager;

class Player : public Character {
	friend PlayerInputListener;
private:
	//std::array<Position2f, 2> shadowPositions_;
	std::array<Position2f, 60> moveHistory_;
	size_t currentMoveIndex_ = 0;
	void SetCurrentPosition(Position2f& pos);
	const Position2f& GetBackTimePosition(size_t backFrame)const;

	Position2f lastPos_;
	/*uint32_t frameForShadow_=0;*/
	float velY_ = 0.0f;
	float accelY_ = 0.0f;
	bool isRight_ = true;
	std::vector<std::shared_ptr<Equipment>> equipments_;
	size_t currentEquipmentNo_ = 0;
	std::shared_ptr<CollisionManager> collisionManager_;

	//í èÌèÛë‘
	void NormalUpdate();
	//â∫ç~èÛë‘
	void FallUpdate();
	//è„è∏èÛë‘
	void RiseUpdate();
	using Updater_t = void (Player::*)();
	using Drawer_t = void (Player::*)();

	//í èÌï`âÊ
	void NormalDraw();
	//â∫ç~ï`âÊ
	void FallDraw();
	//è„è∏ï`âÊ
	void RiseDraw();

	Updater_t updater_;
	Drawer_t drawer_;
	GameplayingScene* gs_;
public:
	enum class Direction {
		left,
		right
	};

	Direction GetDirection()const;
	void OnHit(CollisionInfo&)override;
	Player(GameplayingScene* gs);
	~Player();
	void Attack(const Input& input);
	void Jump();
	void NextEquip();
	void SetPosition(const Position2f& p);
	const Position2f& Position()const;
	void Move(const Vector2f& v);
	void Update()override;
	void Draw()override;
	std::shared_ptr<Equipment> CurrentEquipment();
	size_t CurrentEquipmentNo()const;
};

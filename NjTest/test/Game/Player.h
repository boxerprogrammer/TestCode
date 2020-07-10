#pragma once
#include"Character.h"
#include<vector>
///プレイヤークラス
class GameplayingScene;
class Equipment;
class Input;
class PlayerInputListener;
class Player : public Character {
	friend PlayerInputListener;
private:
	bool isRight_=true;
	std::vector<std::shared_ptr<Equipment>> equipments_;
	size_t currentEquipmentNo_=0;
public:
	enum class Direction {
		left,
		right
	};
	
	Direction GetDirection()const;

	Player(GameplayingScene* gs);
	~Player();
	void Attack(const Input& input);
	void NextEquip();
	void SetPosition(const Position2f& p);
	const Position2f& Position()const;
	void Move(const Vector2f& v);
	void Update()override;
	void Draw()override;
	std::shared_ptr<Equipment> CurrentEquipment();
	int CurrentEquipmentNo()const;
};

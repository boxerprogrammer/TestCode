#pragma once
#include"Character.h"
#include<vector>
///プレイヤークラス
class GameplayingScene;
class Equipment;
class Input;
class Player : public Character {
private:
	std::vector<std::shared_ptr<Equipment>> equipments_;
	size_t currentEquipmentNo_=0;
public:
	Player(GameplayingScene* gs);
	~Player();
	void Attack(const Input& input);
	void SetPosition(const Position2& p);
	const Position2& Position()const;
	void Move(const Vector2& v);
	void Update()override;
	void Draw()override;
};

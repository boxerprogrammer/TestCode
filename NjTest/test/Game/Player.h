#pragma once
#include"Character.h"
///�v���C���[�N���X
class GameplayingScene;
class Player : public Character {
public:
	Player(GameplayingScene* gs);
	~Player();
	void SetPosition(const Position2& p);
	void Move(const Vector2& v);
	void Update()override;
	void Draw()override;
};

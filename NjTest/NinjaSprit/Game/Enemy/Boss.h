#pragma once
#include "Enemy.h"
#include<memory>
class GameplayingScene;
class Boss :
    public Enemy
{
protected:
    GameplayingScene* gameScene_;
public:
    Boss(GameplayingScene* gs);
    virtual ~Boss() = default;
	
	/// <summary>
	/// �U�����󂯂�
	/// </summary>
	/// <param name="damage">�_���[�W�l</param>
	virtual void OnDamage(int damage) = 0;

	/// <summary>
	/// ���C�x���g
	/// </summary>
	virtual void OnDead() = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;

	virtual const std::vector<Circle>& GetCircles()const = 0;
};


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
	/// 攻撃を受けた
	/// </summary>
	/// <param name="damage">ダメージ値</param>
	virtual void OnDamage(int damage) = 0;

	/// <summary>
	/// 死イベント
	/// </summary>
	virtual void OnDead() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	virtual const std::vector<Circle>& GetCircles()const = 0;
};


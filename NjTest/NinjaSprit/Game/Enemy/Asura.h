#pragma once
#include "Boss.h"
#include<array>

class EffectManager;
/// <summary>
/// 1面ボス阿修羅
/// </summary>
class Asura :
    public Boss
{
private:
    struct EnergyBall {
        Vector2f pos;
        int frame;
        EnergyBall(const Position2f& p):pos(p),frame(0) {
        }
    };
    std::array<EnergyBall, 4> energyBalls_ = {
        Vector2f(-128.0f,-420.0f),
        Vector2f(128.0f,-420.0f),
        Vector2f(-100.0f,-300.0f),
        Vector2f(100.0f,-300.0f)};

    std::shared_ptr<EffectManager> effectManager_;
    int ashuraH_ = -1;
    int frame_ = 0;
    Enemy* MakeClone()override;
    using Update_t =void (Asura::*)();
    Update_t updater_;
    using Draw_t = void (Asura::*)();
    Draw_t drawer_;

    //登場演出状態
    void EnteringUpdate();

    
    //通常状態
    void NormalUpdate();

    //ダメージ状態
    void DamageUpdate();

    //退場演出状態
    void ExitingUpdate();

    //死
    void DeadUpdate();

    //通常描画
    void NormalDraw();
    void DamageDraw();
    void ExitingDraw();
    void DeadDraw();
    std::vector<Circle> circles_;
public:
    Asura(GameplayingScene*);
    ~Asura()=default;

    /// <summary>
    /// 衝突判定イベント
    /// </summary>
    /// <param name="me">衝突情報自分</param>
    /// <param name="another">衝突情報相手</param>
    void OnHit(CollisionInfo& me, CollisionInfo& another)override;

	/// <summary>
	/// 攻撃を受けた
	/// </summary>
	/// <param name="damage">ダメージ値</param>
	void OnDamage(int damage)override;

	/// <summary>
	/// 死イベント
	/// </summary>
	void OnDead()override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update()override;
    /// <summary>
    /// 描画
    /// </summary>
    void Draw()override;

    const std::vector<Circle>& GetCircles()const override;
};


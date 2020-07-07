#pragma once
#include "Projectile.h"
class BombShot :
    public Projectile
{
private:
    float angle_;
private:
    using Func_t = void (BombShot::*)();

    int frame_;
    int idxExplosion_=0;

    void NormalUpdate();
    void ExplodeUpdate();
    void KillUpdate();
    Func_t updater_;
    
    void NormalDraw();
    void ExplodeDraw();
    void NoDraw();
    Func_t drawer_;

public:
    BombShot(const Position2& pos, const Vector2f& vel);
    ~BombShot();
    //Update��Projectile�̂��̂��g�p
    void Update()override;
    void Draw()override;
};


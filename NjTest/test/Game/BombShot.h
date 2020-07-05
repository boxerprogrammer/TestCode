#pragma once
#include "Projectile.h"
class BombShot :
    public Projectile
{
private:
    float angle_;
public:
    BombShot(const Position2& pos, const Vector2f& vel);
    //UpdateはProjectileのものを使用
    //void Update()override;
    void Draw();
};


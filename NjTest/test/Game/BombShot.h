#pragma once
#include "Projectile.h"
class BombShot :
    public Projectile
{
private:
    float angle_;
public:
    BombShot(const Position2& pos, const Vector2f& vel);
    //Update��Projectile�̂��̂��g�p
    //void Update()override;
    void Draw();
};


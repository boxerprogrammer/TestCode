#pragma once
#include "Projectile.h"
class ShurikenShot :
    public Projectile
{
private:
    float angle_ = 0.0f;
public:
    ShurikenShot(const Position2& pos, const Vector2f& vel);
    ~ShurikenShot();
    void Update()override;
    void Draw()override;
};


#pragma once
#include "Projectile.h"
/// <summary>
/// �藠���{��(��ѓ���)
/// </summary>
class ShurikenShot : public Projectile
{
private:
    float angle_ = 0.0f;
public:
    ShurikenShot(const Position2f& pos, const Vector2f& vel, std::shared_ptr<Camera> c);
    ~ShurikenShot();
    void Update()override;
    void Draw()override;
    void OnHit(CollisionInfo& info)override;
};


#pragma once
#include "Projectile.h"
class BombShot :
    public Projectile
{
private:
    float angle_;
private:
    using Func_t = void (BombShot::*)();

    int frame_=0;
    int idxExplosion_=0;

    void NormalUpdate();
    void Explode();
    void ExplodeUpdate();
    void KillUpdate();
    Func_t updater_;
    
    void NormalDraw();
    void ExplodeDraw();
    void NoDraw();
    Func_t drawer_;

public:
    BombShot(const Position2f& pos, const Vector2f& vel, std::shared_ptr<Camera> c);
    ~BombShot();
    //Update‚ÍProjectile‚Ì‚à‚Ì‚ðŽg—p
    void Update()override;
    void Draw()override;
    void OnHit(CollisionInfo&)override;
};


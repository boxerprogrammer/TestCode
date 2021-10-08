#pragma once
#include "../Projectile.h"

class EffectManager;
class Kunai :
    public Projectile
{
private:
    int frame_ = 0;
    int bulletH_ = -1;
    float speed_ = 1.0f;
    float centripetalSpeed_ = 0.0f;
    Vector2f initVel_;
    std::shared_ptr<EffectManager> effectMgr_;
public:
    /// <summary>
    /// 阿修羅弾コンストラクタ
    /// </summary>
    /// <param name="pos">初期座標</param>
    /// <param name="vel">初期速度</param>
    /// <param name="camera">カメラ</param>
    Kunai(const Position2f& pos, const Vector2f& vel, std::shared_ptr<Camera> camera, std::shared_ptr<EffectManager> efk);
    ~Kunai();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void Update()override;

    /// <summary>
    /// 毎フレーム描画
    /// </summary>
    void Draw()override;

    /// <summary>
    /// 衝突時イベント
    /// </summary>
    /// <param name="colInfo">衝突情報</param>
    void OnHit(CollisionInfo& me, CollisionInfo& other)override;
};




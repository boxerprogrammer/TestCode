#pragma once
#include "Projectile.h"
/// <summary>
/// 手裏剣本体(飛び道具)
/// </summary>
class ShurikenShot : public Projectile
{
private:
    int shurikenH_ = -1;
    int shurikenSE_ = -1;
    float angle_ = 0.0f;//表示回転のための角度
public:
    /// <summary>
    /// 手裏剣コンストラクタ
    /// </summary>
    /// <param name="pos">初期座標</param>
    /// <param name="vel">初期速度</param>
    /// <param name="camera">カメラ</param>
    ShurikenShot(const Position2f& pos, const Vector2f& vel, std::shared_ptr<Camera> camera);
    ~ShurikenShot();

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
    void OnHit(CollisionInfo& colInfo)override;
};


#pragma once
#include "../Projectile.h"
/// <summary>
/// プレイヤーが発射する爆弾
/// </summary>
class BombShot :
    public Projectile
{
private:
    float angle_;//表示回転角
    int bombH = -1;
    int explodeSH = -1;
    int explodeH = -1;
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
    /// <summary>
    /// 爆弾クラスコンストラクタ
    /// </summary>
    /// <param name="pos">初期座標</param>
    /// <param name="vel">初期速度</param>
    /// <param name="camera">カメラへの参照</param>
    /// <returns></returns>
    BombShot(const Position2f& pos, const Vector2f& vel, std::shared_ptr<Camera> camera);
    ~BombShot();

    /// <summary>
    /// 毎フレーム座標等更新
    /// </summary>
    void Update()override;

    /// <summary>
    /// 毎フレーム描画
    /// </summary>
    void Draw()override;

    /// <summary>
    /// ヒット時イベント
    /// </summary>
    /// <param name="colInfo">衝突情報</param>
    void OnHit(CollisionInfo& colInfo)override;
};


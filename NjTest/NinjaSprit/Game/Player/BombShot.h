#pragma once
#include "../Projectile.h"
/// <summary>
/// �v���C���[�����˂��锚�e
/// </summary>
class BombShot :
    public Projectile
{
private:
    float angle_;//�\����]�p
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
    /// ���e�N���X�R���X�g���N�^
    /// </summary>
    /// <param name="pos">�������W</param>
    /// <param name="vel">�������x</param>
    /// <param name="camera">�J�����ւ̎Q��</param>
    /// <returns></returns>
    BombShot(const Position2f& pos, const Vector2f& vel, std::shared_ptr<Camera> camera);
    ~BombShot();

    /// <summary>
    /// ���t���[�����W���X�V
    /// </summary>
    void Update()override;

    /// <summary>
    /// ���t���[���`��
    /// </summary>
    void Draw()override;

    /// <summary>
    /// �q�b�g���C�x���g
    /// </summary>
    /// <param name="colInfo">�Փˏ��</param>
    void OnHit(CollisionInfo& colInfo)override;
};


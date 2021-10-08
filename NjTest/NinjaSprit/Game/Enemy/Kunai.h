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
    /// ���C���e�R���X�g���N�^
    /// </summary>
    /// <param name="pos">�������W</param>
    /// <param name="vel">�������x</param>
    /// <param name="camera">�J����</param>
    Kunai(const Position2f& pos, const Vector2f& vel, std::shared_ptr<Camera> camera, std::shared_ptr<EffectManager> efk);
    ~Kunai();

    /// <summary>
    /// ���t���[���X�V
    /// </summary>
    void Update()override;

    /// <summary>
    /// ���t���[���`��
    /// </summary>
    void Draw()override;

    /// <summary>
    /// �Փˎ��C�x���g
    /// </summary>
    /// <param name="colInfo">�Փˏ��</param>
    void OnHit(CollisionInfo& me, CollisionInfo& other)override;
};




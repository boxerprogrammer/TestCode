#pragma once
#include "../Projectile.h"
/// <summary>
/// �藠���{��(��ѓ���)
/// </summary>
class ShurikenShot : public Projectile
{
private:
    int shurikenH_ = -1;
    int shurikenSE_ = -1;
    float angle_ = 0.0f;//�\����]�̂��߂̊p�x
public:
    /// <summary>
    /// �藠���R���X�g���N�^
    /// </summary>
    /// <param name="pos">�������W</param>
    /// <param name="vel">�������x</param>
    /// <param name="camera">�J����</param>
    ShurikenShot(const Position2f& pos, const Vector2f& vel, std::shared_ptr<Camera> camera);
    ~ShurikenShot();

    /// <summary>
    /// ���t���[���X�V
    /// </summary>
    void Update()override;

    /// <summary>
    /// ���t���[���`��
    /// </summary>
    void Draw()override;

    /// <summary>
/// �Փ˔���C�x���g
/// </summary>
/// <param name="me">�Փˏ�񎩕�</param>
/// <param name="another">�Փˏ�񑊎�</param>
    void OnHit(CollisionInfo& me, CollisionInfo& another)override;
};


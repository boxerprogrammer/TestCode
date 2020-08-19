#pragma once
#include "Boss.h"
#include<array>

class EffectManager;
/// <summary>
/// 1�ʃ{�X���C��
/// </summary>
class Asura :
    public Boss
{
private:
    std::array<Vector2f, 4> energyPositions_ = { Vector2f(-128.0f,-420.0f ),
                                Vector2f(128.0f,-420.0f),
                                Vector2f(-100.0f,-300.0f),
                                Vector2f(100.0f,-300.0f)};
    std::array<int, 4> energyTimes_;
    std::shared_ptr<EffectManager> effectManager_;
    int ashuraH_ = -1;
    int frame_ = 0;
    Enemy* MakeClone()override;
    using Update_t =void (Asura::*)();
    Update_t updater_;
    using Draw_t = void (Asura::*)();
    Draw_t drawer_;

    //�o�ꉉ�o���
    void EnteringUpdate();

    
    //�ʏ���
    void NormalUpdate();

    //�_���[�W���
    void DamageUpdate();

    //�ޏꉉ�o���
    void ExitingUpdate();

    //��
    void DeadUpdate();

    //�ʏ�`��
    void NormalDraw();
    void DamageDraw();
    void ExitingDraw();
    void DeadDraw();
    std::vector<Circle> circles_;
public:
    Asura(GameplayingScene*);
    ~Asura()=default;

	void OnHit(CollisionInfo&)override;

	/// <summary>
	/// �U�����󂯂�
	/// </summary>
	/// <param name="damage">�_���[�W�l</param>
	void OnDamage(int damage)override;

	/// <summary>
	/// ���C�x���g
	/// </summary>
	void OnDead()override;

    /// <summary>
    /// �X�V
    /// </summary>
    void Update()override;
    /// <summary>
    /// �`��
    /// </summary>
    void Draw()override;

    const std::vector<Circle>& GetCircles()const override;
};


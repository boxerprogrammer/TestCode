#pragma once
#include "Collider.h"
#include"../../Geometry.h"
#include<memory>
/// <summary>
/// �~�̓����蔻��N���X
/// </summary>
class CircleCollider :
    public Collider
{
private:
    Circle circle_;///<����Ɏg���~
   
public:
    /// <summary>
    /// ���̓����蔻��̂��ƂɂȂ�~��Ԃ�
    /// </summary>
    /// <returns></returns>
    const Circle& GetCircle()const;
    /// <summary>
    /// ���ۂ̍��W��Ԃ�(�~�Ȃ̂Œ��S�_���W)
    /// </summary>
    /// <returns>��������l���������S�_�̍��W</returns>
    const Vector2f AcutualPosition();

    /// <summary>
    /// �����Ȃ����{0,0},0 �u�~�̃R���C�_�[�v�ɂȂ�
    /// </summary>
    /// <param name="owner">������</param>
    /// <param name="tag">���ʃ^�O</param>
    CircleCollider(std::shared_ptr<Character> owner,const char* tag="");

    /// <summary>
    /// �~��񂩂�~�Փ˔������邽�߂̃R���X�g���N�^
    /// </summary>
    /// <param name="owner">������</param>
    /// <param name="circle">�~���</param>
    /// <param name="tag">���ʃ^�O</param>
    CircleCollider(std::shared_ptr<Character> owner,const Circle& circle, const char* tag = "");

    /// <summary>
    /// ���S�Ɣ��a����~�Փˏ������
    /// </summary>
    /// <param name="p">���S</param>
    /// <param name="r">���a</param>
    CircleCollider(std::shared_ptr<Character> chr,const Position2f& p,float r,  const char* tag = "");

    /// <summary>
    /// ���̃R���C�_�[�Ɠ����������ǂ���
    /// </summary>
    /// <param name="other">���R���C�_�[�I�u�W�F�N�g</param>
    /// <returns>true:�������Ă���</returns>
    bool IsHit(std::shared_ptr<Collider> other)override;

    /// <summary>
    /// �R���C�_�[��������悤�ɕ`�悷��
    /// �f�o�b�O���[�h���ɂ̂ݗL��
    /// </summary>
    void Draw()override;
    
};


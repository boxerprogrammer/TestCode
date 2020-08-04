#pragma once
#include "Collider.h"
#include"../Geometry.h"
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
    const Circle& GetCircle()const;
    /// <summary>
    /// ���ۂ̍��W��Ԃ�(�~�Ȃ̂Œ��S�_���W)
    /// </summary>
    /// <returns>��������l���������S�_�̍��W</returns>
    const Vector2f AcutualPosition();

    /// <summary>
    /// �����Ȃ����{0,0},0 �u�~�̃R���C�_�[�v
    /// </summary>
    CircleCollider(std::shared_ptr<Character> chr,const char* tag="");

    /// <summary>
    /// �~��񂩂�~�Փ˔�������
    /// </summary>
    /// <param name="c">�~���</param>
    CircleCollider(std::shared_ptr<Character> chr,const Circle& c, const char* tag = "");

    /// <summary>
    /// ���S�Ɣ��a����~�Փˏ������
    /// </summary>
    /// <param name="p">���S</param>
    /// <param name="r">���a</param>
    CircleCollider(std::shared_ptr<Character> chr,const Position2f& p,float r,  const char* tag = "");

    bool IsHit(std::shared_ptr<Collider> col)override;
    void Draw()override;
    
};


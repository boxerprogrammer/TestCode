#pragma once
#include "Collider.h"
#include"../../Geometry.h"

/// <summary>
/// �J�v�Z���^�����蔻��I�u�W�F�N�g
/// </summary>
class CapsuleCollider :
    public Collider
{
private:
	Capsule capsule_;///<����Ɏg�p����J�v�Z���`��
public:
	CapsuleCollider(std::shared_ptr<Character> owner,const Capsule& cup,const char* tag = "",bool isImmortal=false);
	~CapsuleCollider();
	///�O���炢�����悤�ɎQ�Ƃ�Ԃ�
	///�������ɐL�яk�݂ł���悤��
	Capsule& GetCapsule();

	/// <summary>
	/// �q�b�g�������ǂ�����Ԃ�
	/// </summary>
	/// <param name="col">���葤�R���C�_�[</param>
	/// <returns>true:�ՓˁAfalse:��Փ�</returns>
	bool IsHit(CollisionInfo& col)override;//�D�h���N���X��CollisionInfo�ɕύX

	/// <summary>
	/// �f�o�b�O�p�B�����f�o�b�O�~��f�o�b�O��`��\���������ꍇ��
	///�\������
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ���ۂ̍��W��Ԃ�(�J�v�Z���^�Ȃ̂ŋN�_�̍��W)
	/// </summary>
	/// <returns>��������l�������N�_�̍��W</returns>
	const Vector2f AcutualPosition();
};


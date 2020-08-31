#pragma once
#include "Collider.h"
class FanCollider :
    public Collider
{
public:

	FanCollider(std::shared_ptr<Character> owner, const Capsule& cup, const char* tag = "", bool isImmortal = false);
	~FanCollider();
	///�O���炢�����悤�ɎQ�Ƃ�Ԃ�
	FanShape& GetFanShape();

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
	/// ���ۂ̍��W��Ԃ�(��`�̒��S�_�̍��W)
	/// </summary>
	/// <returns>��������l�������N�_�̍��W</returns>
	const Vector2f AcutualPosition();
};


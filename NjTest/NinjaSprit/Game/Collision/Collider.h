#pragma once

#include<memory>
#include<string>
#include"../../Geometry.h"

constexpr char tag_player_attack[] = "patk";
constexpr char tag_enemy_damage[] = "edmg";

class Character;
class Camera;
/// <summary>
/// �����蔻��\�͂������N���X
/// </summary>
class Collider
{
private:
	std::string tag_;///<�^�O(�g�����ȁc�H)
	std::weak_ptr<Character> owner_;///<������̐������ɂɂ͊֗^���Ȃ��̂Ŏ�Q��
protected:
	bool isDeletable_ = false;//����ł������t���O
	bool isImmortal_ = false;//�s�Ńt���O
	bool isActive_ = true;
	Vector2f GetCameraOffset();
public:
	Collider(std::shared_ptr<Character> owner,const char* tag="",bool immortal=false);
	virtual ~Collider() = default;

	/// <summary>
	/// �폜���Ă����v�H
	/// </summary>
	/// <returns></returns>
	bool IsDeletable()const;
	/// <summary>
	/// ���E
	/// </summary>
	void Suicide();
	
	/// <summary>
	/// �ꎞ�I�ɋ@�\���Ȃ�����
	/// </summary>
	void Sleep();
	/// <summary>
	/// �@�\����悤�ɂ���
	/// </summary>
	void Awake();
	/// <summary>
	/// �����蔻��͋@�\���Ă��邩�H
	/// </summary>
	/// <returns>true:�@�\���� false:�@�\���Ȃ�</returns>
	bool IsActive()const;

	
	const std::string& GetTag()const;

	/// <summary>
	/// �����傪���ɂ܂���
	/// </summary>
	/// <returns>true:���Afalse:�����Ă�</returns>
	bool OwnerIsDead()const;

	/// <summary>
	/// ������̃X�}�[�g�|�C���^��Ԃ�
	/// </summary>
	/// <returns>������̃X�}�[�g�|�C���^</returns>
	std::shared_ptr<Character> GetOwner();

	/// <summary>
	/// �q�b�g�������ǂ�����Ԃ�
	/// </summary>
	/// <param name="col">���葤�R���C�_�[</param>
	/// <returns>true:�ՓˁAfalse:��Փ�</returns>
	virtual bool IsHit(std::shared_ptr<Collider> col)=0;

	/// <summary>
	/// �f�o�b�O�p�B�����f�o�b�O�~��f�o�b�O��`��\���������ꍇ��
	///�\������
	/// </summary>
	virtual void Draw() {};

};

/// <summary>
/// ���������Ƃ��ɓn�����
/// ���̂Ƃ���͑���R���C�_�[�I�u�W�F�N�g�݂̂���
/// �g�����ׂ̈ɂ������Ă���B
/// </summary>
struct CollisionInfo {
	std::shared_ptr<Collider> collider;
};


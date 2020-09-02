#pragma once
#include "Equipment.h"
#include"../../Geometry.h"
class Player;
class CapsuleCollider;
class Camera;
class ShadowClone;
class FanCollider;
/// <summary>
/// �a���G�t�F�N�g�p�\����(��`������)
/// �s�Ϗ����Fv1��v2�͎��v���ł��鎖
///          :v1�̑傫����v2�̑傫���͓����ł��鎖
/// </summary>
struct Slash {
	Position2f center; //���S���W
	Vector2f v1; //��`�̒[�x�N�g���@
	Vector2f v2; //��`�̒[�x�N�g���A
	Slash() :center(0, 0), v1(0, 0), v2(0, 0) {}//�����ł�
	Slash(const Position2f& p, const Vector2f& inv1, const Vector2f& inv2);
	Slash(const Position2f& p, float r, float angle);
	Slash(const Position2f& p, const Vector2f& inv, float angle);
	void Draw(int graphH,int psH=-1,int normalH=-1,unsigned color=0xffffff,bool isTurn=false);
	float Radius()const;
	void AddAngle1(float angle);
	void AddAngle2(float angle);
	float GetAngle()const;
};

/// <summary>
/// ������
/// </summary>
class SwordEquip : public Equipment
{
private:
	//��]����
	enum class RotateDir {
		cw,
		ccw
	};
	RotateDir rotDir_= RotateDir::cw;
	Slash slash_;
	ShadowClone* shadow_ = nullptr;
	int normalH_ = -1;
	FanCollider* fanCollider_ = nullptr;
	int frame_=0;
	std::shared_ptr<Player>& player_;
	float swingTargetAngle_ = 0.0f;
	float swingPerAngle_ = 0.0f;
	float currentAngle_ = 0.0f;
	int swingFrame_ = 0;
	Vector2f direction_;

public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="collisionMgr">�����蔻��}�l�[�W��</param>
	/// <param name="camera">�J����</param>
	SwordEquip(std::shared_ptr<Player>& player,
		std::shared_ptr<CollisionManager> collisionMgr,
		std::shared_ptr<Camera> camera, ShadowClone* shadow = nullptr);
	/// <summary>
	/// �����U���������܂�
	/// </summary>
	/// <param name="player">�v���C���[�ւ̎Q��</param>
	/// <param name="input">���͎Q��</param>
	void Attack(const Player& player, const Input& input, Vector2f offset = Vector2f::ZERO)override;
	/// <summary>
	/// �ǉ�����(�U��񂵂̂��߂����̃��\�b�h)
	/// </summary>
	/// <param name="input">���͏��</param>
	void AdditionalInput(const Input& input);
	/// <summary>
	/// ���t���[���X�V�p
	/// </summary>
	void Update()override;
	/// <summary>
	/// �`��p
	/// </summary>
	void Draw()override;
};


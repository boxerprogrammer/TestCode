#pragma once
#include"../Geometry.h"
#include<memory>
class Player;
/// <summary>
/// �J�����N���X
/// </summary>
class Camera
{
private:
	std::shared_ptr<Player> player_;
	Position2f pos_;//�J�����̒��S���W
	Size stageSize_;///<�f�t�H���g��800x600�ɂ��Ƃ�
public:
	/// <summary>
	/// �J�����R���X�g���N�^
	/// </summary>
	/// <param name="p">�v���C���[�ւ̎Q��</param>
	Camera();
	~Camera();

	/// <summary>
	/// �Ď��p�v���C���[���Z�b�g
	/// </summary>
	/// <param name="player">�v���C���[�I�u�W�F�N�g</param>
	void SetPlayer(std::shared_ptr<Player> player);

	/// <summary>
	/// �X�e�[�W�̌��E�T�C�Y���Z�b�g����
	/// </summary>
	/// <param name="size">�X�e�[�W�S�̂̃T�C�Y</param>
	void SetStageSize(const Size& size);

	/// <summary>
	/// �J�������W�̍X�V(�v���C���[�ƃX�e�[�W���E�Ō��܂�)
	/// </summary>
	void Update();

	/// <summary>
	/// ���݂̃J�������W��Ԃ�
	/// </summary>
	/// <returns>�J�������W</returns>
	const Position2f& GetPosition()const;

	/// <summary>
	/// �J�����̎B�e�͈͂�Ԃ�
	/// </summary>
	/// <returns>�B�e�͈͋�`</returns>
	const Rect GetViewRange()const;

	/// <summary>
	/// �`����ǂꂭ�炢���点�΂������I�t�Z�b�g�x�N�g����Ԃ�
	/// </summary>
	/// <returns>�I�t�Z�b�g</returns>
	const Vector2f ViewOffset()const;

};


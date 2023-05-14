#pragma once

//#include�ł͂Ȃ��v���g�^�C�v�錾���g�p����
class SceneManager;//�V�[���}�l�[�W��
class InputState;//���̓X�e�[�g
//�Ȃ̂ŁA���̎��_�ł�SceneManager��InputState��
//�|�C���^�������͎Q�ƂƂ��Ă��������Ȃ��B

/// <summary>
/// �V�[�����N���X
/// </summary>
class Scene
{
protected:
	SceneManager& manager_;//�V�[���}�l�[�W���ւ̎Q��
public:
	Scene(SceneManager& manager) :manager_(manager) {}
	virtual ~Scene() {};
	///�V�[�����X�V����
	virtual void Update(const InputState& input) = 0;
	///�V�[����`�悷��
	virtual void Draw() = 0;
};


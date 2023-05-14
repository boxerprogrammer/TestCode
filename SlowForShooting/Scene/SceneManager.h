#pragma once

#include<deque>

//�v���g�^�C�v�錾
class Scene;//�V�[�����N���X
class InputState;//���̓X�e�[�g

//�V�[���Ď��N���X
class SceneManager {
private:
	//�u�����s���v�̃V�[����؂�ւ��Ă��������̂�
	//�Q�Ƃł͂Ȃ��|�C���^�Ƃ��Đ錾
	std::deque<Scene*> scenes_;
public:
	/// <summary>
	/// �V�[���̐؂�ւ����s��
	/// </summary>
	/// <param name="scene">�؂�ւ��������V�[���̃A�h���X</param>
	void ChangeScene(Scene* scene);

	/// <summary>
	/// �V�[������ɐς�
	/// Update�Ŏ��s�����̂͏�ɐς܂ꂽ�V�[���̂�
	/// </summary>
	void PushScene(Scene* scene);

	/// <summary>
	/// ��ԏ�̃V�[�����폜����
	/// �������A���̌��ʃX�^�b�N����ɂȂ邱�Ƃ̂Ȃ��悤��
	/// </summary>
	void PopScene();


	/// <summary>
	/// �e�V�[����Update(�X�V)���s��
	/// </summary>
	/// <param name="input">���̓X�e�[�g</param>
	void Update(const InputState& input);
	/// <summary>
	/// �e�V�[���̕`����s��
	/// </summary>
	void Draw();
};

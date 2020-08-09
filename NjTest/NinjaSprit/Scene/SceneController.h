#pragma once
#include<memory>
#include<deque>
#include<functional>
class Scene;
class Input;
class Application;

/// <summary>
/// �V�[���؂�ւ��N���X
/// </summary>
class SceneController
{
	friend Application;
private:
	SceneController();
	std::deque<std::shared_ptr<Scene>> scene_;
	std::function<void(void)> postDrawExecuter_;
public:

	void ClearScene();

	///�V�[�����V�[���X�^�b�N�̓��ɓ����
	void PushScene(Scene*);
	///�V�[���X�^�b�N�̐擪���폜����
	void PopScene();

	///�V�[���X�^�b�N����x�N���A���Ă���
	///�w��̃V�[���֑J�ڂ���
	///<param name="scene">�؂�ւ���̃V�[���I�u�W�F�N�g</param>
	///<attention>�O�̃V�[���͌ďo����ɍ폜
	///����邽�߁A���̊֐��̌Ăяo������
	///�폜�Ώۂ������ꍇ�ɂ́A�֐��Ăяo�����
	///�������s���Ă͂Ȃ�Ȃ�</attension>
	void CleanChangeScene(Scene* scene);
	
	///�V�[����؂�ւ���
	///<param name="scene">�؂�ւ���̃V�[���I�u�W�F�N�g</param>
	///<attention>�O�̃V�[���͌ďo����ɍ폜
	///����邽�߁A���̊֐��̌Ăяo������
	///�폜�Ώۂ������ꍇ�ɂ́A�֐��Ăяo�����
	///�������s���Ă͂Ȃ�Ȃ�</attension>	
	void ChangeScene(Scene*);

	///<summary>�����Ă���擪�V�[����Update���Ă�</summary>
	///<param name="input">���͏��</param>
	void Update(const Input& input);
	///�����Ă���V�[�����ׂĂ�`�悷��
	void Draw();
};


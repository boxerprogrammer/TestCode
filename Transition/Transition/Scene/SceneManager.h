#pragma once

#include<memory>
#include<list>
class Scene;
class Input;
class FileManager;
class SceneManager
{
private:
	FileManager& fileManager_;
	std::list<std::shared_ptr<Scene> > scenes_;//���ݎ��s���̃V�[��
public:
	FileManager& GetFileManager();
	SceneManager(FileManager& fileManager);
	/// <summary>
	/// �擪��(Update���Ă΂��)�V�[����؂�ւ���
	/// </summary>
	/// <param name="scene">�؂�ւ���̃V�[��</param>
	void ChangeScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// �V�[�����v�b�V������B�X�^�b�N�̐���������
	/// ��ԏ�̃V�[����Update�����Ă΂�܂���
	/// </summary>
	/// <param name="scene">��ɐςރV�[��</param>
	void PushScene(std::shared_ptr<Scene> scene);
	/// <summary>
	/// �X�^�b�N�̓��̃V�[�����폜����
	/// �������A�X�^�b�N��ɃV�[����1�����Ȃ��ꍇ��
	/// �폜���Ȃ�
	/// </summary>
	void PopScene();
	/// <summary>
	/// �����Ă���V�[���X�^�b�N�̐擪��Update���Ăяo��
	/// </summary>
	/// <param name="input">����IO�p�����[�^</param>
	void Update(Input& input);

	/// <summary>
	/// �����Ă���V�[�����ׂĂ�`�悷��
	/// �������`�揇�͉�����`�悳��܂�
	/// </summary>
	void Draw();
};


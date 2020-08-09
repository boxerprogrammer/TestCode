#pragma once
#include<memory>

//�A�v���P�[�V�����N���X
//�S�Ă̂������ƂɂȂ�N���X
//main�̑���B���ۂ�main��
//�G���g���|�C���g�̖��������ɂ��Ă���
class SceneController;
struct Size;
struct Rect;
//�\�����ׂ���ʂ̏�������
class Viewport {
public:
	Size GetSize()const;//�Q�[�����(�r���[�|�[�g)�̃T�C�Y��Ԃ�
	Rect GetRect()const;//�Q�[����ʂ̃r���[�|�[�g��`��Ԃ�
};

/// <summary>
/// �A�v���P�[�V�����S�̂��R���g���[��
/// </summary>
class Application
{
private:
	Viewport viewport_;
	std::unique_ptr<SceneController> sceneController_;

	Application() = default;
	Application(const Application&) = delete;
	void operator = (const Application&) = delete;
public:
	/// <summary>
	/// �V���O���g���C���X�^���X��Ԃ�
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static Application& Instance();
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>�����������������ǂ���</returns>
	bool Initialize();
	/// <summary>
	/// ���C�����[�v�����s����
	/// </summary>
	void Run();

	/// <summary>
	///�㏈��
	/// </summary>
	void Terminate();

	///<summary>
	///�A�v���P�[�V�������I������
	///</summary>
	void Exit();

	/// <summary>
	/// �Q�[����ʂ̃r���[�|�[�g����Ԃ�
	/// </summary>
	/// <returns></returns>
	const Viewport& GetViewport()const;

};


#pragma once
#include<vector>
#include<memory>
class Camera;
class Background
{
private:
	std::vector<int> bgH_;//�w�i���C���[�n���h��
	int frame_=0;//���݂̃t���[���ԍ�
	std::shared_ptr<Camera> camera_;
public:
	/// <summary>
	/// �w�i�N���X�R���X�g���N�^
	/// </summary>
	/// <param name="camera">�J�����ւ̎Q��</param>
	Background(std::shared_ptr<Camera> camera);
	~Background();

	/// <summary>
	/// �J�������W�ɂ��X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �J�������W���l�������`��
	/// </summary>
	void Draw();
};


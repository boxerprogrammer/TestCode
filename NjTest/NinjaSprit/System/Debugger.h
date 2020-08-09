#pragma once

struct Circle;
struct Segment;
struct Capsule;


/// <summary>
/// �f�o�b�O�x���ɕK�v�ȏ������s�����߂�
/// �N���X��V���O���g����
/// </summary>
class Debugger
{
private:
	Debugger()=default;
	Debugger(const Debugger&) = delete;
	void operator =(const Debugger&) = delete;
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>Debugger�C���X�^���X</returns>
	static Debugger& Instance();

	/// <summary>
	/// ���f�o�b�O���[�h���ǂ���
	/// </summary>
	/// <returns>true�F�f�o�b�O���[�h</returns>
	bool IsDebugMode()const;

	/// <summary>
	/// ���t���[���Ă΂�����ϐ����X�V���Ă���
	/// </summary>
	void Update();

	/// <summary>
	/// �p�t�H�[�}���X�J�E���^��\������
	/// </summary>
	void DisplayPerformance();

	/// <summary>
	/// �~�`��
	/// </summary>
	/// <param name="circle">�~</param>
	/// <param name="color">�F</param>
	/// <param name="xoffset">X�����I�t�Z�b�g</param>
	/// <param name="yoffset">Y�����I�t�Z�b�g</param>
	/// <param name="thickness">���̑���</param>
	void Draw(const Circle& circle,unsigned int color,float xoffset=0.0f, float yoffset = 0.0f,float thickness=1.0f);
	/// <summary>
	/// �����`��
	/// </summary>
	/// <param name="seg">����</param>
	/// <param name="color">�F</param>
	/// <param name="xoffset">X�����I�t�Z�b�g</param>
	/// <param name="yoffset">Y�����I�t�Z�b�g</param>
	/// <param name="thickness">���̑���</param>
	void Draw(const Segment& seg, unsigned int color, float xoffset = 0.0f, float yoffset = 0.0f, float thickness = 1.0f);
	/// <summary>
	/// �J�v�Z���`��
	/// </summary>
	/// <param name="capsule">�J�v�Z��</param>
	/// <param name="color">�F</param>
	/// <param name="xoffset">X�����I�t�Z�b�g</param>
	/// <param name="yoffset">Y�����I�t�Z�b�g</param>
	/// <param name="thickness">���̑���</param>
	void Draw(const Capsule& capsule, unsigned int color,  float xoffset = 0.0f, float yoffset = 0.0f, float thickness = 1.0f);

};


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

	void Draw(const Circle& circle,unsigned int color,float xoffset=0.0f, float yoffset = 0.0f,float thickness=1.0f);
	void Draw(const Segment& seg, unsigned int color, float xoffset = 0.0f, float yoffset = 0.0f, float thickness = 1.0f);
	void Draw(const Capsule& capsule, unsigned int color,  float xoffset = 0.0f, float yoffset = 0.0f, float thickness = 1.0f);

};


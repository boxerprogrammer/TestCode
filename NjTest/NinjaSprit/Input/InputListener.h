#pragma once
class Input;
/// <summary>
/// ���̓��X�i���N���X
///��{�I�ɃI�u�U�[�o�p�^�[���̃C���^�[�t�F�[�X��񋟂���̂�
/// </summary>
class InputListener
{
public:
	/// <summary>
	/// �I�u�U�[�o�ɓ��̓C�x���g��m�点��
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Notify(const Input& input) = 0;
};


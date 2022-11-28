#pragma once

class SceneManager;
struct InputState;
/// <summary>
/// �e�V�[���̊��N���X
/// ���ׂẴV�[���̒��ۂ�����킷
/// </summary>
class Scene {
protected:
	SceneManager& manager_;
public:
	Scene(SceneManager& manager) :manager_(manager) {}
	virtual ~Scene() {};
	/// <summary>
	/// �V�[���̓��e�̍X�V
	/// </summary>
	/// <param name="input">���͏��/param>
	/// <remarks>input�͌Ăяo�����Ŏ��̂�����đ��</remarks>
	virtual void Update(const InputState& input) = 0;
	/// <summary>
	/// �V�[���̕`�掞�ɌĂяo�����
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// �w��̃L�[�̉����ꂽ�u�Ԃ�true��Ԃ�	
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <param name="key">���̓L�[�w��l</param>
	/// <returns>true:�����ꂽ�Afalse:������ĂȂ�or��������</returns>
	/// <example>
	/// if(IsTriggered(input,KEY_INPUT_RETURN))
	/// </example>
	bool IsTriggered(const InputState& input, char key);
};

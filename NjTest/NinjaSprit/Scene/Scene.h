#pragma once

//struct Input {
//	char keyState[256];
//	char lastKeyState[256];
//
//	void Update();
//
//};
class Input;
class SceneController;
/// <summary>
/// �V�[�����N���X
/// </summary>
class Scene
{
	friend SceneController;
protected:
	SceneController& controller_;
	Scene() = default;
	Scene(SceneController& );
public:
	/// <summary>
	/// �V�[���X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(const Input& input) = 0;

	/// <summary>
	/// �V�[���`��
	/// </summary>
	virtual void Draw() = 0;
	virtual ~Scene() = default;
};


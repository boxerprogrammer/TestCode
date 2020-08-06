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
/// シーン基底クラス
/// </summary>
class Scene
{
	friend SceneController;
protected:
	SceneController& controller_;
	Scene() = default;
	Scene(SceneController& );
public:
	virtual void Update(const Input&) = 0;
	virtual void Draw() = 0;
	virtual ~Scene() = default;
};


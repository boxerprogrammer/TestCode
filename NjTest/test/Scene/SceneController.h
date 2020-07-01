#pragma once
#include<memory>
#include<deque>
class Scene;
class Input;
class Application;
class SceneController
{
	friend Application;
private:
	SceneController();
	std::deque<std::shared_ptr<Scene>> scene_;
public:
	void PushScene(Scene*);
	void PopScene();
	void CleanChangeScene(Scene*);
	void ChangeScene(Scene*);
	void Update(const Input&);
	void Draw();
};


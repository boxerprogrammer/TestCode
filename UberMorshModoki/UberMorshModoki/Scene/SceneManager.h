#pragma once
#include<memory>
class Scene;
struct InputState;
class SceneManager {
private:
	std::unique_ptr<Scene> scene_ = nullptr;
public:
	SceneManager() {}
	void ChangeScene(std::unique_ptr<Scene>&& scene);
	void Update(const InputState& input);
	void Draw();
};

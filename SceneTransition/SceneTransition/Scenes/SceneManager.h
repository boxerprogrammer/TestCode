#pragma once
class Scene;
struct InputState;
class SceneManager {
private:
	Scene* scene_ = nullptr;
public:
	SceneManager() {}
	void ChangeScene(Scene* scene);
	void Update(const InputState& input);
	void Draw();
};

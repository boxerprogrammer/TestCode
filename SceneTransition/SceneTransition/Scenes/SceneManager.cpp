#include "SceneManager.h"
#include"Scene.h"

void 
SceneManager::ChangeScene(Scene* scene) {
	if (scene_ != nullptr) {
		delete scene_;
	}
	scene_ = scene;
}
void 
SceneManager::Update(const InputState& input) {
	scene_->Update(input);
}
void 
SceneManager::Draw() {
	scene_->Draw();
}
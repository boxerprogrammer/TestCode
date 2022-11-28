#include "SceneManager.h"
#include"Scene.h"

using namespace std;

void 
SceneManager::ChangeScene(unique_ptr<Scene>&& scene) {
	scene_=std::move(scene);
}
void 
SceneManager::Update(const InputState& input) {
	scene_->Update(input);
}
void 
SceneManager::Draw() {
	scene_->Draw();
}
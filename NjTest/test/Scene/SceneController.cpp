#include "SceneController.h"
#include"TitleScene.h"
#include<cassert>

using namespace std;

void
SceneController::PushScene(Scene* scene) {
	scene_.emplace_front(scene);
}

void
SceneController::PopScene() {
	scene_.erase(scene_.begin());
	assert(!scene_.empty());
}

void 
SceneController::CleanChangeScene(Scene* scene) {
	scene_.clear();
	scene_.emplace_front(scene);
}

SceneController::SceneController() {
	scene_.emplace_front(new TitleScene(*this));
}

void 
SceneController::ChangeScene(Scene* scene) {
	scene_.pop_front();
	scene_.emplace_front(scene);
}
void 
SceneController::Update(const Input& input) {
	assert(!scene_.empty());
	scene_.front()->Update(input);
	
}
void 
SceneController::Draw() {
	auto rit = scene_.rbegin();
	for (; rit != scene_.rend(); ++rit) {
		(*rit)->Draw();
	}
}
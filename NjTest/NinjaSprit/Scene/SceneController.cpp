#include "SceneController.h"
#include"TitleScene.h"
#include<cassert>

using namespace std;

void
SceneController::PushScene(Scene* scene) {
	postDrawExecuter_ = [this, scene]() {
		scene_.emplace_front(scene);
	};
}

void
SceneController::PopScene() {
	postDrawExecuter_ = [this]() {
		scene_.erase(scene_.begin());
		assert(!scene_.empty());
	};
}


SceneController::SceneController() {
	scene_.emplace_front(new TitleScene(*this));
	postDrawExecuter_ = []() {};
	//scene_.reset(new TitleScene(*this));//OK
}

void 
SceneController::CleanChangeScene(Scene* scene) {
	postDrawExecuter_ = [this, scene]() {
		scene_.clear();
		scene_.emplace_front(scene);
	};
}

void 
SceneController::ChangeScene(Scene* scene) {
	postDrawExecuter_ = [this, scene]() {
		scene_.pop_front();
		scene_.emplace_front(scene);
	};
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
	postDrawExecuter_();
	postDrawExecuter_ = []() {};
}
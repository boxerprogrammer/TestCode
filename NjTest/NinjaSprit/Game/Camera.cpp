#include "Camera.h"
#include"Player/Player.h"
#include"../System/Application.h"
#include<algorithm>

using namespace std;

void
Camera::SetPlayer(std::shared_ptr<Player> player) {
	player_ = player;
}

Camera::Camera(): stageSize_(800,600) {
}
Camera::~Camera() {

}
/// <summary>
/// �X�e�[�W�̌��E�T�C�Y���Z�b�g����
/// </summary>
/// <param name="size">�X�e�[�W�S�̂̃T�C�Y</param>
void 
Camera::SetStageSize(const Size& size) {
	stageSize_ = size;
}



void
Camera::Lock() {
	isLocked = true;
}

/// <summary>
/// �J�������W�̍X�V(�v���C���[�ƃX�e�[�W���E�Ō��܂�)
/// </summary>
void 
Camera::Update() {
	if (isLocked)return;
	auto& app = Application::Instance();
	auto& viewport=app.GetViewport();
	const auto& cameraSize=viewport.GetSize();
	pos_ = player_->GetPosition();
	pos_.y = cameraSize.h / 2.0f;
	//X�͈̔͐���
	pos_.x = Clamp(pos_.x,cameraSize.w/2.0f,stageSize_.w-cameraSize.w/2.0f);
	
}

/// <summary>
/// ���݂̃J�������W��Ԃ�
/// </summary>
/// <returns>�J�������W</returns>
const Position2f& 
Camera::GetPosition()const {
	return pos_;
}

/// <summary>
/// �J�����̎B�e�͈͂�Ԃ�
/// </summary>
/// <returns>�B�e�͈͋�`</returns>
const Rect 
Camera::GetViewRange()const {
	auto& app = Application::Instance();
	auto& viewport = app.GetViewport();
	const auto& cameraSize = viewport.GetSize();
	return Rect(
		static_cast<int>(pos_.x- cameraSize.w/2),
		static_cast<int>(pos_.y- cameraSize.h/2),
		cameraSize.w,
		cameraSize.h);
}

const Vector2f 
Camera::ViewOffset()const {
	auto& app = Application::Instance();
	auto& viewport = app.GetViewport();
	const auto& cameraSize = viewport.GetSize();
	return Vector2f(cameraSize.w / 2 -pos_.x, cameraSize.h / 2 -pos_.y);
}
#include "Player.h"
#include"../DrawFunctions.h"
#include"../InputState.h"
#include"../SoundManager.h"
#include"Force.h"
#include<DxLib.h>

//���@�摜�P������̕��ƍ���
constexpr int img_width = 26;
constexpr int img_height = 21;
constexpr float draw_scale = 2.0f;

constexpr int burst_img_width = 32;
constexpr int burst_img_height = 32;
constexpr float burst_draw_scale = 1.0f;
constexpr int burst_frame_num = 8;//�A�j���[�V��������
constexpr int burst_frame_speed = 10;//�P���ɕK�v�ȃt���[����
constexpr int ultimate_frames = 180;//���G����

void Player::NormalUpdate(const InputState& input)
{
	constexpr float speed = 4.0f;
	Vector2 vel = { 0.0f,0.0f };//���x�x�N�g��
	if (input.IsPressed(InputType::up)) {//��
		imgIdx_ = 2;
		vel.y = -1.0f;
	}
	else if (input.IsPressed(InputType::down)) {//��
		imgIdx_ = 1;
		vel.y = 1.0f;
	}
	else {//������������ĂȂ�
		imgIdx_ = 0;
	}
	if (input.IsPressed(InputType::left)) {//��
		vel.x = -1.0f;
	}
	else if (input.IsPressed(InputType::right)) {//�E
		vel.x = 1.0f;
	}
	if (vel.Length() > 0.0f) {//�x�N�g���̑傫�����P�ȏ�̎��ɂ݈̂ړ����s��
		vel.Normalize();//�����ɂ�����炸�傫�����P�ɂ���
		vel *= speed;//�P�̕����ɃX�s�[�h����Z����
	}
	if (input.IsTriggered(InputType::switching)) {
		force_->Switch();
	}
	//���̌��ʁA�����ɂ�����炸�X�s�[�h��2.0f�ɂȂ�B
	//�����pos_�ɉ��Z����B
	rect_.center += vel;
	force_->Update();
	ultimateTimer_ = std::max(ultimateTimer_-1, 0);
}

void Player::NormalDraw()
{
	if ((ultimateTimer_ / 10) % 2 != 0) {
		return;
	}
	my::MyDrawRectRotaGraph(
		rect_.center.x, rect_.center.y,//�\�����W���S
		imgIdx_ * img_width, 0,//�؂��荶��
		img_width, img_height,//��,����
		draw_scale, 0.0f,//�g�嗦�A��]�p�x
		handle_, true);
	force_->Draw();
	rect_.Draw(0xaaffaa);
}

void Player::BurstUpdate(const InputState& input)
{
	imgIdx_++;
	if (imgIdx_ == burst_frame_num * burst_frame_speed) {
		updateFunc_ = &Player::NormalUpdate;
		drawFunc_ = &Player::NormalDraw;
		ultimateTimer_ = ultimate_frames;
		imgIdx_ = 0;
	}
}

void Player::BurstDraw()
{
	my::MyDrawRectRotaGraph(
		rect_.center.x, rect_.center.y,//�\�����W���S
		(imgIdx_/burst_frame_speed) * burst_img_width, 0,//�؂��荶��
		burst_img_width, burst_img_height,//��,����
		burst_draw_scale, 0.0f,//�g�嗦�A��]�p�x
		burstH_, true);
}

Player::Player() :rect_( { 320.0f,240.0f } ,{26,21} )
{
	handle_ = my::MyLoadGraph(L"Data/img/game/player.png");
	burstH_ = my::MyLoadGraph(L"Data/img/game/player_burst.png");
	force_ = std::make_shared<Force>(*this);
	updateFunc_ = &Player::NormalUpdate;
	drawFunc_ = &Player::NormalDraw;
}

void Player::SetHasMissile(bool hasMissile)
{
	hasMissile_ = hasMissile;
}

bool Player::HasMissile()const
{
	return hasMissile_;
}

bool Player::IsCollidable() const
{
	return (updateFunc_==&Player::NormalUpdate) && ultimateTimer_==0;
}

const Position2& Player::GetPosition() const
{
	return rect_.center;
}

void Player::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}
void Player::Draw()
{
	(this->*drawFunc_)();
	//DrawGraph(0, 0, handle_, true);

	//DrawRectGraph(
	//	pos_.x, pos_.y,//�\�����W
	//	imgIdx_ * img_width, 0,//�؂��荶��
	//	img_width, img_height,//��,����
	//	handle_, true);


}
void
Player::OnDamage() {
	updateFunc_ = &Player::BurstUpdate;
	drawFunc_ = &Player::BurstDraw;
	imgIdx_ = 0;
	SoundManager::GetInstance().Play(L"dead");
}

const Rect& Player::GetRect() const
{
	return rect_;
}

std::shared_ptr<Force> Player::GetForce()
{
	return force_;
}

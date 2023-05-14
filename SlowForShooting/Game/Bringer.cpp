#include "Bringer.h"

#include"../DrawFunctions.h"
#include"BulletFactory.h"
#include"ItemFactory.h"
#include"Player.h"
#include<DxLib.h>


constexpr int anim_frame_speed = 10;//�P���ɕK�v�ȃt���[����
constexpr int anim_frame_num = 4;//�A�j���[�V��������
constexpr int img_width = 32;
constexpr int img_height = 32;
constexpr float img_scale = 2.0f;


//�����n�̒萔
constexpr int burst_img_width = 48;
constexpr int burst_img_height = 48;
constexpr float burst_img_scale = 1.0f;
constexpr int burst_frame_num = 8;//�A�j���[�V��������
constexpr int burst_frame_speed = 10;//�P���ɕK�v�ȃt���[����
constexpr float bullet_speed = 2.0f;//

void Bringer::NormalUpdate()
{
	vel_.x = -4.0f;
	vel_.y = -sinf(angle_);
	angle_ += 0.01f;
	vel_.Normalize();
	vel_ *= 1.0f;

	rect_.center += vel_;

	idx_ = (idx_ + 1) % (anim_frame_speed * anim_frame_num);
	if (rect_.center.x < -100) {
		isEnabled_ = false;
	}
	fireFrame_--;
	if (fireFrame_ == 0) {
		fireFrame_ = (fire_interval + //60�t���[��
			GetRand(fire_rand_rate * 2)  //+0�`20�t���[��
			- fire_rand_rate);//-10�t���[��=50�`70�t���[��
		//���@�_���e�����
		//���@�_���x�N�g�� = �I�_(�v���C���[���W) - �n�_(�G�@���g�̍��W)
		auto vel = player_->GetPosition() - rect_.center;
		if (vel.SQLength() == 0.0f) {
			//�G�@�Ǝ��@���d�Ȃ��Ă�Ƃ��͊��̕���
			//�^���ɔ�΂��Ă����܂��B
			vel = { -bullet_speed,0.0f };
		}
		else {
			//�󋵂ɂ����vel���ω����Ă��܂����߁A�������񐳋K������(�傫��1)
			vel.Normalize();
			//vel *= bullet_speed;//�X�s�[�h����Z����
		}
		bulletFactory_->Create(BulletType::Zako, rect_.center, vel);
	}
}

void Bringer::NormalDraw()
{
	int imgX = (idx_ / anim_frame_speed) * img_width;
	my::MyDrawRectRotaGraph(
		rect_.center.x, rect_.center.y,
		imgX, 0,
		img_width, img_height,
		img_scale, 0.0f,
		handle_, true);

	rect_.Draw(0x00ff00);
}

void Bringer::BurstUpdate()
{
	idx_++;
	if (idx_ == burst_frame_num * burst_frame_speed) {
		itemFactory_->AddItem(rect_.center, itemType_);
		isEnabled_ = false;
	}
}

void Bringer::BurstDraw()
{
	int imgX = (idx_ / burst_frame_speed) * burst_img_width;
	my::MyDrawRectRotaGraph(
		rect_.center.x, rect_.center.y,
		imgX, 0,
		burst_img_width, burst_img_height,
		burst_img_scale, 0.0f,
		burstH_, true);
}

Bringer::Bringer(std::shared_ptr<Player> player, const Position2& pos, int handle, int burstH, 
	ItemType itemType,
	std::shared_ptr<BulletFactory> bulletFactory, std::shared_ptr<ItemFactory> itemFactory):
    Enemy(player,pos,bulletFactory),
    itemFactory_(itemFactory),
    handle_(handle),
    burstH_(burstH),
	itemType_(itemType),
    drawFunc_(&Bringer::NormalDraw),
    updateFunc_(&Bringer::NormalUpdate)
{
	rect_ = { pos, { static_cast<int>(img_width * img_scale),static_cast<int>(img_height * img_scale) } };
}

void Bringer::OnDamage(int damage)
{
	//�_���[�W������ƁA�����A�j���[�V�����Ɉڍs����
	updateFunc_ = &Bringer::BurstUpdate;
	drawFunc_ = &Bringer::BurstDraw;
}

bool Bringer::IsCollidable() const
{
	return isEnabled_ && (updateFunc_ == &Bringer::NormalUpdate);
}

void Bringer::Update()
{
	(this->*updateFunc_)();
}

void Bringer::Draw()
{
	(this->*drawFunc_)();
}

std::shared_ptr<Enemy> Bringer::Clone()
{
	return std::make_shared<Bringer>(*this);
}

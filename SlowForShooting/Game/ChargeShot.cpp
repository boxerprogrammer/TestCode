#include "ChargeShot.h"
#include"../DrawFunctions.h"
#include"../SoundManager.h"

constexpr int img_width = 63;
constexpr int img_height = 32;
constexpr float img_scale = 1.0f;
constexpr int anim_num = 4;//�A�j���[�V�����̖���
constexpr int anim_frame_per_img = 4;//1��������̃t���[����


ChargeShot::ChargeShot(int handle):vel_(10.0f,0.0f),handle_(handle)
{
	rect_.size = { static_cast<int>(img_width * img_scale),
					static_cast<int>(img_height * img_scale) };
	
}

const Rect& ChargeShot::GetRect() const
{
	return rect_;
}

void ChargeShot::Fire(const Position2& pos)
{
	rect_.center = pos;
	isEnabled_ = true;
	SoundManager::GetInstance().Play(L"chargeshot");
}

bool ChargeShot::IsEnabled() const
{
	return isEnabled_;
}

void ChargeShot::Update()
{
	rect_.center+= vel_;//�e�����ݑ��x�ňړ�������

	//�摜�̃C���f�b�N�X��؂�ւ��Ă�
	animFrame_ = (animFrame_ + 1) % (anim_num * anim_frame_per_img);

	//�e����ʊO�ɏo����A���񂾂��Ƃɂ���
	if (rect_.center.x < -img_width || 640 + img_width < rect_.center.x) {
		isEnabled_ = false;
	}
}

void ChargeShot::Draw()
{
	my::MyDrawRectRotaGraph(
		static_cast<int>(rect_.center.x), static_cast<int>(rect_.center.y),
		(animFrame_/anim_frame_per_img) * img_width, 0,
		img_width, img_height,
		img_scale, 0.0f,
		handle_, true);

	rect_.Draw(0xaaaaff);
}

#include "WipeTransitor.h"
#include<DxLib.h>
#include<array>
#include<algorithm>
#include"../Application.h"

using namespace std;

WipeTransitor::WipeTransitor(TransitDirection dir, int interval)
{
	float left, right, top, bottom;
	left = right = top = bottom = 1.0f;

	switch (dir) {
	case TransitDirection::left:
		right = 0.0f;
		break;
	case TransitDirection::right:
		left = 0.0f;
		break;
	case TransitDirection::up:
		bottom = 0.0f;
		break;
	case TransitDirection::down:
		top = 0.0f;
		break;
	default:
		break;
	}
	const auto& wsize = Application::GetInstance().GetWindowSize();
	array<COLOR_U8, 4> colors;
	colors[0].r = colors[0].g = colors[0].b = colors[0].a = 255 * left * top;//����
	colors[1].r = colors[1].g = colors[1].b = colors[1].a = 255 * right * top;//�E��
	colors[2].r = colors[2].g = colors[2].b = colors[2].a = 255 * left * bottom;//����
	colors[3].r = colors[3].g = colors[3].b = colors[3].a = 255 * right * bottom;//�E��

	array<VERTEX2D,6> vertices;

	// ����̒��_�̏����Z�b�g( 1�|���S���ڂ̑�1���_ )
	vertices[0].pos.x = 0;
	vertices[0].pos.y = 0;
	vertices[0].pos.z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].dif = colors[0];
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	// �E��̒��_�̏����Z�b�g( 1�|���S���ڂ̑�2���_ )
	vertices[1].pos.x = wsize.w;
	vertices[1].pos.y = 0;
	vertices[1].pos.z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].dif = colors[1];
	vertices[1].u = 0.0f;
	vertices[1].v = 0.0f;

	// �����̒��_�̏����Z�b�g( 1�|���S���ڂ̑�3���_ )
	vertices[2].pos.x = 0;
	vertices[2].pos.y = wsize.h;
	vertices[2].pos.z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].dif = colors[2];
	vertices[2].u = 0.0f;
	vertices[2].v = 0.0f;

	// �E���̒��_�̏����Z�b�g( 2�|���S���ڂ̑�1���_ )
	vertices[3].pos.x = wsize.w;
	vertices[3].pos.y = wsize.h;
	vertices[3].pos.z = 0.0f;
	vertices[3].rhw = 1.0f;
	vertices[3].dif = colors[3];
	vertices[3].u = 0.0f;
	vertices[3].v = 0.0f;

	// 2�|���S���ڂ̑�2���_�͍����̒��_�Ȃ̂ŃR�s�[
	vertices[4] = vertices[2];

	// 2�|���S���ڂ̑�3���_�͉E��̒��_�Ȃ̂ŃR�s�[
	vertices[5] = vertices[1];
	gradationH_ = MakeScreen(wsize.w, wsize.h,true);
	auto bkScrH = GetDrawScreen();
	//�����_�\�^�[�Q�b�g��ύX���A�O���f�[�V�����̕`��
	SetDrawScreen(gradationH_);
	DrawPolygon2D(vertices.data(), 2, DX_NONE_GRAPH, true);
	//�O���f�[�V�����`������܂����ɖ߂�
	SetDrawScreen(bkScrH);
}

void WipeTransitor::Update()
{
	if (frame_ < interval_) {
		++frame_;
		SetDrawScreen(newRT_);
	}
	else if (frame_ == interval_) {
		SetDrawScreen(DX_SCREEN_BACK);
	}
}

void WipeTransitor::Draw()
{
	if (IsEnd()) {
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, newRT_,true);
	auto rate = (float)frame_ / (float)interval_;
	auto result = DrawBlendGraph(0, 0,oldRT_ , true, gradationH_, 255*rate, 64);
}
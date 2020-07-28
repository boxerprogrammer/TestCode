#include "CapsuleCollider.h"
#include"CircleCollider.h"
#include"Character.h"
#include<DxLib.h>
#include<cassert>
#include<algorithm>

using namespace std;

namespace {
	float Clamp(float value, float minVal = 0.0f, float maxVal = 1.0f) {
		return min(max(value,minVal),maxVal);
	}
}

CapsuleCollider::CapsuleCollider(std::shared_ptr<Character> owner, const Capsule& cup, const char* tag,bool isImmortal):Collider(owner, tag,isImmortal), capsule_(cup) {
}
Capsule& 
CapsuleCollider::GetCapsule() {
	return capsule_;
}

CapsuleCollider::~CapsuleCollider() {
	::OutputDebugStringA("Capsule Deleted");
}

bool 
CapsuleCollider::IsHit(std::shared_ptr<Collider> col) {
	assert(col != nullptr);
	if (capsule_.seg.vec.SQMagnitude() == 0)return false;
	auto ccol = dynamic_pointer_cast<CircleCollider>(col);
	if (ccol != nullptr) {
		Capsule capsule= { AcutualPosition() ,capsule_.seg.vec, capsule_.radius};
		Circle circle = { ccol->AcutualPosition() ,ccol->GetCircle().radius };
		Vector2f vp = circle.center - capsule.seg.start;
		float SQDist = (vp - capsule.seg.vec * Clamp(Dot(vp, capsule.seg.vec) /
						capsule.seg.vec.SQMagnitude(),0,1)
						).SQMagnitude();
		float totalRadius = circle.radius + capsule.radius;
		return SQDist <= totalRadius * totalRadius;
	}
	return false;
}
void 
CapsuleCollider::Draw(){
	if (capsule_.seg.vec.SQMagnitude() == 0)return ;
	uint32_t color = 0xffaaaa;

	Capsule capsule = { AcutualPosition(),capsule_.seg.vec, capsule_.radius };
	auto& spos = capsule.seg.start;
	auto epos = capsule.seg.start+capsule.seg.vec;

	auto offset = GetCameraOffset();
	DrawCircle(spos.x+ offset.x, spos.y, capsule_.radius, color, false,2);
	
	auto v90=capsule.seg.vec;//���̃x�N�g���̃R�s�[
	v90 = { -v90.y, v90.x };//90����]
	v90.Normalize();//���K��
	v90 *= capsule.radius;//���a��������
	auto p1 = spos + v90;//����
	auto p2 = epos + v90;//�E��
	auto p3 = epos - v90;//�E��
	auto p4 = spos - v90;//����

	DrawQuadrangleAA(p1.x + offset.x, p1.y,
		p2.x + offset.x, p2.y,
		p3.x + offset.x, p3.y,
		p4.x + offset.x, p4.y,
		color, false,2);

	DrawCircle(epos.x+ offset.x, epos.y,capsule_.radius, color, false,2);
}

const Vector2f
CapsuleCollider::AcutualPosition()
{
	return capsule_.seg.start + GetOwner()->GetPosition();
}

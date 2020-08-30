#include "CapsuleCollider.h"
#include"CircleCollider.h"
#include"../Character.h"
#include<DxLib.h>
#include<cassert>
#include<algorithm>
#include"../../System/Debugger.h"

using namespace std;


CapsuleCollider::CapsuleCollider(std::shared_ptr<Character> owner, const Capsule& cup, const char* tag,bool isImmortal):Collider(owner, tag,isImmortal), capsule_(cup) {
}
Capsule& 
CapsuleCollider::GetCapsule() {
	return capsule_;
}

CapsuleCollider::~CapsuleCollider() {
}

bool 
CapsuleCollider::IsHit(CollisionInfo& colInfo) {
	auto col = colInfo.collider;
	assert(col != nullptr);
	if (capsule_.seg.vec.SQMagnitude() == 0)return false;
	auto ccol = dynamic_pointer_cast<CircleCollider>(col);
	if (ccol != nullptr) {
		Capsule capsule= { Segment(AcutualPosition() ,capsule_.seg.vec), capsule_.radius};
		Circle circle = { ccol->AcutualPosition() ,ccol->GetCircle().radius };
		Vector2f vp = circle.center - capsule.seg.start;
		float SQDist = (vp - capsule.seg.vec * Clamp(Dot(vp, capsule.seg.vec) /
						capsule.seg.vec.SQMagnitude(),0.0f,1.0f)
						).SQMagnitude();
		float totalRadius = circle.radius + capsule.radius;
		return SQDist <= totalRadius * totalRadius;
	}
	return false;
}
void 
CapsuleCollider::Draw(){
#ifdef _DEBUG
	auto& dbg=Debugger::Instance();
	if (!dbg.IsDebugMode())return;
	if (capsule_.seg.vec.SQMagnitude() == 0)return ;
	uint32_t color = 0xffaaaa;
	if (OwnerIsDead())return;
	Capsule capsule = { Segment(AcutualPosition(),capsule_.seg.vec), capsule_.radius };
	auto& spos = capsule.seg.start;
	auto epos = capsule.seg.start+capsule.seg.vec;

	auto offset = GetCameraOffset();
	DrawCircleAA(spos.x + offset.x, spos.y, capsule_.radius, 16,color, false,2.0f);
	
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

	DrawCircleAA(epos.x+ offset.x, epos.y,capsule_.radius, 16,color, false,2.0f);
#endif
}

const Vector2f
CapsuleCollider::AcutualPosition()
{
	return capsule_.seg.start + GetOwner()->GetPosition();
}

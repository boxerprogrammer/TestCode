#include "FanCollider.h"
#include"CircleCollider.h"
#include"../Character.h"
#include<DxLib.h>
#include<cassert>
#include<algorithm>
#include"../../System/Debugger.h"
#include"../../Arithmetic.h"

using namespace std;

FanCollider::FanCollider(std::shared_ptr<Character> owner, const FanShape& fan, const char* tag, bool isImmortal) :Collider(owner, tag, isImmortal), fan_(fan) {
}

void 
FanCollider::Draw() {
	auto offset = GetCameraOffset();
	auto c = AcutualPosition() + offset;
	auto p1 = c + fan_.v1;
	auto p2 = c + fan_.v2;
	DrawLineAA(c.x, c.y, p1.x, p1.y, 0xff8888, 3.0f);
	DrawLineAA(c.x, c.y, p2.x, p2.y, 0xff8888, 3.0f);
}

const Vector2f 
FanCollider::AcutualPosition() {
	return fan_.center + GetOwner()->GetPosition();
}

FanShape& 
FanCollider::GetFanShape() {
	return fan_;
}

bool 
FanCollider::IsHit(CollisionInfo& colInfo) {
	auto col = colInfo.collider;
	assert(col != nullptr);
	auto ccol = dynamic_pointer_cast<CircleCollider>(col);
	if (fan_.v1 == fan_.v2)return false;
	if (ccol != nullptr) {//これも実質CircleColliderしか受け付けない
		
		FanShape fan = {AcutualPosition() ,fan_.v1,fan_.v2 };
		Circle c = { ccol->AcutualPosition() ,ccol->GetCircle().radius };

		float diff = (c.center - fan.center).Magnitude();
		//①まず、半径外にいれば確定で当たってないので除外するで
		if (diff > fan.Radius() + c.radius)return false;
		//②中心が扇形のv1とv2の間に入っていれば確定当たり。はっきりわかんだね。
		auto v = (c.center - fan.center);
		auto betAngle = fan.GetAngle();
		if (GetAngle2Vector(fan.v1, v) < betAngle) {
			return true;
		}
		//③最後はそれぞれの線分との距離を測る(ここがちょっとめんどい)
		//v1と比較
		if ((v - fan.v1 * Clamp(Dot(v, fan.v1) / fan.v1.SQMagnitude(),0.0f,1.0f)).Magnitude() <= c.radius) {
			return true;
		}
		//v2と比較
		if ((v - fan.v2 * Clamp(Dot(v, fan.v2) / fan.v2.SQMagnitude(),0.0f,1.0f)).Magnitude() <= c.radius) {
			return true;
		}

		return false;
	}
	return false;
}
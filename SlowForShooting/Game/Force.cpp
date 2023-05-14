#include "Force.h"
#include"Player.h"
#include"../DrawFunctions.h"

namespace {
    constexpr int anim_frame_speed = 10;//１枚に必要なフレーム数
    constexpr int anim_frame_num = 3;//アニメーション枚数
    constexpr int img_width = 64;
    constexpr int img_height = 64;
    constexpr float img_scale = 1.0f;
    
    constexpr float force_pull_speed = 1.0f;//フォースが自機に引き寄せられる速度
    constexpr float holdable_distance = 20.0f;//吸着可能な距離

    constexpr int force_size = 30;
    constexpr float force_offset = 39.0f;

    constexpr float separate_speed = 15.0f;
    constexpr float separate_friction = 0.95f;


}

//くっつき状態
void Force::AttachedUpdate()
{
    rect_.center = player_.GetPosition() + 
        Vector2(holdType_==HoldType::front? force_offset :-force_offset, 0.0f);
}

//浮遊状態
void Force::FloatingUpdate()
{
    auto diffY = player_.GetPosition().y - rect_.center.y;
    Vector2 vec = {};
    if (diffY > 0.0f) {
        vec.y = 0.5f;
    }
    else if (diffY < 0.0f) {
        vec.y = -0.5f;
    }

    rect_.center += vec;

    CheckHoldable();
}

void Force::SeparatingUpdate()
{
    rect_.center += {pushSpeed,0.0f};
    pushSpeed *= separate_friction;
    if (fabsf(pushSpeed) <= 0.1f) {
        updateFunc_ = &Force::FloatingUpdate;
    }
}

void Force::CheckHoldable()
{
    
    auto diff = player_.GetPosition() - rect_.center;
    //ある程度近づいたら
    if (diff.Length() < holdable_distance) {
        updateFunc_ = &Force::AttachedUpdate;//吸着！
        if (diff.x > 0.0f) {
            holdType_ = HoldType::back;//フォース位置が自機の後方ならば、後方吸着
        }
        else {
            holdType_ = HoldType::front;//フォース位置が自機の前方ならば、前方吸着
        }
    }
}

//引っ張り状態
void Force::PullingUpdate()
{
    //終点から始点を引く
    auto vec = player_.GetPosition() - rect_.center;
    vec.Normalize();
    vec *= force_pull_speed;
    //この時点のvecが、フォースが自機に近づくベクトルになる

    rect_.center += vec;
    
    CheckHoldable();

}

Force::Force(Player& player):player_(player),
updateFunc_(&Force::FloatingUpdate)
{
    handle_=my::MyLoadGraph(L"./Data/img/Game/force.png");
    rect_.size = { force_size,force_size };
}

const Rect& Force::GetRect() const
{
    return rect_;
}

void Force::SetPosition(const Position2& pos)
{
    rect_.center = pos;
    imgRect_.center = pos;
}

const Position2& Force::GetPosition() const
{
    return rect_.center;
}

int Force::GetLevel() const
{
    return level_;
}

void Force::Update()
{
    if (level_ == 0)return;
    frame_ = (frame_+1) % 60;
    idxX_ = frame_ / 20;

    (this->*updateFunc_)();
    
}

void Force::Draw()
{
    if (level_ == 0)return;
    my::MyDrawRectRotaGraph(rect_.center.x,
        rect_.center.y,
        img_width * idxX_,
        0,
        img_width,
        img_height,
        img_scale,
        0.0f,
        handle_,
        true);
    rect_.Draw(0x8888ff);
}

void Force::LevelUp()
{
    if (level_ == 0) {
        updateFunc_ = &Force::FloatingUpdate;
    }
    level_ = std::min(level_ + 1, 3);
}

void Force::ResetLevel()
{
    level_ = 0;
}

bool Force::IsSepareted() const
{
    return !(updateFunc_==&Force::AttachedUpdate);
}

void Force::Pull()
{
    if (level_ > 0) {
        updateFunc_ = &Force::PullingUpdate;
    }
}

void Force::Push()
{
    if (level_ > 0) {
        updateFunc_ = &Force::SeparatingUpdate;
        pushSpeed = holdType_==HoldType::front? 
                                separate_speed : -separate_speed;
    }
}

void Force::Switch()
{
    //浮遊状態だったら引き寄せ
    if (updateFunc_ == &Force::FloatingUpdate) {
        Pull();
    }
    else if (updateFunc_ = &Force::AttachedUpdate) {
        Push();
    }

}

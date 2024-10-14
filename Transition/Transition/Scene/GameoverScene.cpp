#include "GameoverScene.h"
#include<DxLib.h>
#include"../Input.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"../Application.h"
#include"../Geometry.h"
#include<cassert>

void GameoverScene::LoadingUpdate(Input& input)
{
	if (!(!cutDataFile_) && cutDataFile_->IsReadable()) {
		//まず、要素数をゲットする
		int count = 0;
		cutDataFile_->Read(&count, sizeof(count));
		for (int i = 0; i < count; ++i) {
			//元のファイル名(の長さ)を取得
			byte nameLen = 0;
			cutDataFile_->Read(&nameLen, sizeof(nameLen));

			std::string strName;//元のファイル名
			strName.resize(nameLen);//nameLenでリサイズ
			cutDataFile_->Read(strName.data(), strName.size());
			Rect rc = {};
			cutDataFile_->Read(&rc, sizeof(rc));
			rectTable_[strName] = rc;
		}
		cutDataFile_->Close();
		cutDataFile_.reset();
	}
	if (DxLib::GetASyncLoadNum() == 0) {
		updateFunc_ = &GameoverScene::NormalUpdate;
		drawFunc_ = &GameoverScene::NormalDraw;
	}
}

void GameoverScene::LoadingDraw()
{
}

void GameoverScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("next")) {
		sceneManager_.ChangeScene(std::make_shared<TitleScene>(sceneManager_));
	}
}

void GameoverScene::NormalDraw()
{
	int idx = (frame_ / 5)  % filenames_.size();

	idx = idx % filenames_.size();
	auto it = rectTable_.find(filenames_[idx]);
	if (it != rectTable_.end()) {
		auto& rc = it->second;
		constexpr float scale = 5.0f;
		DrawRectRotaGraph(320+rc.offX* scale, 240+ rc.offY * scale,
			rc.pos.x, rc.pos.y,
			rc.size.w, rc.size.h,
			scale, 0.0f, handle_, true);
	}
	else {
		assert(0);
	}

	frame_=(frame_+1)%65536;
}

GameoverScene::GameoverScene(SceneManager& manager):Scene(manager)
{
	updateFunc_ = &GameoverScene::LoadingUpdate;
	drawFunc_ = &GameoverScene::LoadingDraw;
	auto& app = Application::GetInstance();
	auto mt = app.CreateRandomObject();
	SetUseASyncLoadFlag(true);
	cutDataFile_ = std::make_shared<MimicFile>();
	filenames_ = {
		"crouch-kick1.png",
		"crouch-kick2.png",
		"crouch-kick3.png",
		"crouch-kick4.png",
		"crouch-kick5.png",
		"crouch1.png",
		"crouch2.png",
		"fall1.png",
		"fall2.png",
		"flying-kick1.png",
		"flying-kick2.png",
		"hurt1.png",
		"hurt2.png",
		"idle1.png",
		"idle2.png",
		"idle3.png",
		"idle4.png",
		"jump1.png",
		"jump2.png",
		"kick1.png",
		"kick2.png",
		"kick3.png",
		"kick4.png",
		"kick5.png",
		"punch1.png",
		"punch2.png",
		"punch3.png",
		"punch4.png",
		"punch5.png",
		"punch6.png",
		"walk1.png",
		"walk2.png",
		"walk3.png",
		"walk4.png",
		"walk5.png",
		"walk6.png"
	};
	handle_ = LoadGraph(L"img/monk.png");
	cutDataFile_->Open(L"img/monk_rects.dat");
}
void GameoverScene::Update(Input& input)
{
	(this->*updateFunc_)(input);
}

void GameoverScene::Draw()
{
	ClearDrawScreen();
	(this->*drawFunc_)();
	DrawString(50, 50, L"Gameover Scene", 0xffffff);
}

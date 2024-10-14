#include "ContinueScene.h"
#include<DxLib.h>
#include"../Input.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"../Application.h"
#include"../Geometry.h"
#include<cassert>

void ContinueScene::LoadingUpdate(Input& input)
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
		updateFunc_ = &ContinueScene::NormalUpdate;
		drawFunc_ = &ContinueScene::NormalDraw;
	}
}

void ContinueScene::LoadingDraw()
{
}

void ContinueScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("next")) {
		sceneManager_.ChangeScene(std::make_shared<TitleScene>(sceneManager_));
	}
}

void ContinueScene::NormalDraw()
{
	int idx = (frame_ / 5)  % rectTable_.size();

	//idx = idx % filenames_.size();
	//auto it = rectTable_.find(filenames_[idx]);

	int i = 0;
	for (const auto& rcp : rectTable_) {
		const auto& rc = rcp.second;
		if (idx == i){
			constexpr float scale = 5.0f;
			DrawRectRotaGraph(320 + rc.offX * scale, 240 + rc.offY * scale,
				rc.pos.x, rc.pos.y,
				rc.size.w, rc.size.h,
				scale, 0.0f, handle_, true);
		}
		++i;
	}
	

	frame_=(frame_+1)%65536;
}

ContinueScene::ContinueScene(SceneManager& manager):Scene(manager)
{
	updateFunc_ = &ContinueScene::LoadingUpdate;
	drawFunc_ = &ContinueScene::LoadingDraw;
	auto& app = Application::GetInstance();
	auto mt = app.CreateRandomObject();
	SetUseASyncLoadFlag(true);
	cutDataFile_ = std::make_shared<MimicFile>();
	
	handle_ = LoadGraph(L"img/hassan.png");
	cutDataFile_->Open(L"img/hassan_rects.dat");
}
void ContinueScene::Update(Input& input)
{
	(this->*updateFunc_)(input);
}

void ContinueScene::Draw()
{
	(this->*drawFunc_)();
	DrawString(50, 50, L"Gameover Scene", 0xffffff);
}

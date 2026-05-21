#include<DxLib.h>
#include "GameoverScene.h"
#include"GameScene.h"
#include"SceneController.h"
#include"../Input.h"
#include"../ResourceManager.h"
#include"../Application.h"
#include"PauseScene.h"
#include"../StringUtility.h"
#include<cassert>

constexpr int fade_interval = 30;



GameScene::GameScene(SceneController& ctrl):Scene(ctrl),
update_(&GameScene::FadeInUpdate),
draw_(&GameScene::FadeDraw),
frame_(fade_interval),
deadBullet_(true)
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	pos_ = { wsize.w * 0.5f,wsize.h * 0.5f };
	vel_ = {};
	gameH_ = ResourceManager::LoadGraph(L"Resource/img/gaming.png");

	//カメラの設定
	SetupCamera_Perspective(DX_PI_F / 3.0f);
	SetCameraPositionAndAngle(VGet(0.0f, 1.0f, -10.0f),
		0.0f, 0.0f, 0.0f);
	SetCameraNearFar(0.3f, 1000.0f);
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);

	//ライトの設定
	auto lightVec = GetLightDirection();
	lightVec.x *= 0.5;
	lightVec = VNorm(lightVec);
	SetLightDirection(lightVec);

	

	//配置情報の読み取り
	auto handle = FileRead_open(L"location.dat");
	int count = 0;
	FileRead_read(&count, sizeof(count), handle);
	gameObjects_.resize(count);
	for (auto& obj : gameObjects_) {
		byte len = 0;
		FileRead_read(&len, sizeof(len), handle);
		obj.name.resize(len);
		FileRead_read(obj.name.data(), len, handle);
		FileRead_read(&(obj.pos), sizeof(obj.pos), handle);
		FileRead_read(&(obj.axis), sizeof(obj.axis), handle);
		FileRead_read(&(obj.angle), sizeof(obj.angle), handle);
		FileRead_read(&(obj.scale), sizeof(obj.scale), handle);

	}
	FileRead_close(handle);


	models_.clear();
	CreateEnvironmentModelList(L"./Resource/model");
	for (auto& model : models_) {
		LoadModel(model.first);
	}

}

void GameScene::Update(Input& input)
{
	(this->*update_)(input);
}

void GameScene::Draw()
{
	(this->*draw_)();
}


//フェードイン
void GameScene::FadeInUpdate(Input& input) {
	if (--frame_ <= 0) {
		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
	}
}
//フェードアウト
void GameScene::FadeOutUpdate(Input& input) {
	if (++frame_ >= fade_interval) {
		controller_.ChangeScene(std::make_shared<GameoverScene>(controller_));
		return;
	}
}
//通常更新
void GameScene::NormalUpdate(Input& input) {
	if (input.IsTriggered("ok")) {
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
		return;
	}
	if (input.IsTriggered("pause")) {
		controller_.PushScene(std::make_shared<PauseScene>(controller_));
		return;
	}
	constexpr float speed = 0.5f;
	Vector2 vel = {};
	if (input.IsPressed("up")) {
		vel.y = -1.0f;
	}
	if (input.IsPressed("down")) {
		vel.y = 1.0f;
	}
	if (input.IsPressed("left")) {
		vel.x = -1.0f;
	}
	if (input.IsPressed("right")) {
		vel.x = 1.0f;
	}
	constexpr float jump_power = -10.0f;
	constexpr float gravity = 0.25f;
	if (input.IsTriggered("jump")) {
		isJumping_ = true;
		vel_ += {0.0f, jump_power};
	}
	if (isJumping_) {
		vel_ += {0.0f, gravity};
	}
	vel_ += vel.Normalied() * speed;
	pos_ += vel_;
	auto bottom = Application::GetInstance().GetWindowSize().h;
	if (bottom <= pos_.y + 20.0f) {
		isJumping_ = false;
		pos_.y = bottom - 20.0f;
		vel_.y = 0.0f;
	}
}

//フェード描画
void GameScene::FadeDraw() {
	NormalDraw();

	//黒いセロファンをアルファブレンディングしている
	auto size = Application::GetInstance().GetWindowSize();
	//0～60→0.0～1.0に変換している
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	//0.0～1.0*255.0→0.0～255.0に変換される
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * rate));
	//黒セロファン
	DrawBox(0, 0, size.w, size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
//通常描画
void GameScene::NormalDraw() {
	DrawBox(0, 0, 1280, 720, 0x0000ff, true);
	for (const auto& obj : gameObjects_) {
		if (obj.name == "Sphere") {
			DrawSphere3D(obj.pos.GetVECTOR(),
				0.5f,
				32,
				0xffffff,
				0xffffff,
				true);
		}else if(obj.name=="Cube"){
			Cube cube(Vector3(), 0.5f);
			MATRIX m = MGetIdent();
			m = MMult(m, MGetScale(obj.scale.GetVECTOR()));//拡縮
			m = MMult(m,MGetRotAxis(obj.axis.GetVECTOR(), DegreeToRadian( obj.angle)));//回転
			m = MMult(m, MGetTranslate(obj.pos.GetVECTOR()));//平行移動
			cube.Draw(m);
		}
		else {
			int handle = models_[obj.name];
			MV1SetScale(handle, obj.scale.GetVECTOR());
			MV1SetRotationMatrix(handle, 
				MGetRotAxis(obj.axis.GetVECTOR(), DegreeToRadian(obj.angle)));
			MV1SetPosition(handle,obj.pos.GetVECTOR());
			MV1DrawModel(handle);
		}
	}

	int x = 100;
	int y = 100;
	for (const auto& obj : gameObjects_) {
		auto wname = StringUtility::GetWideStringFromString(obj.name);
		DrawFormatString(x, y, 0xffffff,
			L"name=%s,pos=%.3f,%.3f,%.3f , axis=%.3f,%.3f,%.3f, angle=%.3f",
			wname.c_str(), obj.pos.x, obj.pos.y, obj.pos.z,
			obj.axis.x,obj.axis.y,obj.axis.z,
			obj.angle);
		y += 30;
	}
}

void GameScene::CreateEnvironmentModelList(const std::wstring& folder)
{
	FILEINFO info;
	auto findHandle = FileRead_findFirst((folder + L"/*.mv1").c_str(), &info);
	if (findHandle == (ULONGLONG)(-1)) {
		return;
	}
	do {
		auto str = StringUtility::GetStringFromWideString(info.Name);
		auto n = str.rfind(".mv1");
		str = str.substr(0, n);
		models_[str] = -1;
	} while (FileRead_findNext(findHandle, &info)==0);
}

void GameScene::LoadModel(const std::string& name)
{
	std::wstring path = L"./Resource/Model/";
	path += StringUtility::GetWideStringFromString(name);
	path += L".mv1";
	int handle = MV1LoadModel(path.c_str());
	assert(handle >= 0);
	models_[name] = handle;
}


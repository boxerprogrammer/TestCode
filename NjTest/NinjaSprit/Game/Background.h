#pragma once
#include<vector>
#include<memory>
class Camera;
class Background
{
private:
	std::vector<int> bgH_;//背景レイヤーハンドル
	int frame_=0;//現在のフレーム番号
	std::shared_ptr<Camera> camera_;
public:
	/// <summary>
	/// 背景クラスコンストラクタ
	/// </summary>
	/// <param name="camera">カメラへの参照</param>
	Background(std::shared_ptr<Camera> camera);
	~Background();

	/// <summary>
	/// カメラ座標による更新
	/// </summary>
	void Update();

	/// <summary>
	/// カメラ座標を考慮した描画
	/// </summary>
	void Draw();
};


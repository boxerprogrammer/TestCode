#pragma once
#include"../Geometry.h"
#include<memory>
class Player;
/// <summary>
/// カメラクラス
/// </summary>
class Camera
{
private:
	std::shared_ptr<Player> player_;
	Position2f pos_;//カメラの中心座標
	Size stageSize_;///<デフォルトは800x600にしとく
public:
	/// <summary>
	/// カメラコンストラクタ
	/// </summary>
	/// <param name="p">プレイヤーへの参照</param>
	Camera();
	~Camera();

	/// <summary>
	/// 監視用プレイヤーをセット
	/// </summary>
	/// <param name="player">プレイヤーオブジェクト</param>
	void SetPlayer(std::shared_ptr<Player> player);

	/// <summary>
	/// ステージの限界サイズをセットする
	/// </summary>
	/// <param name="size">ステージ全体のサイズ</param>
	void SetStageSize(const Size& size);

	/// <summary>
	/// カメラ座標の更新(プレイヤーとステージ限界で決まる)
	/// </summary>
	void Update();

	/// <summary>
	/// 現在のカメラ座標を返す
	/// </summary>
	/// <returns>カメラ座標</returns>
	const Position2f& GetPosition()const;

	/// <summary>
	/// カメラの撮影範囲を返す
	/// </summary>
	/// <returns>撮影範囲矩形</returns>
	const Rect GetViewRange()const;

	/// <summary>
	/// 描画をどれくらいずらせばいいかオフセットベクトルを返す
	/// </summary>
	/// <returns>オフセット</returns>
	const Vector2f ViewOffset()const;

};


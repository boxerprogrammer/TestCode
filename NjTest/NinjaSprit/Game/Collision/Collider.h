#pragma once

#include<memory>
#include<string>
#include"../../Geometry.h"

constexpr char tag_player_attack[] = "patk";
constexpr char tag_enemy_damage[] = "edmg";

class Character;
class Camera;
/// <summary>
/// 当たり判定能力を持つ基底クラス
/// </summary>
class Collider
{
private:
	std::string tag_;///<タグ(使うかな…？)
	std::weak_ptr<Character> owner_;///<持ち主の生き死にには関与しないので弱参照
protected:
	bool isDeletable_ = false;//死んでもいいフラグ
	bool isImmortal_ = false;//不滅フラグ
	bool isActive_ = true;
	Vector2f GetCameraOffset();
public:
	Collider(std::shared_ptr<Character> owner,const char* tag="",bool immortal=false);
	virtual ~Collider() = default;

	/// <summary>
	/// 削除しても大丈夫？
	/// </summary>
	/// <returns></returns>
	bool IsDeletable()const;
	/// <summary>
	/// 自殺
	/// </summary>
	void Suicide();
	
	/// <summary>
	/// 一時的に機能しなくする
	/// </summary>
	void Sleep();
	/// <summary>
	/// 機能するようにする
	/// </summary>
	void Awake();
	/// <summary>
	/// 当たり判定は機能しているか？
	/// </summary>
	/// <returns>true:機能する false:機能しない</returns>
	bool IsActive()const;

	
	const std::string& GetTag()const;

	/// <summary>
	/// 持ち主が死にました
	/// </summary>
	/// <returns>true:死、false:生きてる</returns>
	bool OwnerIsDead()const;

	/// <summary>
	/// 持ち主のスマートポインタを返す
	/// </summary>
	/// <returns>持ち主のスマートポインタ</returns>
	std::shared_ptr<Character> GetOwner();

	/// <summary>
	/// ヒットしたかどうかを返す
	/// </summary>
	/// <param name="col">相手側コライダー</param>
	/// <returns>true:衝突、false:非衝突</returns>
	virtual bool IsHit(std::shared_ptr<Collider> col)=0;

	/// <summary>
	/// デバッグ用。もしデバッグ円やデバッグ矩形を表示したい場合は
	///表示する
	/// </summary>
	virtual void Draw() {};

};

/// <summary>
/// 当たったときに渡す情報
/// 今のところは相手コライダーオブジェクトのみだが
/// 拡張性の為にこうしている。
/// </summary>
struct CollisionInfo {
	std::shared_ptr<Collider> collider;
};


#pragma once
#include <DxLib.h>

class AnimationController;
class Player
{
public:
	// シングルトン（生成・取得・削除）
	static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new Player(); } };
	static Player* GetInstance(void) { return instance_; };
	static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }

private:
	// 静的インスタンス
	static Player* instance_;
private:
	// デフォルトコンストラクタをprivateにして、外部から生成できない様にする
	Player(void);
	// デストラクタも同様
	~Player(void);

	// コピー・ムーブ操作を禁止
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	Player(Player&&) = delete;
	Player& operator=(Player&&) = delete;

	// 下記をコンパイルエラーさせるため 上記を追加
	// Item copy = *Item::GetInstance();
	// Item copied(*Item::GetInstance());
	// Item moved = std::move(*Item::GetInstance());
public:

	// アニメーションの再生速度
	static constexpr float SPEED_ANIM = 20.0f;

	// 移動スピード
	static constexpr float SPEED_MOVE = 10.0f;

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		MAX,
	};

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// アニメーション制御
	AnimationController* animationController_;

	// アニメーション種別
	ANIM_TYPE animType_;

	// モデル情報
	int modelId_;

	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;
	VECTOR moveDir_;
	VECTOR movePow;
	// ローカル角度
	VECTOR localAngles_;
};

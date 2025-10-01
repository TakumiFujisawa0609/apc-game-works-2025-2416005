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
	static constexpr float SPEED_MOVE = 3.0f;

	// 重力
	static constexpr float GRAVITY_POW = 0.8f;

	// ジャンプ力
	static constexpr float JUMP_POW = 20.0f;

	// ノックバックスピード
	static constexpr float SPEED_KNOCKBACK = 10.0f;

	// ダッシュスピード
	static constexpr float DASH_SPEED = 6.0f;
	
	// ダッシュ体力最大値
	static constexpr int DASH_TP_MAX = 100;  

	// 1フレームで消費する体力
	static constexpr int DASH_TP_USE = 1;   

	// 1フレームで回復する体力
	static constexpr int DASH_TP_RECOVER = 1; 


	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		JUMP,
		CAPO,
		BAKA,
		MAX,
	};

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void);

	VECTOR GetAngles(void);

	float GetRadius() const;

private:

	enum class STATE
	{
		NONE,
		STANDBY,
		KNOCKBACK,
		ATTACK,
		DEAD,
		END,
		VICTORY,
	};

	// アニメーション制御
	AnimationController* animationController_;

	// アニメーション種別
	ANIM_TYPE animType_;

	// モデル情報
	int modelId_;

	// 現在の状態  
	STATE currentState_;

	int prevAnimType_ = static_cast<int>(ANIM_TYPE::IDLE);

	void ChangeState(STATE newState);

	// 状態を変更する
	void ChangeKnockback(void);
	void ChangeStandby(void);
	void ChangeAttack(void);
	void ChangeDead(void);
	void ChangeEnd(void);
	void ChangeVictory(void);

	void UpdateKnockback(void);

	void ProcessUp(void);
	void ProcessDown(void);
	void ProcessMove(void);
	void ProcessShot(void);
	void ProcessAtack(void);
	void ProcessBrink(void);


	// ノックバック方向
	VECTOR knockBackDir_;

	// ノックバックカウンタ(点滅用)
	int cntKnockBack_;

	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;
	VECTOR moveDir_;
	VECTOR movePow;
	// ローカル角度
	VECTOR localAngles_;

	// ジャンプ力
	float jumpPow_;

	// ジャンプ判定
	bool isJump_;

	// ダッシュ体力
	int dashTp;

	VECTOR jumpForward_ = VGet(0.0f, 0.0f, 0.0f);  // 前方向のジャンプ移動量
};

#pragma once
#include <DxLib.h>

class Weapon;
class AnimationController;
class Player
{
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
	static constexpr int DASH_TP_MAX = 500;  

	// 1フレームで消費する体力
	static constexpr int DASH_TP_USE = 0.1;   

	// 1フレームで回復する体力
	static constexpr int DASH_TP_RECOVER = 10; 

	// 当たり判定用半径
	static constexpr float ATTACK_TIME_LIMIT = 1.5f;

	// 体力
	static constexpr int HP = 1000;

	// 攻撃速度調整変数（短すぎる場合は0.5～0.8推奨）
	float attackAnimSpeed_ = 30.0f;



	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		JUMP,
		CAPO,
		BAKA,
		ATTACKS,
		ATTACK,
		ATTACK1,
		ATTACK2,
		ATTACK3,
		ATTACK4,
		ATTACK5,
		ATTACK6,
		ATTACK1_BRANCH,
		ATTACK2_BRANCH,
		ATTACK3_BRANCH,
		ATTACK4_BRANCH,
		ATTACK5_BRANCH,
		ATTACK6_BRANCH,
		MAX,
		ATTACK_IDLE,
		PANCH,
		PANCH_2,
		KICK,
		RUN,
	};


	// 状態関数型
	typedef void (*AttackStepFunction)(Player&);

	Player(void);
	~Player(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void);
	VECTOR* GetPosPtr(void);

	VECTOR GetAngles(void);

	int GetHP(void);

	float GetRadius() const;

	int GetModelId(void) const { return modelId_; }

	void TakeDamage(int damage);

	bool IsAtack() const { return isAtack_; }

	void PlayEffectAt(const VECTOR & pos);


private:

	enum class STATE
	{
		NONE,
		STANDBY,
		KNOCKBACK,
		ATTACK,
		SHOT,
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


	bool IsBranching() const { return isBranching_; }
	void SetBranching(bool flag) { isBranching_ = flag; }

	// 現在の状態に応じた更新処理
	void UpdateKnockback(void);
	void UpdateStandby(void);
	void UpdateAttack(void);
	void UpdateShot(void);
	void UpdateDead(void);
	void UpdateEnd(void);
	void UpdateVictory(void);

	// 状態を変更する
	void ChangeKnockback(void);
	void ChangeStandby(void);
	void ChangeAttack(void);
	void ChangeShot(void);
	void ChangeDead(void);
	void ChangeEnd(void);
	void ChangeVictory(void);

	// 描画処理
	void DrawStandby(void);
	void DrawKnockback(void);
	void DrawAttack(void);
	void DrawShot(void);
	void DrawDead(void);
	void DrawEnd(void);
	void DrawVictory(void);


	void ProcessUp(void);
	void ProcessDown(void);
	void ProcessMove(void);
	void ProcessShot(void);
	void ProcessAtack(void);
	void ProcessBrink(void);

	void ApplyRootMotion();


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

	// アタック判定	
	bool isAtack_;

	// アタック継続時間
	int cntAtack_;

	// ダッシュ体力
	int dashTp;

	// サウンドハンドル
	int SHandle ;

	VECTOR jumpForward_ = VGet(0.0f, 0.0f, 0.0f);  // 前方向のジャンプ移動量

	bool isDash_ = false;  // ダッシュ中か

	Weapon* weapon_;

	bool isBrinkAction_ = false;  // ブースト中 or ダッシュ中フラグ



	float attackTimeLimit_;


	int attackStep_ = 0;         // 現在の攻撃段数（0:非攻撃, 1～6:攻撃段数）
	int attackInputBuffer_ = 0;  // 入力バッファ（次段攻撃受付）
	int attackInputTimer_ = 0;   // 入力受付時間（フレーム数）
	bool isBranchAttack_ = false; // 派生攻撃
	int branchType_;        // どの段から派生したか（1?6）
	bool isAttackStartedThisFrame = true;

	VECTOR prevRootPos_ = VGet(0, 0, 0);

	VECTOR cameraTarget = pos_;

	bool isInBranchCommand_;  // 派生コマンド入力中ならtrue
	bool isBranching_ = false;  // 派生入力中かどうか

	int hp_ = 1000;
	int maxHp_ = 1000;
	bool isDead_ = false;
	bool isHit_ = false;
	bool isInvincible_ = false;
	int invincibleTimer_ = 0;

	int efectslashId_;

	int attackFrameCount_;

};

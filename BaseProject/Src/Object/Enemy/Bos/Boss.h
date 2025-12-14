#pragma once
#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../Utility/AnimationController.h"


class Player;
class Boss : public EnemyBase
{
public:

    // アニメーション種別
    enum class ANIM_TYPE
    {
        IDLE,
        WALK,
        ATTACK,
        DAMAGE,
        DEAD,
        WIN,
    };

    // コンストラクタ
    Boss(Player* player);

    // デストラクタ
    ~Boss();

    // 初期化
    void Init(float _x, float _y, float _z) override;

    // 更新
    void Update() override;

    // 描画
    void Draw() override;

    // 解放
    void Release() override;

    // 撃破
    void Kill() override;

    // ダメージを受ける
    void TakeDamage(int damage);

    // 状態列挙型
    enum class STATE {
        NONE,
        IDLE,
        ATTACK,
        DAMAGE,
        DEAD,
    };

    // アニメーション制御
    AnimationController* animationController_;

    // 現在のアニメーション種別
    ANIM_TYPE animType_;

    VECTOR GetPos() const { return Pos_; }
    float GetRadius() const { return radius_; }
    void OnHit(const VECTOR&);
    int GetHP() const { return hp_; }
    bool CanBeHit() const;

private:

    void ChangeState(STATE newState);

    void UpdateIdle();
    void UpdateAttack();
    void UpdateDamage();
    void UpdateDead();

    STATE currentState_;

    Player* player_;

    float radius_ = 100.0f;
    int modelId_ = -1;
    int hp_ = 100;  // 体力

    // 攻撃関連（フレーム単位で管理、Slime と同様の方式）
    bool isAttacking_ = false;     // 攻撃中かどうか
    int attackTimer_ = 0;          // 攻撃の経過フレーム
    int attackCooldown_ = 0;       // 攻撃クールタイム（フレーム）
    const int attackDamage_ = 30;  // ダメージ量
    const int attackRange_ = 80;   // 攻撃可能距離（ワールド単位）
    const int attackDuration_ = 900; // 攻撃動作フレーム数
    const int attackHitFrame_ = 15; // 実際にダメージが発生するフレーム
    const int attackCooldownMax_ = 300; // 次の攻撃までの待機（フレーム）

    // 被ダメージ関連
    int damageStunTimer_ = 0;     // 被ダメージ後の行動不能タイマー（フレーム）
    const int damageStunMax_ = 60; // 無力化時間（60フレーム = 約1秒）

    int moveSpeed_ = 1;

    VECTOR Pos_;
    VECTOR Angels_;
};
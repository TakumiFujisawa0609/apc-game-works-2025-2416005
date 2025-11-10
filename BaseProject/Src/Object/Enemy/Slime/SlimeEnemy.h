#pragma once
#include "../EnemyBase.h"
#include <DxLib.h>
#include <vector>
#include "../../Player/ShotPlayer.h"

class Player;
class SlimeEnemy : public EnemyBase {
public:

    bool isCounted = false;

    SlimeEnemy(Player* player);
    ~SlimeEnemy();

    void Init(float _x, float _y, float _z) override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void Kill() override;         // オーバーライド
    void OnHit(const VECTOR& hitPos); // 弾から呼ばれる

    void TakeDamage(int damage);

    VECTOR pos = VGet(x, y, z);

    VECTOR GetPos() const { return VGet(x, y, z); }
    float GetRadius() const { return 40.0f; }
	float GetRadius2() const { return 40.0f; }

    bool IsDeadEffect() const { return isDeadEffect_; }
    int GetHP() const { return hp_; }

    bool CanBeHit() const;

private:
    float moveSpeed;
    int color;

    float enemymoove = 1.0f; // 追従速度

    // モデル情報
    int modelId_;

    // ノックバック速度
    VECTOR knockbackVel_;   

    // 当たった弾の位置
    VECTOR lastHitPos_;        

    // 点滅中
    bool isDeadEffect_ = false;        

    // 点滅残りフレーム
    int deadEffectTimer_ = 0;         

    // ノックバック中か（点滅前）
    bool isKnockbackOnly_ = false;    

	int hp_ = 10;  // 体力

	Player* player_ = nullptr;

    // 攻撃関連
    bool isAttacking_ = false;     // 攻撃中かどうか
    int attackTimer_ = 0;          // 攻撃の経過フレーム
    int attackCooldown_ = 0;       // 攻撃クールタイム
    const int attackDamage_ = 5;  // ダメージ量
    const int attackRange_ = 80;   // 攻撃可能距離
    const int attackDuration_ = 30; // 攻撃動作フレーム数
    const int attackHitFrame_ = 15; // 実際にダメージが発生するフレーム
    const int attackCooldownMax_ = 90; // 次の攻撃までの待機
    int damageStunTimer_;     // 被ダメージ後の行動不能タイマー
    const int damageStunMax_ = 60; // 無力化時間（60フレーム = 約1秒）
};

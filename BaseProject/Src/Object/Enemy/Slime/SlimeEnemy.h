#pragma once
#include "../EnemyBase.h"
#include <DxLib.h>
#include <vector>
#include "../../Player/ShotPlayer.h"

class SlimeEnemy : public EnemyBase {
public:

    bool isCounted = false;

    SlimeEnemy();
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
    float GetRadius() const { return 20.0f; }
	float GetRadius2() const { return 20.0f; }

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

	int hp_ = 1;  // 体力
};

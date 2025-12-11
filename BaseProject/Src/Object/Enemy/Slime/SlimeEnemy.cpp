#include <DxLib.h>
#include <vector>
#include "../../../Application.h"
#include "../../Player/Player.h"
#include "../Slime/SlimeEnemy.h"
#include "../EnemyManager.h"
#include "../EnemyBase.h"

std::vector<SlimeEnemy*> slimes;
std::vector<ShotPlayer*> shots;

SlimeEnemy::SlimeEnemy(Player* player)
    : moveSpeed(0.1f), color(GetColor(0, 255, 0)),
    isDeadEffect_(false), deadEffectTimer_(0), knockbackVel_(VGet(0, 0, 0)), lastHitPos_(VGet(0, 0, 0))
{
	player_ = player;
}


SlimeEnemy::~SlimeEnemy()
{
    Release();
}

void SlimeEnemy::Init(float _x, float _y, float _z) 
{
    EnemyBase::Init(_x, _y, _z);
    moveSpeed = 0.5f;
    color = GetColor(0, 106, 182);
	isAlive = true;
    damageStunTimer_ = 0;

	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Enemy/slime.mv1").c_str());
}

void SlimeEnemy::Update()
{
    if (!isAlive) return;

    if (damageStunTimer_ > 0)
    {
        damageStunTimer_--;

        // ノックバック中なら少しだけ動かす
        if (isKnockbackOnly_)
        {
            pos = VAdd(pos, knockbackVel_);
            knockbackVel_ = VScale(knockbackVel_, 0.9f);
            if (VSize(knockbackVel_) < 0.1f) {
                isKnockbackOnly_ = false;
                knockbackVel_ = VGet(0, 0, 0);
            }
        }
        return;
    }


    // 攻撃クールタイム減少
    if (attackCooldown_ > 0) attackCooldown_--;

    VECTOR playerPos = player_->GetPos();
    VECTOR toPlayer = VSub(playerPos, GetPos());
    toPlayer.y = 0.0f;
    float distance = VSize(toPlayer);

    // ノックバック処理
    if (isKnockbackOnly_)
    {
        pos = VAdd(pos, knockbackVel_);
        knockbackVel_ = VScale(knockbackVel_, 0.9f);
        if (VSize(knockbackVel_) < 0.1f) {
            isKnockbackOnly_ = false;
            knockbackVel_ = VGet(0, 0, 0);
        }
        return;
    }

    // 死亡エフェクト処理
    if (isDeadEffect_)
    {
        deadEffectTimer_--;
        if (deadEffectTimer_ <= 0)
        {
            isAlive = false;
        }
        return;
    }

    // 追従範囲
    const float followRange = 1000.0f;
    const float stopRange = 100.0f;
    static bool isChasing = false;

    if (distance <= followRange)
        isChasing = true;
    else if (distance > followRange + stopRange)
        isChasing = false;

    // 攻撃処理
    if (!isAttacking_ && attackCooldown_ <= 0)
    {
        // 一定距離内なら攻撃開始
        if (distance < attackRange_)
        {
            isAttacking_ = true;
            attackTimer_ = 0;
        }
    }

    // 攻撃中の処理
    if (isAttacking_)
    {
        attackTimer_++;

        // 攻撃中は移動しない
        if (attackTimer_ == attackHitFrame_)
        {
            // このタイミングでヒット判定を出す
            float minDist = GetRadius() + player_->GetRadius();
            if (VSize(VSub(GetPos(), player_->GetPos())) < minDist + 20.0f)
            {
                player_->TakeDamage(attackDamage_);
            }
        }

        // 攻撃モーション終了
        if (attackTimer_ > attackDuration_)
        {
            isAttacking_ = false;
            attackCooldown_ = attackCooldownMax_;
        }

        // モーション再生など
        return;
    }


    // スライムの位置を取得
    VECTOR slimePos = pos;

    // プレイヤーへの方向ベクトル
    VECTOR direction = VSub(playerPos, slimePos);
    direction = VNorm(direction);

    // スライムの向きをプレイヤー方向に180度反転して設定
    float angleY = atan2f(direction.x, direction.z) + DX_PI_F;
    MV1SetRotationXYZ(modelId_, VGet(0.0f, angleY, 0.0f));


    // モデルの向きを設定
    MV1SetRotationXYZ(modelId_, VGet(0.0f, angleY, 0.0f));

    // 追従
    if (isChasing)
    {
        VECTOR dir = VNorm(toPlayer);
        x += dir.x * moveSpeed;
        z += dir.z * moveSpeed;
    }

    // プレイヤーとの押し戻し 
    {
        VECTOR diff = VSub(GetPos(), playerPos);
        diff.y = 0.0f; float dist = VSize(diff);
        float minDist = GetRadius() + player_->GetRadius();
        if (dist < minDist && dist > 0.0001f) {
            VECTOR pushDir = VNorm(diff); float pushAmount = (minDist - dist);
            x += pushDir.x * pushAmount; z += pushDir.z * pushAmount;
        }
    }

    // 敵同士の押し戻し
    EnemyManager* sm = EnemyManager::GetInstance();
    if (sm)
    {
        const auto& slimes = sm->GetSlimes();
        for (auto other : slimes)
        {
            if (other == this) continue;
            if (!other->GetAlive()) continue;

            VECTOR diff = VSub(GetPos(), other->GetPos());
            diff.y = 0.0f;
            float dist = VSize(diff);
            float minDist = GetRadius() + other->GetRadius();

            if (dist < minDist && dist > 0.0001f)
            {
                VECTOR pushDir = VNorm(diff);
                float pushAmount = (minDist - dist) * 0.5f;
                x += pushDir.x * pushAmount;
                z += pushDir.z * pushAmount;
                other->x -= pushDir.x * pushAmount;
                other->z -= pushDir.z * pushAmount;
            }
        }
    }
}

void SlimeEnemy::Draw()
{
    if (!isAlive) return;

    if (isDeadEffect_)
    {
        if ((deadEffectTimer_ / 5) % 2 == 0)
        {
            DrawSphere3D(VGet(x, y, z), GetRadius2(), 4, GetColor(255, 0, 0), GetColor(255, 128, 128), true);
        }
        return;
    }

	if (modelId_ != -1)
	{
		MV1SetPosition(modelId_, VGet(x, y, z));
		float scale = 0.3f;
        VECTOR rot = VGet(0.0f, DX_PI_F, 0.0f); // DX_PI_F = 180度
        MV1SetRotationXYZ(modelId_, rot);
		MV1SetScale(modelId_, VGet(scale, scale, scale));
		MV1DrawModel(modelId_);
	}
}

void SlimeEnemy::Release() 
{
    if (modelId_ != -1) {
        MV1DeleteModel(modelId_);
        modelId_ = -1;
    }
}

void SlimeEnemy::Kill()
{
    if (isDeadEffect_) return; // 二重処理防止

    isDeadEffect_ = true;
    deadEffectTimer_ = 60; // 1秒点滅
    VECTOR dir = VNorm(VSub(pos, lastHitPos_));
    knockbackVel_ = VScale(dir, 2.0f); // ノックバック方向と強さ

}

void SlimeEnemy::OnHit(const VECTOR& hitPos)
{
    if (!isAlive || isKnockbackOnly_) return;

    lastHitPos_ = hitPos;

    VECTOR diff = VSub(GetPos(), hitPos);
    diff.y = 0.0f; // Y軸は固定する場合
    diff = VNorm(diff);

    knockbackVel_ = VScale(diff, 8.0f); // 適切な速度に調整

    isKnockbackOnly_ = true;
}

void SlimeEnemy::TakeDamage(int damage)
{
    if (!isAlive || isDeadEffect_) return;
    hp_ -= damage;

    damageStunTimer_ = damageStunMax_;

    // HPが0以下になったら死亡
    if (hp_ <= 0)
    {
        hp_ = 0;
        Kill();
    }
}

bool SlimeEnemy::CanBeHit() const
{
    return isAlive && !isDeadEffect_ && !isKnockbackOnly_;
}

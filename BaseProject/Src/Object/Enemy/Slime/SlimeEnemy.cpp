#include <DxLib.h>
#include <vector>
#include "../../../Application.h"
#include "../../Player/Player.h"
#include "../Slime/SlimeEnemy.h"
#include "../Slime/SlimeManager.h"
#include "../EnemyManager.h"
#include "../EnemyBase.h"

std::vector<SlimeEnemy*> slimes;
std::vector<ShotPlayer*> shots;

SlimeEnemy::SlimeEnemy()
    : moveSpeed(0.1f), color(GetColor(0, 255, 0)),
    isDeadEffect_(false), deadEffectTimer_(0), knockbackVel_(VGet(0, 0, 0)), lastHitPos_(VGet(0, 0, 0))
{
}


SlimeEnemy::~SlimeEnemy()
{
    Release();
}

void SlimeEnemy::Init(float _x, float _y, float _z) 
{
    EnemyBase::Init(_x, _y, _z);
    moveSpeed = 5.0f;
    color = GetColor(0, 106, 182);
	isAlive = true;
}

void SlimeEnemy::Update()
{
    if (!isAlive) return;

    VECTOR playerPos = Player::GetInstance()->GetPos();

    // ノックバック処理
    if (isKnockbackOnly_)
    {
        pos = VAdd(pos, knockbackVel_);
        knockbackVel_ = VScale(knockbackVel_, 0.92f); // 減衰率を少し緩める

        if (VSize(knockbackVel_) < 0.3f) // 閾値を下げる
        {
            isKnockbackOnly_ = false;
            knockbackVel_ = VGet(0, 0, 0); // リセット
        }
        return; // ノックバック中は他の処理をスキップ
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

    {
        VECTOR diff = VSub(GetPos(), playerPos);
        diff.y = 0.0f;
        float dist = VSize(diff);
        float minDist = GetRadius() + Player::GetInstance()->GetRadius();

        if (dist < minDist && dist > 0.0001f)
        {
            VECTOR pushDir = VNorm(diff);
            float pushAmount = (minDist - dist);
            x += pushDir.x * pushAmount;
            z += pushDir.z * pushAmount;
        }
    }

    // 点滅中はその他の動作を止める
    if (isDeadEffect_)
    {
        pos = VAdd(pos, knockbackVel_);
        knockbackVel_ = VScale(knockbackVel_, 0.9f);

        deadEffectTimer_--;
        if (deadEffectTimer_ <= 0)
        {
            isDeadEffect_ = false;
            isAlive = false;
        }

        return;
    }

    // 通常の移動処理（上下運動・プレイヤー追従・スライム同士の衝突）
    y += moveSpeed;
    if (y > 50.0f || y < 0.0f) moveSpeed *= -1.0f;

    VECTOR dir = VSub(playerPos, GetPos());
    dir = VNorm(dir);
    x += dir.x * enemymoove;
    y += dir.y * enemymoove;
    z += dir.z * enemymoove;

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
                y += pushDir.y * pushAmount;
                z += pushDir.z * pushAmount;
                other->x -= pushDir.x * pushAmount;
                other->y -= pushDir.y * pushAmount;
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

	// 当たり判定の可視化
	DrawSphere3D(VGet(x, y, z), GetRadius(), 4, color, color, false);
}

void SlimeEnemy::Release() 
{
}

void SlimeEnemy::Kill()
{
    if (isDeadEffect_) return; // 二重処理防止

    isDeadEffect_ = true;
    deadEffectTimer_ = 60; // 1秒点滅
    VECTOR dir = VNorm(VSub(pos, lastHitPos_));
    knockbackVel_ = VScale(dir, 1000.0f); // ノックバック方向と強さ

}

void SlimeEnemy::OnHit(const VECTOR& hitPos)
{
    if (!isAlive) return;
    if (isKnockbackOnly_) return; // 既にノックバック中なら無視

    VECTOR diff = VSub(GetPos(), hitPos);
    diff.y = 0.0f; // Y軸は固定する場合
    diff = VNorm(diff);

    knockbackVel_ = VScale(diff, 8.0f); // 適切な速度に調整

    isKnockbackOnly_ = true;
    lastHitPos_ = hitPos; // 記録しておく
}

void SlimeEnemy::TakeDamage(int damage)
{
    if (!isAlive) return;
    if (isDeadEffect_) return;
    //if (isKnockbackOnly_) return; // ノックバック中は無敵

    hp_ -= damage;

    // HPが0以下になったら死亡
    if (hp_ <= 0)
    {
        hp_ = 0;
        Kill();
    }
}
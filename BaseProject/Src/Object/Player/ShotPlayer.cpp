#include <DxLib.h>
#include "ShotPlayer.h"
#include "../../Object/Enemy/EnemyManager.h"
#include "../../Object/Enemy/EnemyBase.h"
#include "../../Object/Enemy/Slime/SlimeEnemy.h"
#include "../../Utility/AsoUtility.h"
#include "../../Object/Enemy/Bos/Boss.h"

ShotPlayer::ShotPlayer(const VECTOR& pos, const VECTOR& dir)
    : pos_(pos), dir_(VNorm(dir)), speed_(20.0f), alive_(true), startPos_(pos), prevPos_(pos)
{
    dir_ = VNorm(dir_);
}

ShotPlayer::~ShotPlayer()
{

}

void ShotPlayer::Init(void)
{
}

void ShotPlayer::Update(void)
{
    if (!alive_) return;

    prevPos_ = pos_;
    pos_ = VAdd(pos_, VScale(dir_, speed_));

    const float maxRange = 2000.0f;
    if (VSize(VSub(pos_, startPos_)) > maxRange) {
        alive_ = false;
        return;
    }

    EnemyManager* sm = EnemyManager::GetInstance();
    if (!sm) {
        alive_ = false;  
        return;
    }

    const auto& slimes = sm->GetSlimes();
    for (auto slime : slimes) {
        if (!slime) continue;
        if (!slime->CanBeHit()) continue; 
        if (!slime->GetAlive()) continue;

        VECTOR psPos = pos_;            // 弾の現在座標
        VECTOR sPos = slime->GetPos(); // スライム座標

        // 距離ベクトル
        VECTOR diff;
        diff.x = psPos.x - sPos.x;
        diff.y = psPos.y - sPos.y;
        diff.z = psPos.z - sPos.z;

        // 距離の二乗
        float dis =
            diff.x * diff.x +
            diff.y * diff.y +
            diff.z * diff.z;

        // 半径合計（弾 + スライム）
        float radius = SHOT_RADIUS + slime->GetRadius();

        // 半径^2より小さい → 衝突
        if (dis < (radius * radius)) {

            // スライムへダメージ
            slime->OnHit(pos_);
            slime->Kill();

            // 弾を消す
            alive_ = false;
            break;
        }
    }

    Boss* boss = sm->GetBoss();
    if (boss && boss->GetAlive())
    {
        VECTOR psPos = pos_;             // 弾の現在位置
        VECTOR bPos = boss->GetPos();   // ボスの座標

        // 距離ベクトル
        VECTOR diff;
        diff.x = psPos.x - bPos.x;
        diff.y = psPos.y - bPos.y;
        diff.z = psPos.z - bPos.z;

        float dis =
            diff.x * diff.x +
            diff.y * diff.y +
            diff.z * diff.z;

        float radius = SHOT_RADIUS + boss->GetRadius();

        if (dis < (radius * radius))
        {
            boss->Kill();

            // 弾を消す
            alive_ = false;
        }
    }
}

void ShotPlayer::Draw(void)
{
    if (!alive_) return;

    DrawSphere3D(pos_, SHOT_RADIUS, 16, GetColor(255, 0, 0), GetColor(128, 0, 0), TRUE);
}

void ShotPlayer::Release(void)
{

}


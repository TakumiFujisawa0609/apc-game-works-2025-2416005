#include <DxLib.h>
#include "ShotPlayer.h"
#include "../../Object/Enemy/EnemyManager.h"
#include "../../Object/Enemy/EnemyBase.h"
#include "../../Object/Enemy/Slime/SlimeEnemy.h"
#include "../../Utility/AsoUtility.h"

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
    if (!sm) return;

    const auto& slimes = sm->GetSlimes();
    for (auto slime : slimes) {
        if (!slime) continue;
        if (!slime->GetAlive()) continue;

        VECTOR c = slime->GetPos();
        float r = slime->GetRadius() + SHOT_RADIUS;

        VECTOR d = VSub(pos_, prevPos_);
        float dd = VSize(d);
        if (dd == 0.0f) {
            if (VSize(VSub(prevPos_, c)) <= r) {
                slime->Kill();
                break;
            }
            continue;
        }
        float t = AsoUtility::Dot(VSub(c, prevPos_), d) / (dd * dd);
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        VECTOR closest = VAdd(prevPos_, VScale(d, t));
        float dist = VSize(VSub(closest, c));
        if (dist <= r) {
            slime->OnHit(pos_); 
            slime->Kill();
            break;
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


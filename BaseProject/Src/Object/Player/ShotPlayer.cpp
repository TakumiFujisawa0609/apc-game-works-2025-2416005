#include "ShotPlayer.h"

ShotPlayer::ShotPlayer(const VECTOR& pos, const VECTOR& dir)
    : pos_(pos), dir_(VNorm(dir)), speed_(20.0f), alive_(true), startPos_(pos)
{
    dir_ = VNorm(dir_); // 正規化
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

    // 移動
    pos_ = VAdd(pos_, VScale(dir_, speed_));

    // 発射位置からの距離を計算
    float dist = VSize(VSub(pos_, startPos_));

    // 1000.0f 以上離れたら消す（値はお好みで）
    if (dist > 1000.0f)
    {
        alive_ = false;
    }
}

void ShotPlayer::Draw(void)
{
    if (!alive_) return;

    // 半径 1.0 の球を描画（赤色・塗りつぶし）
    DrawSphere3D(pos_, 100.0f, 16, GetColor(255, 0, 0), GetColor(128, 0, 0), TRUE);

}

void ShotPlayer::Release(void)
{

}

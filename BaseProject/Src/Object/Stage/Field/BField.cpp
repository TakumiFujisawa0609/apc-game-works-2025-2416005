#include "BField.h"
#include "../../Player/Player.h"

BField::BField()
{
}

BField::~BField()
{
}

void BField::Init(float _x, float _y, float _z)
{
    pos_ = VGet(_x, _y, _z);
	radius_ = 100.0f;  // 仮の半径
}

void BField::Update(void)
{
    VECTOR playerPos = player->GetPos();

    // プレイヤーが範囲内に入ったら制圧開始
    float dist = VSize(VSub(playerPos, pos_));
    if (dist < radius_)
    {
        StartCapture(FieldState::PLAYER);
        UpdateCapture(1.0f);  // 仮：1フレームごとに1%進行
    }
}

void BField::Draw(void)
{

}

void BField::Release(void)
{

}

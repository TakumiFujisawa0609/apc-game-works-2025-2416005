#include "BField.h"
#include "../../Player/Player.h"

BField::BField()
{
}

BField::~BField()
{
}

void BField::Init(float _x, float _y, float _z, Player* player)
{
    // 親クラス初期化
    FieldBase::Init(_x, _y, _z,player);

    // 固有設定
    state_ = FieldState::ENEMY;     // 最初は敵拠点
    radius_ = 200.0f;               // フィールド範囲
    durability_ = 150;              // 耐久値

}

void BField::Update(void)
{
    FieldBase::Update();
}

void BField::Draw(void)
{
    FieldBase::Draw();
}

void BField::Release(void)
{
    FieldBase::Release();
}

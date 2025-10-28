#include "AField.h"

AField::AField()
{
}

AField::~AField()
{
}

void AField::Init(float _x, float _y, float _z, Player* player)
{
    // 親クラス初期化
    FieldBase::Init(_x, _y, _z,player);

    // 固有設定
    state_ = FieldState::ENEMY;     // 最初は敵拠点
    radius_ = 200.0f;               // フィールド範囲
    durability_ = 150; 
}

void AField::Update(void)
{
    FieldBase::Update();
}

void AField::Draw(void)
{
	FieldBase::Draw();
}

void AField::Release(void)
{
	FieldBase::Release();
}

#include "GField.h"

GField::GField()
{
}

GField::~GField()
{
}

void GField::Init(float _x, float _y, float _z, Player* player)
{
	// 親クラス初期化
	FieldBase::Init(_x, _y, _z, player);
	// 固有設定
	state_ = FieldState::ENEMY;     // 最初は敵拠点
	radius_ = 200.0f;               // フィールド範囲
	durability_ = 150;              // 耐久値
}

void GField::Update(void)
{
	FieldBase::Update();
}

void GField::Draw(void)
{
	FieldBase::Draw();
}

void GField::Release(void)
{
	FieldBase::Release();
}

#include "FieldBase.h"


FieldBase::FieldBase(void)
{
	scales_ = SCALES;
	durability_ = 100;
	radius_ = 150.0f;
	state_ = FieldState::ENEMY;
	isCapturing_ = false;
	captureProgress_ = 0.0f;
}

FieldBase::~FieldBase(void)
{

}

void FieldBase::Init(float _x, float _y, float _z)
{

}

void FieldBase::Update(void)
{

}

void FieldBase::Draw(void)
{
	// フィールド位置を円で描画（仮の視覚化）
	unsigned int color = GetColor(255, 0, 0); // 敵拠点: 赤
	if (state_ == FieldState::PLAYER) color = GetColor(0, 0, 255); // 青
	if (state_ == FieldState::NEUTRAL) color = GetColor(255, 255, 255); // 白

	DrawCircle((int)pos_.x, (int)pos_.z, (int)radius_, color, FALSE);
}

void FieldBase::Release(void)
{

}

void FieldBase::Damage(int dmg)
{
	durability_ -= dmg;
	if (durability_ > 0) 
	{

	}
	else
	{
		durability_ = 0;
	}

}

void FieldBase::StartCapture(FieldState capturer)
{
	if (state_ == capturer) return; // すでに味方のもの
	isCapturing_ = true;
}

void FieldBase::UpdateCapture(float delta)
{
	if (!isCapturing_) return;

	captureProgress_ += delta;

	if (captureProgress_ >= 100.0f)
	{
		state_ = (state_ == FieldState::ENEMY) ? FieldState::PLAYER : FieldState::ENEMY;
		captureProgress_ = 0.0f;
		isCapturing_ = false;
		durability_ = 100;
	}
}

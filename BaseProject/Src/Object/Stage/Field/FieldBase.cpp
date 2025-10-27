#include "FieldBase.h"
#include "../../Player/Player.h"

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
	pos_ = VGet(_x, _y, _z);
	modelHandle_ = MV1LoadModel("Data/Model/FIeld/Line.mv1"); 
	MV1SetPosition(modelHandle_, pos_);
}

void FieldBase::Update(void)
{
	if (player_ == nullptr) return;

	VECTOR playerPos = player_->GetPos();

	float dist = VSize(VSub(playerPos, pos_));
	bool isInside = (dist <= radius_);
	
	if (isInside)
	{
		// フィールド内に入っている
		if (!wasInside_) {
			wasInside_ = true;
			OnEnterField();
		}
	}
	else
	{
		// 外に出た
		if (wasInside_) {
			wasInside_ = false;
			OnExitField();
		}
	}
}

void FieldBase::Draw(void)
{
	MV1SetPosition(modelHandle_, pos_);
	float scalef = 3.0f;
	VECTOR scale = VGet(scalef, 1.0f, scalef);
	MV1SetScale(modelHandle_, scale);
	MV1DrawModel(modelHandle_);
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
	if (isCapturing_) return; // すでに制圧中なら無視
	isCapturing_ = true;
	captureProgress_ = 0.0f;
	capturer_ = capturer;
}

void FieldBase::UpdateCapture(float delta)
{
	if (!isCapturing_) return;

	// 制圧進行（deltaは時間差、または固定値）
	captureProgress_ += delta;

	if (captureProgress_ >= 100.0f) {
		// 完了
		state_ = capturer_;
		isCapturing_ = false;
		captureProgress_ = 0.0f;
		durability_ = 100; // 制圧後に耐久リセット
	}
}

void FieldBase::OnEnterField()
{
	// 制圧開始などの処理
	StartCapture(FieldState::PLAYER);
}

void FieldBase::OnExitField()
{
	// 制圧中断など
	isCapturing_ = false;
}

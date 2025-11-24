#include "GField.h"
#include "../../../Application.h"
#include <DxLib.h>

GField::GField()
{
	fieldName_ = "Gフィールド";
}

GField::~GField()
{
	if (boss_) {
		delete boss_;
		boss_ = nullptr;
	}
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

	// ボスが出てきたら更新
	if (boss_ && boss_->GetAlive())
	{
		boss_->Update();
	}
}

void GField::Draw(void)
{
	FieldBase::Draw();

	// ボス描画
	if (boss_ && boss_->GetAlive())
	{
		boss_->Draw();
	}
}

void GField::Release(void)
{
	if (boss_) {
		delete boss_;
		boss_ = nullptr;
	}

	FieldBase::Release();
}

void GField::CheckCaptureCondition()
{
	if (!enemyManager_) return;

	int killCount = enemyManager_->GetKilledCount();

	// ---- ボス未出現 → 100体倒したらボス出現 ----
	if (!bossSpawned_ && killCount >= 100)
	{
		VECTOR spawnPos = VAdd(pos_, VGet(0.0f, 0.0f, 200.0f));

		boss_ = new Boss(player_);
		boss_->Init(spawnPos.x, spawnPos.y, spawnPos.z);

		bossSpawned_ = true;

		// もうスライムを出さない
		enemyManager_->Release();
		enemyManager_.reset();

		printfDx(" ボス出現！！ Gフィールド決戦開始 \n");
	}

	// ---- ボスが倒されたら制圧完了 ----
	if (bossSpawned_ && boss_ && !boss_->GetAlive())
	{
		isCaptured_ = true;
		state_ = FieldState::PLAYER;
		durability_ = 100;

		printfDx(" Gフィールド制圧達成！\n");
	}
}
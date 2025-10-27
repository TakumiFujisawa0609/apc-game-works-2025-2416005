#include "FieldBase.h"
#include "../../Player/Player.h"

FieldBase::FieldBase(void)
{
	scales_ = SCALES;
	durability_ = 100;
	state_ = FieldState::ENEMY;
	isCapturing_ = false;
	captureProgress_ = 0.0f;
	radius_ = 150.0f;

	isCaptured_ = false;
	isPlayerInside_ = false;
	wasInside_ = false;
	respawnTimer_ = 0;
	player_ = nullptr;
}

FieldBase::~FieldBase(void)
{

}

void FieldBase::Init(float _x, float _y, float _z)
{
	pos_ = VGet(_x, _y, _z);
}

void FieldBase::Update(void)
{
	if (!player_) {
		DrawFormatString(50, 120, GetColor(255, 0, 0), "player_ is nullptr!");
		return;
	}

	VECTOR playerPos = player_->GetPos();

	// ==== フィールド範囲 ====
	VECTOR scale = VGet(3.0f, 1.0f, 3.0f);
	float halfWidth = (1350.0f * scale.x) / 2.0f;
	float halfDepth = (1350.0f * scale.z) / 2.0f;
	float halfHeight = 500.0f;

	bool insideX = fabs(playerPos.x - pos_.x) <= halfWidth;
	bool insideY = fabs(playerPos.y - pos_.y) <= halfHeight;
	bool insideZ = fabs(playerPos.z - pos_.z) <= halfDepth;
	bool isInside = insideX && insideY && insideZ;

	// フィールド侵入・退出チェック
	if (isInside && !wasInside_) { wasInside_ = true; OnEnterField(); }
	else if (!isInside && wasInside_) { wasInside_ = false; OnExitField(); }

	// 敵管理の更新
	if (isPlayerInside_ && enemyManager_) {
		enemyManager_->Update();
		UpdateEnemySpawn();
		CheckCaptureCondition();  
	}

	DrawFormatString(50, 100, GetColor(255, 255, 255),
		"Player Y: %.1f  FieldY: %.1f  Diff: %.1f",
		playerPos.y, pos_.y, playerPos.y - pos_.y);
}

void FieldBase::Draw(void)
{
	VECTOR scale = VGet(3.0f, 1.0f, 3.0f);
	VECTOR half = VGet((1350.0f * scale.x) / 2.0f, (20.0f * scale.y) / 2.0f, (1350.0f * scale.z) / 2.0f);
	VECTOR minPos = VSub(pos_, half);
	VECTOR maxPos = VAdd(pos_, half);
	DrawCube3D(minPos, maxPos, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);

	if (enemyManager_) enemyManager_->Draw();

	// 状態表示
	DrawFormatString(50, 50, isPlayerInside_ ? GetColor(0, 255, 0) : GetColor(255, 0, 0),
		"Player in field: %s", isPlayerInside_ ? "Yes" : "No");

	// 制圧ゲージ表示
	if (isPlayerInside_ && !isCaptured_ && enemyManager_) {
		int kill = enemyManager_->GetKilledCount();
		DrawFormatString(
			(int)pos_.x / 3 + 100, (int)pos_.z / 3 + 80, GetColor(255, 255, 255),
			"討伐数: %d / %d", kill, targetKillCount_);
	}

	// 制圧済み表示
	if (isCaptured_) {
		DrawFormatString(
			(int)pos_.x / 3 + 100, (int)pos_.z / 3 + 100, GetColor(0, 255, 0),
			"制圧完了");
	}
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

void FieldBase::OnExitField()
{
	if (!isPlayerInside_) return;
	isPlayerInside_ = false;

	// 敵を全削除
	if (enemyManager_) {
		enemyManager_->Release();
		enemyManager_.reset();
	}
}

void FieldBase::OnEnterField()
{
	if (isPlayerInside_) return;
	isPlayerInside_ = true;

	// 敵マネージャ生成
	enemyManager_ = std::make_unique<EnemyManager>(player_);
	enemyManager_->SetInstance(enemyManager_.get());

	// 初期スポーン
	SpawnEnemies();
}

void FieldBase::StartBattle()
{
	if (isActive_) return;
	isActive_ = true;
	killCount_ = 0;

	enemyManager_ = std::make_unique<EnemyManager>(player_);
	enemyManager_->SetInstance(enemyManager_.get());

	// 初回に敵を10体出す
	for (int i = 0; i < 10; i++) {
		float rx = pos_.x + (rand() % 200 - 100);
		float rz = pos_.z + (rand() % 200 - 100);
		enemyManager_->Init(rx, pos_.y, rz);
	}
}

void FieldBase::UpdateBattle()
{
}

void FieldBase::SpawnEnemies()
{
	if (!enemyManager_) return;

	VECTOR playerPos = player_->GetPos();

	int current = enemyManager_->GetEnemyCount();
	int spawnCount = maxEnemies_ - current;
	if (spawnCount <= 0) return;

	for (int i = 0; i < spawnCount; i++) {
		float angle = (float)(rand() % 360) * DX_PI_F / 180.0f;
		float dist = (float)(rand() % (int)spawnRadius_);
		float sx = playerPos.x + cosf(angle) * dist;
		float sz = playerPos.z + sinf(angle) * dist;
		enemyManager_->Init(sx, playerPos.y, sz);
	}
}

void FieldBase::UpdateEnemySpawn()
{
	if (!enemyManager_) return;

	int count = enemyManager_->GetEnemyCount();
	if (count < 100) {
		respawnTimer_++;
		if (respawnTimer_ >= 180) { // 3秒後
			respawnTimer_ = 0;
			SpawnEnemies(); // FieldBase 側で呼ぶ
		}
	}
	else {
		respawnTimer_ = 0;
	}
}

void FieldBase::CheckCaptureCondition()
{
	if (!enemyManager_) return;
	if (isCaptured_) return; // すでに制圧済みなら無視

	int killCount = enemyManager_->GetKilledCount();

	// --- 制圧条件達成 ---
	if (killCount >= targetKillCount_) {
		isCaptured_ = true;
		state_ = FieldState::PLAYER;

		// 敵の出現を止める
		enemyManager_->Release();
		enemyManager_.reset();

		// 制圧演出
		printfDx("フィールド制圧完了！\n");

		// （任意）耐久リセットなど
		durability_ = 100;
	}
}

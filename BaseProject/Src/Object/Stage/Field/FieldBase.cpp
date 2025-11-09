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

void FieldBase::Init(float _x, float _y, float _z, Player* player)
{
	pos_ = VGet(_x, _y, _z);

	player_ = player;
}

void FieldBase::Update(void)
{
	VECTOR playerPos = player_->GetPos();

	// フィールド範囲設定
	VECTOR scale = VGet(3.0f, 1.0f, 3.0f);
	float halfWidth = (1350.0f * scale.x) / 2.0f;
	float halfDepth = (1350.0f * scale.z) / 2.0f;
	float halfHeight = 5000000.0f;

	bool insideX = fabs(playerPos.x - pos_.x) <= halfWidth;
	bool insideY = fabs(playerPos.y - pos_.y) <= halfHeight;
	bool insideZ = fabs(playerPos.z - pos_.z) <= halfDepth;
	bool isInside = insideX && insideY && insideZ;

	// フィールド侵入・退出チェック
	if (isInside && !isPlayerInside_) {
		isPlayerInside_ = true;
		OnEnterField();
	}
	else if (!isInside && isPlayerInside_) {
		isPlayerInside_ = false;
		OnExitField();
	}

	// 敵管理更新
	if (isPlayerInside_ && enemyManager_) {
		enemyManager_->Update();
		UpdatePendingEnemies();
		UpdateFallingEnemies();
		UpdateEnemySpawn();
		CheckCaptureCondition();
	}

	//// デバッグ表示
	//DrawFormatString(50, 80, GetColor(255, 255, 255),
	//	"Inside: %s", isPlayerInside_ ? "YES" : "NO");
	//DrawFormatString(50, 100, GetColor(255, 255, 255),
	//	"PlayerPos(%.1f, %.1f, %.1f)", playerPos.x, playerPos.y, playerPos.z);
	//DrawFormatString(50, 120, GetColor(255, 255, 255),
	//	"FieldPos(%.1f, %.1f, %.1f)", pos_.x, pos_.y, pos_.z);

	if (isPlayerInside_) {
		DrawSphere3D(playerPos, 50.0f, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), true); // 赤い球で表示
	}
}

void FieldBase::Draw(void)
{
	VECTOR scale = VGet(3.0f, 1.0f, 3.0f);
	VECTOR half = VGet((1350.0f * scale.x) / 2.0f, (20.0f * scale.y) / 2.0f, (1350.0f * scale.z) / 2.0f);
	VECTOR minPos = VSub(pos_, half);
	VECTOR maxPos = VAdd(pos_, half);
	DrawCube3D(minPos, maxPos, GetColor(0, 255, 0), GetColor(0, 255, 0), FALSE);

	if (enemyManager_) enemyManager_->Draw();

	//// 状態表示
	//DrawFormatString(50, 50, isPlayerInside_ ? GetColor(0, 255, 0) : GetColor(255, 0, 0),
	//	"Player in field: %s", isPlayerInside_ ? "Yes" : "No");

	//// 制圧ゲージ表示
	//if (isPlayerInside_ && !isCaptured_ && enemyManager_) {
	//	int kill = enemyManager_->GetKilledCount();
	//	DrawFormatString(
	//		(int)pos_.x / 3 + 100, (int)pos_.z / 3 + 80, GetColor(255, 255, 255),
	//		"討伐数: %d / %d", kill, targetKillCount_);
	//}

	//// 制圧済み表示
	//if (isCaptured_) {
	//	DrawFormatString(
	//		(int)pos_.x / 3 + 100, (int)pos_.z / 3 + 100, GetColor(0, 255, 0),
	//		"制圧完了");
	//}
}

void FieldBase::Release(void)
{
	if (EnemyManager::GetInstance())
	{
		EnemyManager::GetInstance()->Release();
		EnemyManager::SetInstance(nullptr);
	}
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
	//printfDx("プレイヤーがフィールドに侵入しました。\n");

	if (isCaptured_) return;

	// 敵マネージャ生成
	if (!enemyManager_) {
		enemyManager_ = std::make_unique<EnemyManager>(player_);
		enemyManager_->SetInstance(enemyManager_.get());
		SpawnEnemies();
	}
}

void FieldBase::OnExitField()
{
	//printfDx("プレイヤーがフィールドから退出しました。\n");

	if (enemyManager_) {
		enemyManager_->Release();
		EnemyManager::SetInstance(nullptr); // グローバルを無効化
		enemyManager_.reset();              // unique_ptr で安全に破棄
	}
}

void FieldBase::UpdateFallingEnemies()
{
	for (auto it = fallingEnemies_.begin(); it != fallingEnemies_.end(); )
	{
		it->pos = VAdd(it->pos, it->vel);
		it->timer++;

		// 地面に着いたか判定（y座標が地面以下）
		if (it->pos.y <= pos_.y) {
			it->landed = true;

			// 敵として正式スポーン
			enemyManager_->Init(it->pos.x, pos_.y, it->pos.z);

			it = fallingEnemies_.erase(it);
			continue;
		}

		++it;
	}
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
	if (!player_ || !enemyManager_) return;

	int current = enemyManager_->GetEnemyCount() + (int)fallingEnemies_.size() + (int)pendingEnemies_.size();
	int spawnCount = maxEnemies_ - current;
	if (spawnCount <= 0) return;

	VECTOR scale = VGet(3.0f, 1.0f, 3.0f);
	float halfWidth = (1350.0f * scale.x) / 2.0f;
	float halfDepth = (1350.0f * scale.z) / 2.0f;

	for (int i = 0; i < spawnCount; i++) {
		PendingEnemy pe;
		float offsetX = (float)(rand() % (int)(halfWidth * 2)) - halfWidth;
		float offsetZ = (float)(rand() % (int)(halfDepth * 2)) - halfDepth;

		pe.pos = VAdd(pos_, VGet(offsetX, 0.0f, offsetZ));

		// 各敵が出てくるまでの遅延時間（例：1～3秒）
		pe.delay = 60 + rand() % 1200; // 60～180フレーム後に出現
		pendingEnemies_.push_back(pe);
	}
}


void FieldBase::UpdateEnemySpawn()
{
	if (!enemyManager_) return;

	respawnTimer_++;

	if (respawnTimer_ >= 120) { // 2秒おきに出す
		respawnTimer_ = 0;
		SpawnEnemies(); // ← 空から降らせる
	}
}

void FieldBase::CheckCaptureCondition()
{
	if (!enemyManager_) return;
	if (isCaptured_) return; // すでに制圧済みなら無視

	int killCount = enemyManager_->GetKilledCount();

	// 制圧条件達成 
	if (killCount >= targetKillCount_) {
		isCaptured_ = true;
		state_ = FieldState::PLAYER;

		// 敵の出現を止める
		enemyManager_->Release();
		enemyManager_.reset();

		// 制圧演出
		//printfDx("フィールド制圧完了！\n");

		// 耐久リセットなど
		durability_ = 100;
	}
}

void FieldBase::UpdatePendingEnemies()
{
	for (auto it = pendingEnemies_.begin(); it != pendingEnemies_.end(); )
	{
		it->delay--;

		// 時間がきたら落下開始
		if (it->delay <= 0)
		{
			FallingEnemy fe;
			fe.pos = VAdd(it->pos, VGet(0.0f, 400.0f + (rand() % 100), 0.0f));
			fe.vel = VGet(0.0f, -6.0f - (rand() % 3), 0.0f);
			fe.landed = false;
			fe.timer = 0;

			fallingEnemies_.push_back(fe);

			it = pendingEnemies_.erase(it);
		}
		else {
			++it;
		}
	}
}

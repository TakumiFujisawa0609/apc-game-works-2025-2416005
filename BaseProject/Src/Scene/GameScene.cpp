#include <cmath>
#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Stage/Stage.h"
#include "TitleScene.h"
#include "../Object/Player/Player.h"
#include "GameScene.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Weapon/Weapon.h"
#include "../Object/Stage/Field.h"

GameScene::GameScene(void) : SceneBase()
{
	grid_ = nullptr;
	SetMouseDispFlag(false);
}

GameScene::~GameScene(void)
{

}

void GameScene::Init(void)
{
	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(player_);
	camera->ChangeMode(Camera::MODE::FOLLOW);

	// ステージ初期化
	Stage::CreateInstance();
	Stage::GetInstance()->Init();

	// フィールド初期化
	Field::CreateInstance();
	Field::GetInstance()->Init();

	// プレイヤー初期化
	Player::CreateInstance();
	Player::GetInstance()->Init();

	enemy_ = new EnemyManager();
	EnemyManager::SetInstance(enemy_);
	enemy_->Init(0.0f, 0.0f, 50.0f);
}

void GameScene::Update(void)
{
	// ステージ更新
	Stage::GetInstance()->Update();

	// フィールド更新
	Field::GetInstance()->Update();

	// プレイヤー更新
	Player::GetInstance()->Update();

	enemy_->Update();
}

void GameScene::Draw(void)
{
	// ステージ描画
	Stage::GetInstance()->Draw();

	// フィールド描画
	Field::GetInstance()->Draw();

	enemy_->Draw();

	// プレイヤー描画
	Player::GetInstance()->Draw();
}

void GameScene::Release(void)
{
	// ステージ解放
	Stage::GetInstance()->Release();
	Stage::DeleteInstance();

	// フィールド解放
	Field::GetInstance()->Release();
	Field::DeleteInstance();

	// プレイヤー解放
	Player::GetInstance()->Release();
	Player::DeleteInstance();

	enemy_->Release();
	EnemyManager::SetInstance(nullptr);
}

#include <cmath>
#include <DxLib.h>
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Stage/Stage.h"
#include "TitleScene.h"
#include "../Object/Player/Player.h"
#include "GameScene.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Weapon/Weapon.h"
#include "../Object/Stage/Field/Field.h"
#include "../Scene/SceneManager.h"

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

}

void GameScene::Update(void)
{
	// ステージ更新
	Stage::GetInstance()->Update();

	// フィールド更新
	Field::GetInstance()->Update();

	// プレイヤー更新
	player_->Update();

	enemy_->Update();
}

void GameScene::Load(void)
{
	// プレイヤー初期化
	player_ = new Player();
	player_->Init();

	// カメラモード変更
	Camera* camera = SceneManager::GetInstance()->GetCamera();
	camera->SetFollow(player_);
	camera->ChangeMode(Camera::MODE::FOLLOW);

	// ステージ初期化
	Stage::CreateInstance();
	Stage::GetInstance()->Init();
	Stage::GetInstance()->SetPlayer(player_);

	// フィールド初期化
	Field::CreateInstance();
	Field::GetInstance()->Init();


	enemy_ = new EnemyManager(player_);
	EnemyManager::SetInstance(enemy_);
	enemy_->Init(0.0f, 0.0f, 50.0f);
}

void GameScene::LoadEnd(void)
{

}

void GameScene::Draw(void)
{

	Camera* camera = SceneManager::GetInstance()->GetCamera();
	camera->SetBeforeDraw();

	// ステージ描画
	Stage::GetInstance()->Draw();

	// フィールド描画
	Field::GetInstance()->Draw();

	enemy_->Draw();

	// プレイヤー描画
	player_->Draw();

	camera->DrawDebug();
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
	player_->Release();

	enemy_->Release();
	EnemyManager::SetInstance(nullptr);
}

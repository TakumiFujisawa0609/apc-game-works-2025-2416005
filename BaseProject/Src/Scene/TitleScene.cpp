#include <cmath>
#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Stage.h"
#include "TitleScene.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Slime/SlimeManager.h"

TitleScene::TitleScene(void) : SceneBase()
{
	grid_ = nullptr;
	SetMouseDispFlag(false);
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	// カメラモード変更
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(player_);
	camera->ChangeMode(Camera::MODE::FOLLOW);

	// グリッド初期化
	grid_ = new Grid();
	grid_->Init();

	//// ステージ初期化
	Stage::CreateInstance();
	Stage::GetInstance()->Init();

	// プレイヤー初期化
	Player::CreateInstance();
	Player::GetInstance()->Init();

	slime_ = new SlimeManager();
	SlimeManager::SetInstance(slime_);
	slime_->Spawn(0.0f, 0.0f, 50.0f);
}

void TitleScene::Update(void)
{
	// グリッド更新
	grid_->Update();

	// ステージ更新
	Stage::GetInstance()->Update();

	// プレイヤー更新
	Player::GetInstance()->Update();

	slime_->Update();
}

void TitleScene::Draw(void)
{
	// グリッド描画
	grid_->Draw();

	// ステージ描画
	Stage::GetInstance()->Draw();

	// プレイヤー描画
	Player::GetInstance()->Draw();

	slime_->Draw();
}

void TitleScene::Release(void)
{
	// グリッド解放
	grid_->Release();
	delete grid_;

	// ステージ解放
	Stage::GetInstance()->Release();
	Stage::DeleteInstance();

	// プレイヤー解放
	Player::GetInstance()->Release();
	Player::DeleteInstance();

	slime_->Release();
	SlimeManager::SetInstance(nullptr);
}

#include <cmath>
#include <DxLib.h>
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Stage/Stage.h"
#include "TitleScene.h"
#include "../Object/Player/Player.h"
#include "../Manager/Input/KeyManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/GameScene.h"
#include "../Application.h"

TitleScene::TitleScene(void) : SceneBase()
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	imageId_ = LoadGraph("Data/Image/suekane.png");
	if (imageId_ == -1) {
		MessageBoxA(NULL, "末金画像の読み込みに失敗しました。パスやファイルを確認してください。", "エラー", MB_OK);
	}
}

void TitleScene::Update(void)
{
	if (CheckHitKey(KEY_INPUT_SPACE) || (KEY::GetIns().GetInfo(KEY_TYPE::GAME_NEXT).down))
	{
		// スペースキーが押下されたら、ゲームシーンへ遷移する
		SceneManager::GetInstance()->ChangeScene(std::make_shared<GameScene>());
	}
}

void TitleScene::Load(void)
{
}

void TitleScene::LoadEnd(void)
{
}

void TitleScene::Draw(void)
{
	DrawGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y /*/ 2*/, imageId_, true);

	DrawString(100, 100, "TitleScene", GetColor(255, 0, 0));
	DrawString(100, 200, "Press A Button", GetColor(255, 0, 0));
	DrawString(100, 300, "Press SPACE", GetColor(255, 0, 0));
}

void TitleScene::Release(void)
{
}

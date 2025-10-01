#include <DxLib.h>
#include "../Manager/SceneManager.h"
#include "GameClear.h"

GameClear::GameClear(void)
{
}

GameClear::~GameClear(void)
{
}

void GameClear::Init(void)
{
}

void GameClear::Update(void)
{
    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
    }
}

void GameClear::Draw(void)
{
}

void GameClear::Release(void)
{
}

#include "EnemyManager.h"
#include  "../Player/Player.h"
#include <cmath>
#include "../../Scene/SceneManager.h"
#include "../../Scene/GameClear.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

EnemyManager* g_enemy_manager_instance = nullptr;

EnemyManager* EnemyManager::GetInstance() { return g_enemy_manager_instance; }
void EnemyManager::SetInstance(EnemyManager* instance) { g_enemy_manager_instance = instance; }

EnemyManager::EnemyManager(Player* player)
{
	slimes.clear();
	spawnedCount = 0;
	framesSinceLastSpawn = 0;
	killedCount = 0;
	player_ = player;
}

EnemyManager::~EnemyManager()
{
    Release();
}


void EnemyManager::Init(float x, float y, float z)
{
    SlimeEnemy* slime = new SlimeEnemy(player_);
    slime->Init(x, y, z);
    slimes.push_back(slime);

    spawnedCount++;
}

void EnemyManager::Update()
{
    // プレイヤー位置取得
    VECTOR playerPos = player_->GetPos();

    // スライムの更新
    for (auto slime : slimes) {
        slime->Update();
    }

    // スポーン制御
    if (spawnedCount < maxSlimes) {
        framesSinceLastSpawn++;
        if (framesSinceLastSpawn >= spawnInterval) {
            framesSinceLastSpawn = 0;

            // ランダムな角度を決定
            float angle = (float)(rand() % 360) * (M_PI / 180.0f);

            // 円周上に配置
            float sx = playerPos.x + cosf(angle) * spawnRadius;
            float sy = playerPos.y;
            float sz = playerPos.z + sinf(angle) * spawnRadius;

            Init(sx, sy, sz);
        }
    }

    // 死んだ敵を削除
    slimes.erase(
        std::remove_if(slimes.begin(), slimes.end(),
            [this](SlimeEnemy* s) {

                if (!s) return true;

                // 死亡エフェクト
                if (s->IsDeadEffect()) {
                    if (!s->GetAlive()) {
                        AddKilledCount(1);

                        // ゲームクリア判定
                        if (GetKilledCount() >= 250) {
                            // シーン切替
                            SceneManager::GetInstance()->ChangeScene(std::make_shared<GameClear>());
                        }

                        delete s;
                        return true;
                    }
                    return false; // 点滅中は残す
                }

                // 通常の即時死亡
                if (!s->GetAlive()) {
                    AddKilledCount(1);

                    if (GetKilledCount() >= 250) {
                        SceneManager::GetInstance()->ChangeScene(std::make_shared<GameClear>());
                    }

                    delete s;
                    return true;
                }

                return false;
            }
        ),
        slimes.end()
    );
}

void EnemyManager::Draw()
{
    for (auto slime : slimes) {
        slime->Draw();
    }

    DrawFormatString(
        0, 70, 0xffffff,
        "スライム数 : %d / %d", (int)slimes.size(), maxSlimes
    );
}

void EnemyManager::Release()
{
    for (auto slime : slimes) {
        delete slime;
    }
    slimes.clear();
    spawnedCount = 0;
    framesSinceLastSpawn = 0;

    ResetKilledCount();
}

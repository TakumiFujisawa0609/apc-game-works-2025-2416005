#include "SlimeManager.h"
#include  "../../Player/Player.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SlimeManager* g_slime_manager_instance = nullptr;

SlimeManager* SlimeManager::GetInstance() { return g_slime_manager_instance; }
void SlimeManager::SetInstance(SlimeManager* instance) { g_slime_manager_instance = instance; }

SlimeManager::SlimeManager() 
{

}

SlimeManager::~SlimeManager() 
{
    Release(); 
}


void SlimeManager::Spawn(float x, float y, float z)
{
    SlimeEnemy* slime = new SlimeEnemy();
    slime->Init(x, y, z);
    slimes.push_back(slime);

    spawnedCount++;
}

void SlimeManager::Update() 
{
    // プレイヤー位置取得
    VECTOR playerPos = Player::GetInstance()->GetPos();

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

            Spawn(sx, sy, sz);
        }
    }

    // 死んだ敵を削除
    slimes.erase(
        std::remove_if(slimes.begin(), slimes.end(),
            [](SlimeEnemy* s) {
                if (!s->GetAlive()) {
                    delete s;
                    return true; // erase 対象
                }
                return false;
            }),
        slimes.end());
}

void SlimeManager::Draw() 
{
    for (auto slime : slimes) {
        slime->Draw();
    }

    DrawFormatString(
        0, 90, 0xffffff,
        "スライム数 : %d / %d", (int)slimes.size(), maxSlimes
    );
}

void SlimeManager::Release() 
{
    for (auto slime : slimes) {
        delete slime;
    }
    slimes.clear();
    spawnedCount = 0;
    framesSinceLastSpawn = 0;
}

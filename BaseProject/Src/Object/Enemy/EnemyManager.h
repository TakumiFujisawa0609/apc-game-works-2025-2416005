#pragma once
#include <vector>
#include "../Enemy/Slime/SlimeEnemy.h"

class Player;
class EnemyManager {
public:
    static EnemyManager* GetInstance();
    static void SetInstance(EnemyManager* instance);

    static void ResetInstance() { instance_ = nullptr; }

private:
    std::vector<SlimeEnemy*> slimes;

public:
    EnemyManager(Player* player);
    ~EnemyManager();

    void Init(float x, float y, float z);
    void Update();
    void Draw();
    void Release();

    std::vector<SlimeEnemy*> GetSlimes() const { return slimes; }

    int GetEnemyCount() const { return (int)slimes.size(); }
    int GetKilledCount() const { return killedCount; }
    void ResetKilledCount() { killedCount = 0; }

    void AddKilledCount(int n) { killedCount += n; }

private:
    int maxSlimes = 100;
    int spawnedCount = 0;
    int framesSinceLastSpawn = 0;
    int spawnInterval = 180;
    float spawnRadius = 1000.0f;

    // “|‚µ‚½“G‚Ì—ÝŒv
    int killedCount = 0;

    static EnemyManager* instance_;

	Player* player_ = nullptr;
};

#pragma once
#include <vector>
#include "../Enemy/Slime/SlimeEnemy.h"

class EnemyManager {
public:
    static EnemyManager* GetInstance();
    static void SetInstance(EnemyManager* instance);

private:
    std::vector<SlimeEnemy*> slimes;

public:
    EnemyManager();
    ~EnemyManager();

    void Init(float x, float y, float z);
    void Update();
    void Draw();
    void Release();

    const std::vector<SlimeEnemy*>& GetSlimes() const { return slimes; }

private:
    int maxSlimes = 250;
    int spawnedCount = 0;
    int framesSinceLastSpawn = 0;
    int spawnInterval = 1;
    float spawnRadius = 1000.0f;

    // “|‚µ‚½“G‚Ì—İŒv
    int killedCount = 0;

    // “|‚µ‚½”‚ğ‘‚â‚·
    void AddKilledCount(int n = 1) { killedCount += n; }

    // “|‚µ‚½”‚ğæ“¾
    int GetKilledCount() const { return killedCount; }

    // “|‚µ‚½”‚ğƒŠƒZƒbƒg
    void ResetKilledCount() { killedCount = 0; }
};

#pragma once
#include <vector>
#include "SlimeEnemy.h"

class SlimeManager {

private:
    std::vector<SlimeEnemy*> slimes;

public:
    SlimeManager();
    ~SlimeManager();

    void Spawn(float x, float y, float z); // ƒXƒ‰ƒCƒ€oŒ»
    void Update();
    void Draw();
    void Release();
};

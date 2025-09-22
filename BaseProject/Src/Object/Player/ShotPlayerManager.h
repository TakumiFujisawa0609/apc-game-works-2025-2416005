#pragma once
#include <DxLib.h>
#include <vector>
#include "ShotPlayer.h"

class ShotPlayerManager
{
public:
    static ShotPlayerManager& GetInstance()
    {
        static ShotPlayerManager instance;
        return instance;
    }

    void AddShot(const VECTOR& pos, const VECTOR& dir);
    void Update();
    void Draw();
    void Clear();

private:
    ShotPlayerManager() {}
    std::vector<ShotPlayer*> shots_;
};
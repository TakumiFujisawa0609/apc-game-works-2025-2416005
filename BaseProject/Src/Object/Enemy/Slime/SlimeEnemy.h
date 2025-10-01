#pragma once
#include "../EnemyBase.h"
#include <DxLib.h>
#include <vector>
#include "../../Player/ShotPlayer.h"

class SlimeEnemy : public EnemyBase {
public:

    bool isCounted = false;

    SlimeEnemy();
    ~SlimeEnemy();

    void Init(float _x, float _y, float _z) override;
    void Update() override;
    void Draw() override;
    void Release() override;

    VECTOR pos = VGet(x, y, z);

    VECTOR GetPos() const { return VGet(x, y, z); }
    float GetRadius() const { return 15.0f; }

private:
    float moveSpeed;
    int color;

    // ƒ‚ƒfƒ‹î•ñ
    int modelId_;
};

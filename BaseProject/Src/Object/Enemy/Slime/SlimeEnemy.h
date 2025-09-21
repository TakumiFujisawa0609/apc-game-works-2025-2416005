#pragma once
#include "../EnemyBase.h"

// スライム敵クラス
class SlimeEnemy : public EnemyBase {

private:
    float moveSpeed;   // 移動速度
    int color;         // 色

public:
    SlimeEnemy();
    ~SlimeEnemy();

    void Init(float _x, float _y, float _z) override;
    void Update() override;
    void Draw() override;
    void Release() override;

    VECTOR pos = VGet(x, y, z);
};

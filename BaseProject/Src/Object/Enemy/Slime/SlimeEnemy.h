#pragma once
#include "../EnemyBase.h"
#include <DxLib.h>
#include <vector>
#include "../../Player/ShotPlayer.h"

// スライム敵クラス
class SlimeEnemy : public EnemyBase {

public:
    SlimeEnemy();
    ~SlimeEnemy();

    void Init(float _x, float _y, float _z) override;
    void Update() override;
    void Draw() override;
    void Release() override;

    VECTOR pos = VGet(x, y, z);

    bool IsAlive() const { return isAlive; }
    void Kill() { isAlive = false; }

    VECTOR GetPos() const { return VGet(x, y, z); }
    float GetRadius() const { return 15.0f; }
private:
    float moveSpeed;   // 移動速度
    int color;         // 色
    bool isAlive = true;
};

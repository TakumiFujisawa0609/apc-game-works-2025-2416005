#pragma once
#include <DxLib.h>

// 敵の基底クラス
class EnemyBase {

public:

    float x, y, z;   // 位置

    EnemyBase();
    virtual ~EnemyBase();

    virtual void Init(float _x, float _y, float _z); // 初期化
    virtual void Update();  // 更新処理
    virtual void Draw();    // 描画処理
    virtual void Release(); // 解放処理

    bool GetAlive() const { return isAlive; }
    void Kill() { isAlive = false; }
    VECTOR GetPos() const { return VGet(x, y, z); }


    VECTOR pos = VGet(x, y, z);

private:


    int hp;          // 体力
    bool isAlive;    // 生存フラグ

};

#include "EnemyBase.h"


EnemyBase::EnemyBase()
    : x(0.0f), y(0.0f), z(0.0f), hp(100), isAlive(true) 
{
}

EnemyBase::~EnemyBase() 
{
    Release();
}

void EnemyBase::Init(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
    pos = VGet(x, y, z);
    isAlive = true;
}

void EnemyBase::Update() 
{
    // 基底クラスでは特に処理なし
}

void EnemyBase::Draw()
{
    if (!isAlive) return;

    // デバッグ用：簡単に球で描画
    DrawSphere3D(
        VGet(x, y, z),    // 中心座標
        10.0f,            // 半径
        16,               // 分割数
        GetColor(255, 0, 0), // ディフューズカラー
        GetColor(255, 0, 0), // スペキュラカラー
        TRUE              // 塗りつぶし
    );
}

void EnemyBase::Release()
{
    // 解放処理（ここでは特になし）
}

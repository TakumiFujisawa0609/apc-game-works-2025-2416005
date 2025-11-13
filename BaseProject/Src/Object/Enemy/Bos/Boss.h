#pragma once
#include "../EnemyBase.h"

class Boss : public EnemyBase 
{
public:
    // コンストラクタ
	Boss();

	// デストラクタ
	~Boss();

    // 初期化
    void Init(float _x, float _y, float _z) override;

	// 更新
    void Update() override;

	// 描画
    void Draw() override;

	// 解放
    void Release() override;

	// 撃破
    void Kill() override;      


private:

};

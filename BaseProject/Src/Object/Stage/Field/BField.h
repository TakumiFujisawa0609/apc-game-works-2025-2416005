#pragma once
#include "FieldBase.h"

class Player;
class BField : public FieldBase
{
public:

	// コンストラクタ
	BField();

	//　デストラクタ
	~BField()override;

	// 初期化処理
	void Init(float _x, float _y, float _z, Player* player)override;

	// 更新ステップ
	void Update(void)override;

	// 描画処理
	void Draw(void)override;

	// 解放処理
	void Release(void)override;

private:
	Player* player;
};

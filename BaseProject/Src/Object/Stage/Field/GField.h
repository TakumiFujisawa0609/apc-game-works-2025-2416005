#pragma once
#include "FieldBase.h"

class Player;
class GField : public FieldBase
{
public:

	// コンストラクタ
	GField();

	//　デストラクタ
	~GField()override;

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

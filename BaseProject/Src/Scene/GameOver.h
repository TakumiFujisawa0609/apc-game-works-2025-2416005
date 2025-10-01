#pragma once
#include "SceneBase.h"

class GameOver : public SceneBase
{
public:

	// コンストラクタ
	GameOver(void);

	// デストラクタ
	~GameOver(void)override;

	// 初期化処理
	void Init(void)override;

	// 更新ステップ
	void Update(void)override;

	// 描画処理
	void Draw(void)override;

	// 解放処理
	void Release(void)override;

private:
};


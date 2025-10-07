#pragma once
class Grid;
class Player;
class EnemyManager;


class GameScene : public SceneBase
{
public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void)override;

	// 初期化処理
	void Init(void)override;

	// 更新ステップ
	void Update(void)override;

	// 描画処理
	void Draw(void)override;

	// 解放処理
	void Release(void)override;

private:

// グリッド線
Grid* grid_;

Player* player_;

EnemyManager* enemy_;

};

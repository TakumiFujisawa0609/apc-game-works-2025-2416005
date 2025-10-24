#pragma once
class Grid;
class Player;
class EnemyManager;
class Weapon;
class FieldManager;

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

	// 読み込み
	void Load(void) override;

	// 読み込み後の初期化
	void LoadEnd(void) override;

	// 描画処理
	void Draw(void)override;

	// 解放処理
	void Release(void)override;

private:

// グリッド線
Grid* grid_;

Player* player_;

EnemyManager* enemy_;

Weapon* weapon_;

FieldManager* fieldManager_;
};

#pragma once
#include "SceneBase.h"
class Grid;
class Player;
class SlimeManager;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void) override;

	void Init(void) override;
	void Update(void) override;
	void Load(void) override;
	void LoadEnd(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// グリッド線
	Grid* grid_;

	Player* player_;

	SlimeManager* slime_;

	int imageId_;
};

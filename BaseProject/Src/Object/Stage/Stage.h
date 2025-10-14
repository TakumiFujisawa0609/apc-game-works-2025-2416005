#pragma once
#include <DxLib.h>

class Player;
class Stage
{
public:
    // シングルトン（生成・取得・削除）
    static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new Stage(); } };
    static Stage* GetInstance(void) { return instance_; };
    static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }

private:
    // 静的インスタンス
    static Stage* instance_;
private:
    // デフォルトコンストラクタをprivateにして、外部から生成できない様にする
    Stage(void);
    // デストラクタも同様
    ~Stage(void);

    // コピー・ムーブ操作を禁止
    Stage(const Stage&) = delete;
    Stage& operator=(const Stage&) = delete;
    Stage(Stage&&) = delete;
    Stage& operator=(Stage&&) = delete;

    // 下記をコンパイルエラーさせるため 上記を追加
    // Item copy = *Item::GetInstance();
    // Item copied(*Item::GetInstance());
    // Item moved = std::move(*Item::GetInstance());
public:

	bool Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

    float GetGroundHeight(float x, float z); // 地面の高さを返す

	void SetPlayer(Player* player) { player_ = player; }

private:
	// ステージモデルのハンドルID
	int modelId_;

	// ステージモデルの位置
	VECTOR pos_;

	// 雲の画像ハンドルID
	int kumoImageId_{ -1 };

	// 雲の位置
    VECTOR KumoPos_;

	// 都市の画像ハンドルID
	int toshiImageId_{ -1 };

	// 都市の位置
    VECTOR ToshiPos_;

	Player* player_{ nullptr };
};
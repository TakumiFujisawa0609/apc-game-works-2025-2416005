#pragma once
#include <vector>
#include <DxLib.h>

class Field
{
public:
    // シングルトン（生成・取得・削除）
    static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new Field(); } };
    static Field* GetInstance(void) { return instance_; };
    static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }

private:
    // 静的インスタンス
    static Field* instance_;
private:
    // デフォルトコンストラクタをprivateにして、外部から生成できない様にする
    Field(void);
    // デストラクタも同様
    ~Field(void);

    // コピー・ムーブ操作を禁止
    Field(const Field&) = delete;
    Field& operator=(const Field&) = delete;
    Field(Field&&) = delete;
    Field& operator=(Field&&) = delete;

    // 下記をコンパイルエラーさせるため 上記を追加
    // Item copy = *Item::GetInstance();
    // Item copied(*Item::GetInstance());
    // Item moved = std::move(*Item::GetInstance());
public:
	void Init();
	void Update();
	void Draw();
	void Release();

    // --- フィールド範囲を返す関数を追加 ---
    float GetMinX() const { return -1500.0f; } 
    float GetMaxX() const { return 1500.0f; }
    float GetMinZ() const { return -1500.0f; }
    float GetMaxZ() const { return 1500.0f; }

private:

	int modelId_;
	int modelId2_;
	VECTOR pos_;
	VECTOR pos2_;

	std::vector<VECTOR> fieldPositions_;
};


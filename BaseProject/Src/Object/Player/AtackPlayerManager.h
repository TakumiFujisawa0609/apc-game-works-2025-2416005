#pragma once
#include <DxLib.h>
#include <vector>

class AtackPlayerManager
{
public:
	// シングルトン（生成・取得・削除）
	static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new AtackPlayerManager(); } };
	static AtackPlayerManager* GetInstance(void) { return instance_; };
	static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }

private:
	// 静的インスタンス
	static AtackPlayerManager* instance_;
private:
	// デフォルトコンストラクタをprivateにして、外部から生成できない様にする
	AtackPlayerManager(void);
	// デストラクタも同様
	~AtackPlayerManager(void);

	// コピー・ムーブ操作を禁止
	AtackPlayerManager(const AtackPlayerManager&) = delete;
	AtackPlayerManager& operator=(const AtackPlayerManager&) = delete;
	AtackPlayerManager(AtackPlayerManager&&) = delete;
	AtackPlayerManager& operator=(AtackPlayerManager&&) = delete;

	// 下記をコンパイルエラーさせるため 上記を追加
	// Item copy = *Item::GetInstance();
	// Item copied(*Item::GetInstance());
	// Item moved = std::move(*Item::GetInstance());

public:

	void AddAtack(const VECTOR& pos, const VECTOR& dir);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

};
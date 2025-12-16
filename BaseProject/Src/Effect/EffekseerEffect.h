#pragma once
#include <DxLib.h>
#include <EffekseerForDXLib.h>

class EffekseerEffect
{
public:
    // シングルトン（生成・取得・削除）
    static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new EffekseerEffect(); } };
    static EffekseerEffect* GetInstance(void) { return instance_; };
    static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } }

private:
    // 静的インスタンス
    static EffekseerEffect* instance_;
private:
    // デフォルトコンストラクタをprivateにして、外部から生成できない様にする
    EffekseerEffect(void);
    // デストラクタも同様
    ~EffekseerEffect(void);

    // コピー・ムーブ操作を禁止
    EffekseerEffect(const EffekseerEffect&) = delete;
    EffekseerEffect& operator=(const EffekseerEffect&) = delete;
    EffekseerEffect(EffekseerEffect&&) = delete;
    EffekseerEffect& operator=(EffekseerEffect&&) = delete;

public:
	// 初期化
	void Init(void);
    
	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 解放
	void Release(void);

private:
    int shalshutEffectId_;
};

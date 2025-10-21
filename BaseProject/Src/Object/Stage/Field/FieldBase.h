#pragma once
#include <DxLib.h>
#include <string>

class FieldBase
{
public:

	// モデルの大きさ
	static constexpr VECTOR SCALES = { 3.0f, 3.0f, 3.0f };

	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 300.0f, 10.0f, 300.0f };


	// コンストラクタ
	FieldBase(float x = 0, float y = 0, float z = 0);

	// デストラクタ
	virtual ~FieldBase(void) = 0;

	// 初期化処理
	virtual void Init(void) = 0;

	// 更新ステップ
	virtual void Update(void) = 0;

	// 描画処理
	virtual void Draw(void) = 0;

	// 解放処理
	virtual void Release(void) = 0;

	// フィールドのスケール取得
	VECTOR scales_;


	bool IsCleared() const { return isCleared_; }
	const std::string& GetName() const { return name_; }

private:

	std::string name_;
	VECTOR center_;
	float radius_;
	bool isCleared_;
};

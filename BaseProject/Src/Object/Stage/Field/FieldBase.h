#pragma once
#include <DxLib.h>

class FieldBase
{
public:

	enum class TYPE
	{
		A_FIELD,
		B_FIELD
	};

	// モデルの大きさ
	static constexpr VECTOR SCALES = { 3.0f, 3.0f, 3.0f };

	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 300.0f, 10.0f, 300.0f };


	// コンストラクタ
	FieldBase(float x, float y, float z);

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

private:

};

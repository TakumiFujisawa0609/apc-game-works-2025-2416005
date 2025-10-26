#pragma once
#include "FieldBase.h"
#include <map>
#include <memory>
#include <DxLib.h>
#include <vector>

class FieldManager
{
public:


	enum class FieldID
	{
		A_FIELD,
		B_FIELD,
		MAX
	};

	// コンストラクタ
	FieldManager();

	// デストラクタ
	~FieldManager();

	// 初期化処理
	void Init(void);

	// 更新ステップ
	void Update(void);

	// 描画処理
	void Draw(void);

	// 解放処理
	void Release(void);

	FieldBase* GetField(FieldID id);

private:

	std::vector<std::unique_ptr<FieldBase>> fields_;
	FieldID currentField_ = FieldID::A_FIELD;

};


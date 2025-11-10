#include "FieldManager.h"
#include "BField.h"
#include "AField.h"
#include "CField.h"
#include "DField.h"
#include "EField.h"
#include "FField.h"
#include "GField.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Scene/GameClear.h"


FieldManager::FieldManager()
    : currentFieldName_("不明")
{

}

FieldManager::~FieldManager()
{

}

void FieldManager::Init(Player* player)
{
    fields_.push_back(std::make_unique<AField>());
    fields_.back()->Init(300.0f, 0.0f, 300.0f, player);

    fields_.push_back(std::make_unique<BField>());
    fields_.back()->Init(5000.0f, 0.0f, 4000.0f,player);

	fields_.push_back(std::make_unique<CField>());
    fields_.back()->Init(-5000.0f, 0.0f, 4000.0f,player);

    fields_.push_back(std::make_unique<DField>());
    fields_.back()->Init(-5000.0f, 0.0f, -4000.0f,player);

    fields_.push_back(std::make_unique<EField>());
    fields_.back()->Init(-5000.0f, 0.0f, -4000.0f,player);

    fields_.push_back(std::make_unique<FField>());
    fields_.back()->Init(10000.0f, 0.0f, 10000.0f,player);

    fields_.push_back(std::make_unique<GField>());
	fields_.back()->Init(-10000.0f, 0.0f, -10000.0f, player);
}

void FieldManager::Update(void)
{
    currentFieldName_ = "不明";

    for (auto& field : fields_) {
        field->Update();
    }

    // 全制圧チェック
    bool allCaptured = true;
    for (auto& field : fields_) {
        if (field->GetState() != FieldBase::FieldState::PLAYER) {
            allCaptured = false;
            break;
        }
    }


    if (allCaptured) {
        //SceneManager::GetInstance()->ChangeScene(std::make_shared<GameClear>());

    }
}

void FieldManager::Draw(void)
{
    for (auto& field : fields_) {
        field->Draw();
    }

    //// 現在地を画面に表示
    //DrawFormatString(50, 50, GetColor(255, 255, 255),
    //    "現在地：%s", currentFieldName_.c_str());

}

void FieldManager::Release(void)
{

}

FieldBase* FieldManager::GetField(FieldID id)
{
    return fields_[static_cast<int>(id)].get();
}

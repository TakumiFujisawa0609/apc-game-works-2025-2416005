#include "FieldManager.h"
#include "BField.h"
#include "AField.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Scene/GameClear.h"


FieldManager::FieldManager()
{

}

FieldManager::~FieldManager()
{

}

void FieldManager::Init(Player* player)
{
    //fields_.push_back(std::make_unique<AField>());
    //fields_.back()->Init(3000.0f, 0.0f, 3000.0f,player);

    fields_.push_back(std::make_unique<BField>());
    fields_.back()->Init(300.0f, 0.0f, 300.0f,player);
}

void FieldManager::Update(void)
{
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
        SceneManager::GetInstance()->ChangeScene(std::make_shared<GameClear>());

    }
}

void FieldManager::Draw(void)
{
    for (auto& field : fields_) {
        field->Draw();
    }
}

void FieldManager::Release(void)
{

}

FieldBase* FieldManager::GetField(FieldID id)
{
    return fields_[static_cast<int>(id)].get();
}

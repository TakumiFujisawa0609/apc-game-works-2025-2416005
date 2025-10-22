#include "FieldManager.h"
#include "FieldBase.h"
#include "BField.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Scene/GameClear.h"


FieldManager::FieldManager()
{

}

FieldManager::~FieldManager()
{

}

void FieldManager::Init(void)
{
	auto fieldA = std::make_unique<BField>();
	fieldA->Init(0, 0, 0);
	fields_.emplace(FieldID::A_FIELD, std::move(fieldA));

	auto fieldB = std::make_unique<BField>();
	fieldB->Init(300, 0, 300);
	fields_.emplace(FieldID::B_FIELD, std::move(fieldB));
}

void FieldManager::Update(void)
{
    for (auto it = fields_.begin(); it != fields_.end(); ++it)
    {
        it->second->Update();
    }

    // 制圧チェック
    bool allCaptured = true;
    for (auto it = fields_.begin(); it != fields_.end(); ++it)
    {
        if (!it->second->IsCapturedBy(FieldBase::FieldState::PLAYER))
        {
            allCaptured = false;
            break;
        }
    }

    if (allCaptured)
    {
        // SceneManager経由でゲームクリア画面へ
        SceneManager::GetInstance()->ChangeScene(std::make_shared<GameClear>());
    }
}

void FieldManager::Draw(void)
{

}

void FieldManager::Release(void)
{

}

FieldBase* FieldManager::GetField(FieldID id)
{
	auto it = fields_.find(id);
	if (it != fields_.end()) {
		return it->second.get();
	}
}

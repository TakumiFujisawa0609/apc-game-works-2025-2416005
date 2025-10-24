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
    fields_[FieldID::B_FIELD] = std::make_unique<BField>();
    fields_[FieldID::B_FIELD]->Init(300.0f, 0.0f, 0.0f);
}

void FieldManager::Update(void)
{

}

void FieldManager::Draw(void)
{

}

void FieldManager::Release(void)
{

}

FieldBase* FieldManager::GetField(FieldID id)
{
	return fields_[id].get();
}

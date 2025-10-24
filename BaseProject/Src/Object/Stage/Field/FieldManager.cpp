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

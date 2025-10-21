#include "FieldManager.h"

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
	auto it = fields_.find(id);
	if (it != fields_.end()) {
		return it->second.get();
	}
}

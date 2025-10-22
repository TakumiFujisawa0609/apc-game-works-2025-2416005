#include "FieldBase.h"


FieldBase::FieldBase(void)
{
	scales_ = SCALES;
	durability_ = 100;
}

FieldBase::~FieldBase(void)
{

}

void FieldBase::Init(float _x, float _y, float _z)
{

}

void FieldBase::Update(void)
{

}

void FieldBase::Draw(void)
{

}

void FieldBase::Release(void)
{

}

void FieldBase::Damage(int dmg)
{
	durability_ -= dmg;
	if (durability_ > 0) 
	{

	}
	else
	{
		durability_ = 0;
	}

}

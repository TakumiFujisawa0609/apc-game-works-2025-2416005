#pragma once
#include <vector>
#include <DxLib.h>

class Field
{
public:
	Field();
	~Field();

	void Init();
	void Update();
	void Draw();
	void Release();

private:

	int modelId_;
	int modelId2_;
	VECTOR pos_;
	VECTOR pos2_;

	std::vector<VECTOR> fieldPositions_;
};


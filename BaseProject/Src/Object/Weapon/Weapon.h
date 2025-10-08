#pragma once

class Weapon
{
public:
	Weapon();
	~Weapon();

	void Init();
	void Update();
	void Draw();
	void Release();

private:

	int modelId_;

	VECTOR pos_;
};


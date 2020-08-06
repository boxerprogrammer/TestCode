#pragma once
class Input;
class InputListener
{
public:
	virtual void Notify(const Input& input) = 0;
};


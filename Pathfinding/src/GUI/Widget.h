#pragma once
#include <iostream>


class Widget
{
public:
	Widget() = default;
	~Widget() = default;

	virtual void Draw() { std::cout << " asd" << std::endl; }
};


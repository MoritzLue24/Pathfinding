#pragma once
#include "Widget.h"


class Text : public Widget
{
public:
	Text(const char* text) : text(text) {}
	~Text() {}

	void Draw() override;

	const char* text;
};


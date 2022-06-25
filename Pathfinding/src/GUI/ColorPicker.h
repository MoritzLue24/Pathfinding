#pragma once
#include <string>
#include <array>
#include <imgui.h>

#include "Widget.h"


class ColorPicker
{
public:
	ColorPicker(const std::string& label, float colorArray[4]);
	~ColorPicker();

	void Draw(std::array<float, 2> buttonSize = {0.0f, 0.0f});
	bool m_ButtonPressed = false;

private:
	std::string m_Label;
	float* m_ColorArray;
};


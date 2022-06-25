#include "ColorPicker.h"
#include <iostream>


ColorPicker::ColorPicker(const std::string& label, float colorArray[4])
	: m_Label(label), m_ColorArray(colorArray)
{

}

ColorPicker::~ColorPicker()
{

}

void ColorPicker::Draw(std::array<float, 2> buttonSize)
{
	if (ImGui::Button(m_Label.c_str(), { buttonSize[0], buttonSize[1] }))
		m_ButtonPressed = !m_ButtonPressed;

	if (m_ButtonPressed) 
		ImGui::ColorPicker4(m_Label.c_str(), m_ColorArray);
}

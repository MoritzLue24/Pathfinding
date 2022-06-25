#include "GUIWindow.h"
#include <iostream>


GUIWindow::GUIWindow(const char* title, std::array<float, 2> size, std::array<float, 2> position, int flags)
	: m_Title(title), m_Size(size), m_Position(position), m_Flags(flags)
{

}

GUIWindow::~GUIWindow()
{

}

void GUIWindow::Draw()
{
	ImGui::Begin(m_Title, nullptr, m_Flags);
	ImGui::SetWindowSize({ m_Size[0], m_Size[1] });
	ImGui::SetWindowPos({ m_Position[0], m_Position[1] });

	m_Hovered = ImGui::IsWindowHovered();

	for (auto& widget : m_Widgets)
	{
		widget->Draw();
	}

	ImGui::End();
}

void GUIWindow::AddWidget(std::unique_ptr<Widget> widget)
{
	m_Widgets.push_back(std::move(widget));
}

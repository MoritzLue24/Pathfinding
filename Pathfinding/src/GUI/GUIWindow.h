#pragma once
#include <array>
#include <vector>
#include <imgui.h>

#include "Widget.h"


class GUIWindow
{
public:
	GUIWindow(const char* title, std::array<float, 2> size, std::array<float, 2> position, int flags = 0);
	~GUIWindow();

	void Draw();
	void AddWidget(std::unique_ptr<Widget> widget);

	inline bool isHovered() const { return m_Hovered; }

private:
	const char* m_Title;
	std::array<float, 2> m_Size, m_Position;
	int m_Flags;
	bool m_Hovered = false;
	std::vector<std::unique_ptr<Widget>> m_Widgets;
};


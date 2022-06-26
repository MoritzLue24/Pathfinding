#pragma once
#include <vector>

#include "BaseWindow.h"
#include "Grid.h"
#include "Algorithms/AStar.h"
#include "GUI/ColorPicker.h"


class Application : public BaseWindow
{
public:
	Application(const char* title, uint32_t rows, uint32_t columns, uint32_t cellSize = 20, uint32_t nodeSize = 10, uint32_t nodeMargin = 1);
	~Application() {}

private:
	void HandleEvents(sf::Event event) override;
	void Render(sf::RenderWindow& window) override;

	Grid m_Grid;
	AStar m_AStar;
	std::vector<std::string> m_Tools{ "start", "end", "wall", "delete"};
	std::string m_CurrentTool = m_Tools[0];

	sf::FloatRect m_ConfigRect = {0.0f, 0.0f, 300.0f, (float)m_Window.getSize().y};
	bool m_GUIHovered = false;
	std::vector<ColorPicker> m_ColorPickers;
	bool m_ColorWindowOpen = false;
};
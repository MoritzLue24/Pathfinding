#include "Application.h"
#include <iostream>
#include <memory>
#include <string>
#include <imgui.h>

#include "GUI/ColorPicker.h"


Application::Application(const char* title, uint32_t rows, uint32_t columns)
	: m_Grid(rows, columns, 20, 10, 1), BaseWindow(title, columns* (20 + 1), rows* (20 + 1)), m_AStar(m_Grid)
{
	for (auto& color : Node::colors) 
		m_ColorPickers.emplace_back(color.first, color.second.arr);
	m_ColorPickers.emplace_back("Grid color", m_Grid.lineColor);

	ImGui::GetStyle().ItemSpacing = { 0.0f, 10.0f };
}

void Application::HandleEvents(sf::Event event)
{
	if (m_GUIHovered)
		return;

	// Create start & end node
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && (m_CurrentTool == "end" || m_CurrentTool == "start"))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(m_Window);
		Node& node = m_Grid.GetNodeByPos(m_Grid.CoordToGrid((uint32_t)mousePos.y), m_Grid.CoordToGrid((uint32_t)mousePos.x));

		try
		{
			m_Grid.GetNodeByType(m_CurrentTool).type = "empty";
		} 
		catch (NodeNotFoundException)
		{
			std::cout << m_CurrentTool << " node cannot be deleted. New " << m_CurrentTool << " node has been created" << std::endl;
		}
		node.type = m_CurrentTool;
	}
}

void Application::Render(sf::RenderWindow& window)
{
	// Add wall & remove node events
	if (!m_GUIHovered)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (m_CurrentTool == "wall")
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(m_Window);
				m_Grid.GetNodeByPos(m_Grid.CoordToGrid((uint32_t)mousePos.y), m_Grid.CoordToGrid((uint32_t)mousePos.x)).type = "wall";
			}
			else if (m_CurrentTool == "delete")
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(m_Window);
				m_Grid.GetNodeByPos(m_Grid.CoordToGrid((uint32_t)mousePos.y), m_Grid.CoordToGrid((uint32_t)mousePos.x)).type = "empty";
			}
		}
	}

	m_AStar.Update();
	m_Grid.Draw(window);
	m_AStar.RenderPath(window);

	// GUI
	ImGui::Begin("Configuration", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::SetWindowSize({ m_ConfigRect.width, m_ConfigRect.height });
	ImGui::SetWindowPos({ m_ConfigRect.left, m_ConfigRect.top });

	// If the mouse is inside the config menu, set m_GUIHovered to true
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_Window);
	m_GUIHovered = m_ConfigRect.intersects({ (float)mousePos.x, (float)mousePos.y, 1.0f, 1.0f });

	// FPS text
	ImGui::LabelText(std::to_string(getFps()).c_str(), "FPS");

	// Check boxes
	ImGui::Checkbox("Diagonals", &m_Grid.diagonals);
	ImGui::Checkbox("Draw grid", &m_Grid.drawGrid);
	
	// Tools
	if (ImGui::BeginCombo("Tools", m_CurrentTool.c_str()))
	{
		for (const std::string& tool : m_Tools)
		{
			bool selected = (m_CurrentTool == tool);
			if (ImGui::Selectable(tool.c_str(), selected))
				m_CurrentTool = tool;
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	// Node size
	ImGui::SliderInt("Node size", &m_Grid.nodeSize, 0, m_Grid.getCellSize() * 4);

	// Reset
	if (ImGui::Button("Reset", { m_ConfigRect.width / 2 - 13, 30 }))
	{
		m_AStar.Reset();
		m_Grid.Reset();
	}

	// Run
	ImGui::SameLine(m_ConfigRect.width / 2, 13.0f / 2);
	if (ImGui::Button("Run", { m_ConfigRect.width / 2 - 13, 30 }))
	{
		m_Grid.UpdateNeighbors();
		m_AStar.Run();
	}

	// Color pickers
	if (ImGui::Button("Colors", { m_ConfigRect.width - 15, 30 }))
		m_ColorWindowOpen = !m_ColorWindowOpen;

	if (!m_ColorWindowOpen)
	{
		ImGui::BeginChildFrame(1, { 200, 400 });
		ImGui::Text("Colors");
		for (ColorPicker& colorPicker : m_ColorPickers)
			colorPicker.Draw({ 185, 20 });
		ImGui::EndChildFrame();
	}
	ImGui::End();
}

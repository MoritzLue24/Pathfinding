#include "Application.h"

#include <iostream>
#include <memory>
#include <string>
#include <imgui.h>

#include "GUI/ColorPicker.h"


Application::Application(const char* title, uint32_t rows, uint32_t columns, uint32_t cellSize, uint32_t nodeSize, uint32_t nodeMargin)
	: m_Grid(rows, columns, cellSize, nodeSize, nodeMargin), BaseWindow(title, columns * (cellSize + nodeMargin), rows* (cellSize + nodeMargin)), m_AStar(m_Grid)
{
	for (auto& color : Node::colors) 
		m_ColorPickers.emplace_back(color.first, color.second.arr);
	m_ColorPickers.emplace_back("Grid color", m_Grid.lineColor);

	ImGui::GetStyle().ItemSpacing = { 0, 10 };
	ImGui::GetStyle().WindowPadding = { 10, 10 };
	ImGui::GetStyle().FramePadding = { 5, 5 };
	ImGui::GetStyle().ScrollbarSize = 10;
}

void Application::HandleEvents(sf::Event event)
{
	if (m_GUIHovered)
		return;

	// Create start / end node on left mouse click depending on the m_CurrentTool
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && (m_CurrentTool == "end" || m_CurrentTool == "start"))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(m_Window);
		Node& node = m_Grid.GetNodeByPos(m_Grid.CoordToGrid(mousePos.y), m_Grid.CoordToGrid(mousePos.x));

		try
		{
			// Set the current start / end node to empty to prevent the user from creating more than one start / end node
			m_Grid.GetNodeByType(m_CurrentTool).type = "empty";
		} 
		catch (NodeNotFoundException) {}
		node.type = m_CurrentTool;
	}
}

void Application::Render(sf::RenderWindow& window)
{
	if (!m_GUIHovered)
	{
		// Create wall / delete node, when the user is pressing / holding the left mouse button (depending on the m_CurrentTool)
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (m_CurrentTool == "wall")
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(m_Window);
				m_Grid.GetNodeByPos(m_Grid.CoordToGrid(mousePos.y), m_Grid.CoordToGrid(mousePos.x)).type = "wall";
			}
			else if (m_CurrentTool == "delete")
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(m_Window);
				m_Grid.GetNodeByPos(m_Grid.CoordToGrid(mousePos.y), m_Grid.CoordToGrid(mousePos.x)).type = "empty";
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
	m_GUIHovered = m_ConfigRect.intersects({ (float)mousePos.x, (float)mousePos.y, 1, 1 });

	// FPS text
	ImGui::LabelText(std::to_string(getFps()).c_str(), "Fps");

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
	ImGui::SliderInt("Node size", &m_Grid.nodeSize, 0, 100);

	// Reset
	float padding = ImGui::GetStyle().WindowPadding.x;
	float btnWidth = m_ConfigRect.width - 2 * padding;
	float btnHeight = 30;
	if (ImGui::Button("Reset", { btnWidth / 2 - padding / 2, btnHeight }))
	{
		m_AStar.Reset();
		m_Grid.Reset();
	}

	// Run
	ImGui::SameLine(m_ConfigRect.width / 2, padding / 2);
	if (ImGui::Button("Run", { btnWidth / 2 - padding / 2, btnHeight }))
	{
		m_Grid.UpdateNeighbors();
		m_AStar.Run();
	}

	// Color pickers
	if (ImGui::Button("Colors", { btnWidth, btnHeight }))
		m_ColorWindowOpen = !m_ColorWindowOpen;

	if (!m_ColorWindowOpen)
	{
		auto framePadding = ImGui::GetStyle().FramePadding;
		ImGui::BeginChildFrame(1, { 200, (float)m_ColorPickers.size() * (20 + framePadding.y * 2)});

		for (ColorPicker& colorPicker : m_ColorPickers)
			colorPicker.Draw({ 200 - 2 * framePadding.x, 20 });

		ImGui::EndChildFrame();
	}
	ImGui::End();
}
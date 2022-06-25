#include "BaseWindow.h"
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>


BaseWindow::BaseWindow(const char* title, uint32_t width, uint32_t height, uint32_t maxFps)
	: m_Window(sf::VideoMode({ width, height }), title, sf::Style::Close | sf::Style::Titlebar)
{
	m_Window.setFramerateLimit(maxFps);

	// Init ImGui
	ImGui::SFML::Init(m_Window);
	ImGui::GetIO().IniFilename = nullptr;
}

BaseWindow::~BaseWindow()
{
	ImGui::SFML::Shutdown();
}

void BaseWindow::Run()
{
	sf::Clock deltaClock;
	while (m_Window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (m_Window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
				m_Window.close();
			HandleEvents(event);
		}

		// Calculate delta time & fps
		sf::Time dtObj = deltaClock.restart();
		m_DeltaTime = dtObj.asSeconds();
		m_Fps = 1.0f / m_DeltaTime;

		// Update ImGui
		ImGui::SFML::Update(m_Window, dtObj);

		// Rendering
		m_Window.clear(clearColor);
		Render(m_Window);

		ImGui::SFML::Render(m_Window);
		m_Window.display();
	}
}
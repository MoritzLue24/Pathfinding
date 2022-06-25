#pragma once
#include <SFML/Graphics.hpp>


class BaseWindow
{
public:
	BaseWindow(const char* title, uint32_t width, uint32_t height, uint32_t maxFps = 60);
	~BaseWindow();

	virtual inline float getFps() const final { return m_Fps; }

	virtual void Run() final;
	virtual void HandleEvents(sf::Event event) {}
	virtual void Render(sf::RenderWindow& window) {}

	sf::Color clearColor = { 30, 30, 30 };

protected: 
	sf::RenderWindow m_Window;

private:
	float m_DeltaTime = 0.0f;
	float m_Fps = 0.0f;
};
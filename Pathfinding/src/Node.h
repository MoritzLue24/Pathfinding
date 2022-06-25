#pragma once
#include <unordered_map>
#include <functional>
#include <SFML/Graphics.hpp>


struct Color
{
	Color(uint8_t r, uint8_t g, uint8_t b) : arr{ (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f } {}
	Color() : arr{ 0.0f, 0.0f, 0.0f, 1.0f } {}
	~Color() {}

	inline sf::Color sfColor() const 
	{ 
		return { 
			(uint8_t)(arr[0] * 255), 
			(uint8_t)(arr[1] * 255), 
			(uint8_t)(arr[2] * 255), 
			(uint8_t)(arr[3] * 255) 
		}; 
	}

	float arr[4];
};

class Node
{
public:
	static std::unordered_map<std::string, Color> colors;

	Node(uint32_t row, uint32_t column);
	~Node();

	inline uint32_t getRow() const { return m_Row; }
	inline uint32_t getColumn() const { return m_Column; }

	void Draw(sf::RenderWindow& window, uint32_t cellSize, uint32_t nodeSize, uint32_t nodeMargin) const;

	bool operator==(const Node& other) const;
	bool operator!=(const Node& other) const;

	std::string type = "empty";
	std::vector<std::reference_wrapper<Node>> neighbors;
	float g_cost = INFINITY;
	float f_cost = 0.0f;
	Node* cameFrom = nullptr;

private:
	uint32_t m_Row, m_Column;
};


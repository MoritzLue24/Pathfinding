#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Node.h"


struct NodeNotFoundException : public std::exception
{
	const char* what() const throw() override
	{
		return "Not not found!";
	}
};

class Grid
{
public:
	Grid(uint32_t rows, uint32_t columns, uint32_t cellSize = 20, uint32_t nodeSize = 10, uint32_t nodeMargin = 1);
	~Grid() {}

	Node& GetNodeByPos(uint32_t row, uint32_t column);
	Node& GetNodeByType(std::string type);
	
	inline uint32_t getRows() const { return m_Rows; }
	inline uint32_t getColumns() const { return m_Columns; }
	inline uint32_t getCellSize() const { return m_CellSize; }
	inline uint32_t getNodeMargin() const { return m_NodeMargin; }

	inline uint32_t CoordToGrid(uint32_t coordinate) const
	{
		return coordinate / (m_CellSize + m_NodeMargin);
	}
	inline uint32_t GridToCoord(uint32_t gridPos) const
	{
		return gridPos * (m_CellSize + m_NodeMargin);
	}

	void UpdateNeighbors();
	void Reset();
	void Draw(sf::RenderWindow& window);

	float lineColor[4] = {0.2f, 0.2f, 0.2f, 1};
	bool diagonals = true, drawGrid = true;
	int nodeSize;

private:
	uint32_t m_Rows, m_Columns;
	uint32_t m_CellSize, m_NodeMargin;
	std::vector<std::vector<Node>> m_Content;
};
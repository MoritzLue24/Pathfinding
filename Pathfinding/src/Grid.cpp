#include "Grid.h"
#include <cmath>
#include <iostream>


Grid::Grid(uint32_t rows, uint32_t columns, uint32_t cellSize, uint32_t nodeSize, uint32_t nodeMargin)
	: m_Rows(rows), m_Columns(columns), m_CellSize(cellSize), nodeSize(nodeSize), m_NodeMargin(nodeMargin)
{
	for (uint32_t i = 0; i < rows; i++) {
		std::vector<Node> row;
		for (uint32_t j = 0; j < columns; j++)
			row.emplace_back(i, j);
		m_Content.push_back(row);
	}
}

Node& Grid::GetNodeByPos(uint32_t row, uint32_t column)
{
	if (row < 0) row = 0;
	if (row >= m_Rows) row = m_Rows - 1;
	if (column < 0) column = 0;
	if (column >= m_Columns) column = m_Columns - 1;

	return m_Content[row][column];
}

Node& Grid::GetNodeByType(std::string type)
{
	for (auto& row : m_Content)
		for (Node& node : row)
			if (node.type == type)
				return node;

	throw NodeNotFoundException();
}

void Grid::UpdateNeighbors()
{
	for (auto& row : m_Content) {
		for (Node& node : row) {
			node.neighbors.clear();

			// Down
			if (node.getRow() < m_Rows - 1 && GetNodeByPos(node.getRow() + 1, node.getColumn()).type != "wall")
				node.neighbors.emplace_back(std::ref(GetNodeByPos(node.getRow() + 1, node.getColumn())));

			// Up
			if (node.getRow() > 0 && GetNodeByPos(node.getRow() - 1, node.getColumn()).type != "wall")
				node.neighbors.emplace_back(std::ref(GetNodeByPos(node.getRow() - 1, node.getColumn())));

			// Right
			if (node.getColumn() < m_Columns - 1 && GetNodeByPos(node.getRow(), node.getColumn() + 1).type != "wall")
				node.neighbors.emplace_back(std::ref(GetNodeByPos(node.getRow(), node.getColumn() + 1)));

			// Left
			if (node.getColumn() > 0 && GetNodeByPos(node.getRow(), node.getColumn() - 1).type != "wall")
				node.neighbors.emplace_back(std::ref(GetNodeByPos(node.getRow(), node.getColumn() - 1)));

			if (diagonals) {
				// Down-right
				if (node.getRow() < m_Rows - 1 && node.getColumn() < m_Columns - 1 && GetNodeByPos(node.getRow() + 1, node.getColumn() + 1).type != "wall")
					node.neighbors.emplace_back(std::ref(GetNodeByPos(node.getRow() + 1, node.getColumn() + 1)));

				// Down-left
				if (node.getRow() < m_Rows - 1 && node.getColumn() > 0 && GetNodeByPos(node.getRow() + 1, node.getColumn() - 1).type != "wall")
					node.neighbors.emplace_back(std::ref(GetNodeByPos(node.getRow() + 1, node.getColumn() - 1)));

				// Up-right
				if (node.getRow() > 0 && node.getColumn() < m_Columns - 1 && GetNodeByPos(node.getRow() - 1, node.getColumn() + 1).type != "wall")
					node.neighbors.emplace_back(std::ref(GetNodeByPos(node.getRow() - 1, node.getColumn() + 1)));

				// Up-left
				if (node.getRow() > 0 && node.getColumn() > 0 && GetNodeByPos(node.getRow() - 1, node.getColumn() - 1).type != "wall")
					node.neighbors.emplace_back(std::ref(GetNodeByPos(node.getRow() - 1, node.getColumn() - 1)));
			}
		}
	}
}

void Grid::Reset()
{
	m_Content.clear();

	for (uint32_t i = 0; i < m_Rows; i++) {
		std::vector<Node> row;
		for (uint32_t j = 0; j < m_Columns; j++)
			row.emplace_back(i, j);
		m_Content.push_back(row);
	}
}

void Grid::Draw(sf::RenderWindow& window)
{
	if (drawGrid)
	{
		// Get the node with the lowest & highest row & column and get the count of nodes that are not empty
		int lowestRow = m_Rows;
		int highestRow = -1;
		int lowestColumn = m_Columns;
		int highestColumn = -1;
		int nodeCount = 0;

		for (const auto& row : m_Content)
			for (const Node& node : row) {
				if (node.type != "empty") {
					nodeCount++;
					if ((int)node.getRow() < lowestRow)
						lowestRow = (int)node.getRow();
					if ((int)node.getRow() > highestRow)
						highestRow = (int)node.getRow();
					if ((int)node.getColumn() < lowestColumn)
						lowestColumn = (int)node.getColumn();
					if ((int)node.getColumn() > highestColumn)
						highestColumn = (int)node.getColumn();
				}
			}

		if (nodeCount > 1) {
			// Horizontal lines
			for (int r = lowestRow; r <= highestRow; r++) {
				uint32_t x1 = GridToCoord(lowestColumn) + m_CellSize / 2;
				uint32_t x2 = GridToCoord(highestColumn) + m_CellSize / 2;
				uint32_t y = GridToCoord(r) + m_CellSize / 2;

				sf::RectangleShape line({ (float)(x2 - x1), 1.0f });
				line.setPosition({ (float)x1, (float)y, });
				line.setFillColor({ (uint8_t)(lineColor[0] * 255), (uint8_t)(lineColor[1] * 255), (uint8_t)(lineColor[2] * 255), (uint8_t)(lineColor[3] * 255) });

				window.draw(line);
			}
			// Vertical lines
			for (int c = lowestColumn; c <= highestColumn; c++) {
				uint32_t x = GridToCoord(c) + m_CellSize / 2;
				uint32_t y1 = GridToCoord(lowestRow) + m_CellSize / 2;
				uint32_t y2 = GridToCoord(highestRow) + m_CellSize / 2;

				sf::RectangleShape line({ (float)(y2 - y1), 1.0f });
				line.setPosition({ (float)x, (float)y1, });
				line.setFillColor({ (uint8_t)(lineColor[0] * 255), (uint8_t)(lineColor[1] * 255), (uint8_t)(lineColor[2] * 255), (uint8_t)(lineColor[3] * 255) });
				line.rotate(90.0f);

				window.draw(line);
			}
		}
	}

	for (uint32_t i = 0; i < m_Rows; i++)
		for (uint32_t j = 0; j < m_Columns; j++)
			m_Content[i][j].Draw(window, m_CellSize, nodeSize, m_NodeMargin);
}

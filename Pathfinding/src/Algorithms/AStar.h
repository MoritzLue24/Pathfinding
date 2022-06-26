#pragma once
#include <queue>
#include <vector>
#include <SFML//Graphics.hpp>

#include "../Grid.h"
#include "../Node.h"


int heuristic(Node a, Node b);

struct OpenSetItem 
{
	Node* node;
	uint32_t count;

	OpenSetItem(Node* node, uint32_t count) : node(node), count(count) {}

	bool operator<(const OpenSetItem& other) const;
};

class AStar
{
public:
	AStar(Grid& grid) : m_Grid(grid), m_StartNode(nullptr), m_EndNode(nullptr) {}
	~AStar() {}

	void Run();
	void Update();
	void SetPath();
	void RenderPath(sf::RenderWindow& window);
	void Reset();

	bool cellsAsPath = false;

private:

	Grid& m_Grid;
	bool m_Running = false, m_Finished = false;
	uint32_t m_Count = 0;
	Node* m_StartNode;
	Node* m_EndNode;
	std::priority_queue<OpenSetItem> m_OpenSet;
	std::vector<Node> m_OpenSetItems;
	std::vector<sf::Vertex> m_PathLines;
};

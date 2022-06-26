#include "AStar.h"

#include <iostream>
#include <windows.h>


int heuristic(Node a, Node b) 
{
	return abs((int)(a.getRow() - b.getRow())) + abs((int)(a.getColumn() - b.getColumn()));
}

bool OpenSetItem::operator<(const OpenSetItem& other) const 
{
	if (node->f_cost == other.node->f_cost)
		return count > other.count;

	return node->f_cost > other.node->f_cost;
}

void AStar::Run()
{
	Reset();

	// Get the start and end nodes.
	try
	{
		m_StartNode = &m_Grid.GetNodeByType("start");
		m_EndNode = &m_Grid.GetNodeByType("end");
	}
	catch (NodeNotFoundException)
	{
		MessageBox(nullptr, L"Start or end node not found!", L"Error", MB_OK);
		return;
	}

	m_StartNode->g_cost = 0;
	m_StartNode->f_cost = (float)heuristic(*m_StartNode, *m_EndNode);

	// Add the start node to the open set.
	m_OpenSet.push(OpenSetItem(m_StartNode, m_Count));
	m_OpenSetItems.push_back(*m_StartNode);

	m_Running = true;
}

void AStar::Update()
{
	if (!m_Running)
		return;

	if (m_OpenSet.empty()) {
		m_Running = false;
		return;
	}

	Node* current = m_OpenSet.top().node;
	m_OpenSet.pop();
	m_OpenSetItems.erase(std::remove(m_OpenSetItems.begin(), m_OpenSetItems.end(), *current), m_OpenSetItems.end());

	if (current == m_EndNode) {
		m_Running = false;
		m_Finished = true;
		SetPath();
		return;
	}

	for (int i = 0; i < current->neighbors.size(); i++) {
		Node* neighbor = &current->neighbors[i].get();
		float neighbor_temp_g_cost = current->g_cost + 1;

		if (neighbor_temp_g_cost < neighbor->g_cost) {
			neighbor->cameFrom = current;

			neighbor->g_cost = neighbor_temp_g_cost;
			neighbor->f_cost = neighbor->g_cost + heuristic(*neighbor, *m_EndNode);

			// If the neighbor is not in the open set
			if (std::count(m_OpenSetItems.begin(), m_OpenSetItems.end(), *neighbor) == 0) {
				m_OpenSet.push(OpenSetItem(neighbor, m_Count++));
				m_OpenSetItems.push_back(*neighbor);
				neighbor->type = (neighbor->type != "end") ? "open" : "end";
			}
		}
	}

	if (current != m_StartNode)
		current->type = "closed";
}

void AStar::SetPath()
{
	if (!m_Finished)
		return;

	Node* current = m_EndNode;
	while (current != m_StartNode) {
		auto x1 = (float)(current->getColumn() * (m_Grid.getCellSize() + m_Grid.getNodeMargin()) + m_Grid.getCellSize() / 2);
		auto y1 = (float)(current->getRow() * (m_Grid.getCellSize() + m_Grid.getNodeMargin()) + m_Grid.getCellSize() / 2);

		auto x2 = (float)(current->cameFrom->getColumn() * (m_Grid.getCellSize() + m_Grid.getNodeMargin()) + m_Grid.getCellSize() / 2);
		auto y2 = (float)(current->cameFrom->getRow() * (m_Grid.getCellSize() + m_Grid.getNodeMargin()) + m_Grid.getCellSize() / 2);

		current = current->cameFrom;
		if (cellsAsPath)
			current->type = current->type != "start" ? "path" : "start";
		m_PathLines.push_back(sf::Vertex({ x1, y1 }, Node::colors["path"].sfColor()));
		m_PathLines.push_back(sf::Vertex({ x2, y2 }, Node::colors["path"].sfColor()));
	}
}

void AStar::RenderPath(sf::RenderWindow& window)
{
	if (!m_Finished)
		return;

	if (!cellsAsPath)
	{
		sf::Vertex* vertices = &m_PathLines[0];
		window.draw(vertices, m_PathLines.size(), sf::Lines);
	}
	m_StartNode->Draw(window, m_Grid.getCellSize(), m_Grid.nodeSize, m_Grid.getNodeMargin());
	m_EndNode->Draw(window, m_Grid.getCellSize(), m_Grid.nodeSize, m_Grid.getNodeMargin());
}

void AStar::Reset()
{
	m_Count = 0;

	m_StartNode = nullptr;
	m_EndNode = nullptr;

	m_OpenSet = std::priority_queue<OpenSetItem>();
	m_OpenSetItems.clear();

	m_PathLines.clear();

	m_Running = false;
	m_Finished = false;

	for (uint32_t i = 0; i < m_Grid.getRows(); i++) {
		for (uint32_t j = 0; j < m_Grid.getColumns(); j++) {
			Node& node = m_Grid.GetNodeByPos(i, j);
			node.cameFrom = nullptr;
			node.g_cost = INFINITY;
			node.f_cost = 0;

			// Reset the node type if the node is not the start, end, empty or wall node.
			if (node.type != "start" && node.type != "end" && node.type != "empty" && node.type != "wall")
				node.type = "empty";
		}
	}
}
#include "Node.h"


std::unordered_map<std::string, Color> Node::colors = {
	{"empty", Color(40, 40, 40)},
	{"wall", Color(80, 80, 80)},
	{"start", Color(30, 200, 0)},
	{"end", Color(200, 30, 0)},
	{"path", Color(0, 90, 150)},
	{"open", Color(40, 30, 30)},
	{"closed", Color(30, 25, 25)}
};

void Node::Draw(sf::RenderWindow& window, uint32_t cellSize, uint32_t nodeSize, uint32_t nodeMargin) const
{
	if (type == "empty")
		return;

	int nodeOffset = (cellSize / 2 - nodeSize / 2);
	
	sf::RectangleShape rect({ (float)nodeSize, (float)nodeSize });
	rect.setPosition({
		(float)(m_Column * (cellSize + nodeMargin) + nodeOffset),
		(float)(m_Row * (cellSize + nodeMargin) + nodeOffset)
	});
	rect.setFillColor(colors[type].sfColor());
	window.draw(rect);
}

bool Node::operator==(const Node& other) const
{
	return m_Row == other.m_Row && m_Column == other.m_Column;
}

bool Node::operator!=(const Node& other) const
{
	return !(*this == other);
}
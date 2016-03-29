#ifndef _GRAPHNODE_H
#define _GRAPHNODE_H

#include <vector>

class GraphNode
{
private:
	GraphNode* parent;
	char nodeIdentifier;
	bool visited;
	int xPos;
	int yPos;
	float finalCost;
	int g_moveCost;
	float heuristic;
	int height;

public:
	GraphNode() : parent(0), nodeIdentifier(0), visited(0), xPos(0), yPos(0), finalCost(0), g_moveCost(0), heuristic(0) { ; }
	GraphNode(int id) : parent(0), nodeIdentifier(id), visited(0), xPos(0), yPos(0), finalCost(0), g_moveCost(0), heuristic(0) { ; }
	GraphNode(int id, int xPosIn, int yPosIn) : parent(0), nodeIdentifier(id), visited(0), xPos(xPosIn), yPos(yPosIn), finalCost(0), g_moveCost(0), heuristic(0) { ; }
	GraphNode(int xPos, int yPos, int height, int nodeID) : parent(0), nodeIdentifier(nodeID), visited(0), xPos(xPos), yPos(yPos), finalCost(0), g_moveCost(0), heuristic(0), height(height) {;}
	~GraphNode();

	char getID()
	{
		return nodeIdentifier;
	}

	GraphNode* GetParent()
	{
		return parent;
	}

	void AddParent(GraphNode* newNode)
	{
		parent = newNode;
	}

	void SetVisited(bool visitFlag)
	{
		visited = visitFlag;
	}

	bool getVisited()
	{
		return visited;
	}

	void setXPos(int position)
	{
		xPos = position;
	}

	int getXPos()
	{
		return xPos;
	}

	void setHeight(int h)
	{
		height = h;
	}

	int getHeight()
	{
		return height;
	}

	void setYPos(int position)
	{
		yPos = position;
	}

	int getYPos()
	{
		return yPos;
	}

	void setGCost(int gCost)
	{
		g_moveCost = gCost;
	}

	void setHeuristic(float heur)
	{
		heuristic = heur;
	}

	int getGCost()
	{
		return g_moveCost;
	}

	float getHeuristic()
	{
		return heuristic;
	}

	void setFinalCost(float fCost)
	{
		finalCost = fCost;	
	}

	float getFinalCost() const
	{
		return finalCost;
	}
};

#endif 
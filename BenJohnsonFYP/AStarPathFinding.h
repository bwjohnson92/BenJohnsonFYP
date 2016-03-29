#ifndef _ASTARPATHFINDING_H
#define _ASTARPATHFINDING_H

#include "GraphNode.h"
#include "LinkList.h"
#include <iostream>
#include "MapInfo.h"
#include <queue>
#include <functional>
#include <vector>


int searchThroughList(int x, int y, linkedList<GraphNode>* list);
linkedList<GraphNode>* AStarPath(int startX, int startY, int endX, int endY, mapHeightData** world);
bool isGenerated(int direction, int currentX, int currentY);

struct NodeCompare
{
	bool operator()(const GraphNode* lhs, const GraphNode* rhs) const
	{
		return lhs->getFinalCost() > rhs->getFinalCost();
	}
};


#endif
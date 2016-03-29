#include "AStarPathFinding.h"

int searchThroughList(int x, int y, linkedList<GraphNode>* list)
{
	for(int i=0; i<list->listSize(); i++)
	{
		if((list->getAtPosition(i)->getXPos() == x) && (list->getAtPosition(i)->getYPos() == y))
		{
			return i;
		}
	}
	return -1;
}


bool isGenerated(int direction, int currentX, int currentY, mapHeightData** world)
{
	if(direction == 0)			//NORTH
	{
		if(world[(currentX)/32][(currentY-1)/32].mapChunk)
			if(world[(currentX)/32][(currentY-1)/32].mapChunk[(currentX)%32][(currentY-1) % 32].heightGenerated)
				return true;
	}
	else if (direction == 1)	//EAST
	{
		if(world[(currentX+1)/32][(currentY)/32].mapChunk)
			if(world[(currentX+1)/32][(currentY)/32].mapChunk[(currentX+1)%32][(currentY) % 32].heightGenerated)
				return true;
	}
	else if (direction == 2)	//SOUTH
	{
		if(world[(currentX)/32][(currentY+1)/32].mapChunk)
			if(world[(currentX)/32][(currentY+1)/32].mapChunk[(currentX)%32][(currentY+1) % 32].heightGenerated)
				return true;
	}
	else if (direction == 3)	//WEST
	{
		if(world[(currentX-1)/32][(currentY)/32].mapChunk)
			if(world[(currentX-1)/32][(currentY)/32].mapChunk[(currentX-1)%32][(currentY) % 32].heightGenerated)
				return true;
	}
	return false;
}

//TOO COMPLEX - NEEDS CUTTING DOWN
linkedList<GraphNode>* AStarPath(int startX, int startY, int endX, int endY, mapHeightData** world) //Perform A* search 
{
	int currentX = startX;
	int currentY = startY;

	linkedList<GraphNode> *openList = new linkedList<GraphNode>;
	linkedList<GraphNode> *closedList = new linkedList<GraphNode>;

	std::priority_queue<GraphNode*, std::vector<GraphNode*>, NodeCompare > oListHeap;

	int graphNodeID = 0;
	int OListNode = 0;
	bool fullPath = false;

	openList->addToHead(new GraphNode(startX, startY, world[startX/32][startY/32].mapChunk[startX%32][startY%32].avgY, graphNodeID++)); //Push starting node into open list.
	oListHeap.push(openList->getTailData());

	world[startX/32][startY/32].mapChunk[startX%32][startY%32].haveVisited = true; //Flag start as visited
	openList->getHeadData()->setFinalCost(99999.0f);

	GraphNode* currNode = oListHeap.top();
	GraphNode* bestNode = currNode;
	
	while(oListHeap.size()) //While there's still nodes to search.
	{
		world[(currentX)/32][(currentY)/32].mapChunk[(currentX)%32][(currentY)%32].haveVisited = true;

		//CHECK NORTH
		if(isGenerated(0, currentX, currentY, world))//NORTH
		{
			int gCost = std::abs(world[(currentX)/32][(currentY)/32].mapChunk[(currentX)%32][(currentY)%32].avgY - world[(currentX)/32][(currentY-1)/32].mapChunk[(currentX)%32][(currentY-1)%32].avgY);
			gCost = 10 + ((gCost * gCost) / 2);
			float hCost = sqrt(pow((float)abs(10 * (currentX - endX)), 2) + pow((float)abs(10 * (currentY-1 - endY)), 2));
			float fCost = gCost + hCost; //Calculate fCost. (g+h)
			if(world[(currentX)/32][(currentY-1)/32].mapChunk[(currentX)%32][(currentY-1)%32].haveVisited == false)
			{

				openList->addToTail(new GraphNode(currentX, currentY-1, world[(currentX)/32][(currentY-1)/32].mapChunk[(currentX)%32][(currentY-1)%32].avgY, graphNodeID++)); //Add to open list
				world[(currentX)/32][(currentY-1)/32].mapChunk[(currentX)%32][(currentY-1)%32].haveVisited = true; //Now visited

				//MAKE A FUNCTION
				openList->getTail()->data->AddParent(currNode);
				openList->getTail()->data->setFinalCost(fCost); //Cost to get here
				openList->getTail()->data->setGCost(gCost); //Cost to final node
				openList->getTail()->data->setHeuristic(hCost); 

				oListHeap.push(openList->getTailData());
				if(openList->getTail()->data->getFinalCost() < bestNode->getFinalCost())
					bestNode = openList->getTail()->data;
			}
			
		}

		//CHECK SOUTH
		if(isGenerated(2, currentX, currentY, world))//NORTH
		{
			int gCost = std::abs(world[(currentX)/32][(currentY)/32].mapChunk[(currentX)%32][(currentY)%32].avgY - world[(currentX)/32][(currentY+1)/32].mapChunk[(currentX)%32][(currentY+1)%32].avgY);
			gCost = 10 + ((gCost * gCost) / 2);
			float hCost = sqrt(pow((float)abs(10 * (currentX - endX)), 2) + pow((float)abs(10 * (currentY+1 - endY)), 2));
			float fCost = gCost + hCost; //Calculate fCost. (g+h)
			if(world[(currentX)/32][(currentY+1)/32].mapChunk[(currentX)%32][(currentY+1)%32].haveVisited == false)
			{
				openList->addToTail(new GraphNode(currentX, currentY+1, world[(currentX)/32][(currentY+1)/32].mapChunk[(currentX)%32][(currentY+1)%32].avgY, graphNodeID++));
				world[(currentX)/32][(currentY+1)/32].mapChunk[(currentX)%32][(currentY+1)%32].haveVisited = true;
				openList->getTail()->data->AddParent(currNode);
				openList->getTail()->data->setFinalCost(fCost);
				openList->getTail()->data->setGCost(gCost);
				openList->getTail()->data->setHeuristic(hCost);
				oListHeap.push(openList->getTailData());
				if(openList->getTail()->data->getFinalCost() < bestNode->getFinalCost())
					bestNode = openList->getTail()->data;
			}
			
		}

		//CHECK WEST
		if(isGenerated(3, currentX, currentY, world))//NORTH
		{
			int gCost = std::abs(world[(currentX)/32][(currentY)/32].mapChunk[(currentX)%32][(currentY)%32].avgY - world[(currentX-1)/32][(currentY)/32].mapChunk[(currentX-1)%32][(currentY)%32].avgY);
			gCost = 10 + ((gCost * gCost) / 2);
			float hCost = sqrt(pow((float)abs(10 * (currentX-1 - endX)), 2) + pow((float)abs(10 * (currentY - endY)), 2));
			float fCost = gCost + hCost; //Calculate fCost. (g+h)
			if(world[(currentX-1)/32][(currentY)/32].mapChunk[(currentX-1)%32][(currentY)%32].haveVisited == false)
			{
				openList->addToTail(new GraphNode(currentX-1, currentY, world[(currentX-1)/32][(currentY)/32].mapChunk[(currentX-1)%32][(currentY)%32].avgY, graphNodeID++));
				world[(currentX-1)/32][(currentY)/32].mapChunk[(currentX-1)%32][(currentY)%32].haveVisited = true;
				openList->getTail()->data->AddParent(currNode);
				openList->getTail()->data->setFinalCost(fCost);
				openList->getTail()->data->setGCost(gCost);
				openList->getTail()->data->setHeuristic(hCost);
				oListHeap.push(openList->getTailData());
				if(openList->getTail()->data->getFinalCost() < bestNode->getFinalCost())
					bestNode = openList->getTail()->data;
			}
			
		}

		//CHECK EAST
		if(isGenerated(1, currentX, currentY, world))//NORTH
		{
			int gCost = std::abs(world[(currentX)/32][(currentY)/32].mapChunk[(currentX)%32][(currentY)%32].avgY - world[(currentX+1)/32][(currentY)/32].mapChunk[(currentX+1)%32][(currentY)%32].avgY);
			gCost = 10 + ((gCost * gCost) / 2);
			float hCost = sqrt(pow((float)abs(10 * (currentX+1 - endX)), 2) + pow((float)abs(10 * (currentY - endY)), 2));
			float fCost = gCost + hCost; //Calculate fCost. (g+h)
			if(world[(currentX+1)/32][(currentY)/32].mapChunk[(currentX+1)%32][(currentY)%32].haveVisited == false)
			{	
				openList->addToTail(new GraphNode(currentX+1, currentY, world[(currentX+1)/32][(currentY)/32].mapChunk[(currentX+1)%32][(currentY)%32].avgY, graphNodeID++));
				world[(currentX+1)/32][(currentY)/32].mapChunk[(currentX+1)%32][(currentY)%32].haveVisited = true;
				openList->getTail()->data->AddParent(currNode);
				openList->getTail()->data->setFinalCost(fCost);
				openList->getTail()->data->setGCost(gCost);
				openList->getTail()->data->setHeuristic(hCost);
				oListHeap.push(openList->getTailData());
				if(openList->getTail()->data->getFinalCost() < bestNode->getFinalCost())
					bestNode = openList->getTail()->data;
			}
			
		}

		if((currentX == endX) && (currentY == endY)) //Made it!
		{
			fullPath = true;
			break;
		}
		currNode->SetVisited(true);
		currNode->setFinalCost(99999.0f);
		closedList->addToTail(currNode);

		if(oListHeap.size() != 0)
		{
			currentX = oListHeap.top()->getXPos();
			currentY = oListHeap.top()->getYPos();
			currNode = oListHeap.top();
			oListHeap.pop();
		}
	}

	int pathLength = 1;
	linkedList<GraphNode> * finalPath = new linkedList<GraphNode>;
	
	if(fullPath)
	{
		while((currNode->GetParent() != NULL)) //Made it! Recurse through the path we made to get the full path.
		{
			finalPath->addToHead(currNode);
			currNode = currNode->GetParent();
			pathLength++;
		}
	}
	else
	{
		while((bestNode->getXPos() != startX) && (bestNode->getYPos() != startY)) //Continue searching!
		{
			finalPath->addToHead(bestNode);
			bestNode = bestNode->GetParent();
			pathLength++;
		}
	}
	return finalPath;
}
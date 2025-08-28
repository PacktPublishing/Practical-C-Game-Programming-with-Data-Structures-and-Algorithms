#pragma once

#include "Knight.h"
#include "Maze.h"
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class AStarPathFinder
{
private:
	struct Node
	{
		Vector3 Position;		//Position of a grid cell
		int GridPos[2];			//Grid position on the grids
		float GCost, HCost;		//stores G and H values
		Node* Parent;			//The previous waypoint node
		Node(Vector3 pos, Node* parent = nullptr)
			: Position(pos)
			, GridPos{0, 0}
			, GCost(0)
			, HCost(0)
			, Parent(parent) {}

		//The function which calculates the F value
		float FCost() const
		{
			return GCost + HCost;
		}
	};

	float ManhattanDistance(Node* p1, Node* p2)
	{
		return (float)(abs(p1->GridPos[0] - p2->GridPos[0]) +
			abs(p1->GridPos[1] - p2->GridPos[1]));
	}

	vector<Vector3> ReconstructPath(Node* node)
	{
		vector<Vector3> path;
		while (node != nullptr)
		{
			path.push_back(node->Position);
			node = node->Parent;
		}
		reverse(path.begin(), path.end());
		return path;
	}

	struct NodeComparer
	{
		bool operator()(Node* a, Node* b)
		{
			return a->FCost() > b->FCost();
		}
	};
public:
	vector<Vector3> FindPath(Vector3& Start, Vector3& Target);
};





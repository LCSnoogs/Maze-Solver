/* Title: Maze.h
 * Abstract: This header holds the prototypes for the struct Point and the classes Binary Tree and Maze.
 * Name: Lyndon Curry
 * Date: 5/13/11
 */

#ifndef MAZE_H
#define MAZE_H

#include<string>
using namespace std;

struct Point
{
	int x;
	int y;
};

class BinaryTree
{
private:
	class Node
	{
	public:
		Node(Point location);
		Point location;
		Node* right;
		Node* left;
		Node* middle;
		Node* previous;
		bool leftTravelled;
		bool rightTravelled;
		bool middleTravelled;
		bool moveX;
	};

	Node* first;
	Node* latestNode;

public:
	void addNode(Point crossroad);
	Node* getLatestNode();
	void moveLatestNodeToPreviousNode();
	bool firstNode;
};

class Maze
{
private:
	int xLength;
	int yLength;
	char** map;
	bool moveRight;
	bool moveLeft;
	bool moveUp;
	bool moveDown;
	Point position;
	Point start;
	Point finish;
	BinaryTree crossroadsTree;

public:
	Maze(string filename);
	int getXLength();
	int getYLength();
	Point getPosition();
	void travelMaze();
	void findNewPath();
};

#endif
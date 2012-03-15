/* Title: Maze.cpp
 * Abstract: This defines the class Maze so it can solve mazes.
 * Name: Lyndon Curry
 * Date: 5/13/11
 */

#include<string>
#include<fstream>
#include<iostream>
using namespace std;
#include "Maze.h"

Maze::Maze(string filename)
{
	ifstream file;
	crossroadsTree.firstNode = true;

	file.open(filename);

	//Records all values needed to start.
	if (file.is_open())
	{
		file >> this->xLength;
		file >> this->yLength;
		cout << "Width: " << xLength << " Length: " << yLength << endl;

		//To move cursor to the next line which is the first line of the maze.
		file.get();
		map = new char* [xLength];
		
		for (int i = 0; i < xLength; i++)
		{
			map[i] = new char[yLength];
		}
		
		//Records the maze and prints it out on screen.
		for (int i = 0; i < yLength; i++)
		{
			for (int k = 0; k < xLength; k++)
			{
				file >> map[k][i];
				cout <<map[k][i];
			}
			file.get();
			cout << endl;
		}

		//Checks the first line to find where start of the maze is and sets it to the start variable.
		for (int i = 0; i < xLength; i++)
		{
			if (map[i][0] == '.')
			{
				start.x = i;
				start.y = 0;
				break;
			}
		}

		cout << "Start: " << start.x << ", " << start.y << endl;

		//Sets the maze solver to the start of the maze.
		position = start;

		//Checks the last line to see if the end of the maze is located there and assigns it to the finish variable.
		for (int i = 0; i < xLength; i++)
		{
			if (map[i][yLength-1] == '.')
			{
				finish.x = i;
				finish.y = yLength-1;
				break;
			}
		}

		//Checks the left and rights sides of the maze to see if the end of the maze is there and assigns it to finish.
		for (int i = 1; i < yLength; i++)
		{
			if (map[0][i] == '.')
			{
				finish.x = 0;
				finish.y = i;
				break;
			}

			if (map[xLength-1][i] == '.')
			{
				finish.x = xLength-1;
				finish.y = i;
				break;
			}
		}

		cout << "finish: " << finish.x << ", " << finish.y << endl;

		//The first direction the maze solver will be moving in is down
		this->moveDown = true;
		this->moveLeft = false;
		this->moveRight = false;
		this->moveUp = false;

		//Keeps the maze solver travelling the maze while it still has places to go.
		while (moveUp == true || moveDown == true || moveLeft == true || moveRight == true)
		{
			this->travelMaze();
		}
	} else
	{
		cout << "Unable to open file!" << endl;
	}
}

//returns the value of xLength
int Maze::getXLength()
{
	return this->xLength;
}

//Returns the value of yLength
int Maze::getYLength()
{
	return this->yLength;
}

//Returns the Point variable position.
Point Maze::getPosition()
{
	return this->position;
}

//This method is responsible for moving the maze solver through the maze and finding nodes.
void Maze::travelMaze()
{
	while (this->moveRight)
	{

		//Stops the maze solver from travelling the maze if it has reached the finishing point.
		if (position.x == finish.x && position.y == finish.y)
		{
			cout << "finish" << endl;
			moveRight = false;
			moveLeft = false;
			moveUp = false;
			moveDown = false;
			return;
		}

		//Checks to see if the maze solver can move both right and in another direction.
		if ((map[position.x][position.y+1] == '.' && map[position.x+1][position.y] == '.') && (map[position.x][position.y-1] == '.' && map[position.x+1][position.y] == '.'))
		{
			crossroadsTree.addNode(position);

			//moveX is true because the maze solver was moving on the x-axis when it ran into this node.
			crossroadsTree.getLatestNode()->moveX = true;

			//middleTravelled is true because there were three directions to move in: right, up, and down.
			crossroadsTree.getLatestNode()->middleTravelled = true;
		} else if ((map[position.x][position.y+1] == '.' && map[position.x+1][position.y] == '.') || (map[position.x][position.y-1] == '.' && map[position.x+1][position.y] == '.'))
		{
			crossroadsTree.addNode(position);

			//leftTravelled is true because there are two directions that the maze solver can move in: rigth and up or right and down.
			crossroadsTree.getLatestNode()->leftTravelled = true;
			crossroadsTree.getLatestNode()->moveX = true;
		}

		//Checks to see if the maze solver can still move to the right or if it has to move in a new direction.
		if (moveRight && map[position.x+1][position.y] == '.')
		{
			position.x++;
			cout << "Right" << endl;
		} else if (map[position.x][position.y+1] == '.' && map[position.x][position.y-1] == '.')
		{
			crossroadsTree.addNode(position);
			crossroadsTree.getLatestNode()->leftTravelled = true;
			crossroadsTree.getLatestNode()->moveX = true;
			moveRight = false;
			moveLeft = false;
			moveUp = false;

			//The maze solver is now moving in a new direction.
			moveDown = true;
			position.y++;
			cout << "Change Direction: Down" << endl;
		} else if (map[position.x][position.y+1] == '.')
		{
			moveRight = false;
			moveLeft = false;
			moveUp = false;
			moveDown = true;
			position.y++;
			cout << "Change Direction: Down" << endl;
		} else if (map[position.x][position.y-1] == '.')
		{
			moveLeft = false;
			moveRight = false;
			moveDown = false;
			moveUp = true;
			position.y--;
			cout << "Change Direction: Up" << endl;
		} else
		{
			//The maze solver cannot move right, up, or down and cannot advance any further
			moveRight = false;
			moveLeft = false;
			moveUp = false;
			moveDown = false;
		}
		cout << position.x << ", " << position.y << endl;
	}

	while (this->moveLeft)
	{
		if (position.x == finish.x && position.y == finish.y)
		{
			cout << "finish" << endl;
			moveRight = false;
			moveLeft = false;
			moveUp = false;
			moveDown = false;
			return;
		}
		if ((map[position.x][position.y+1] == '.' && map[position.x-1][position.y] == '.') && (map[position.x][position.y-1] == '.' && map[position.x-1][position.y] == '.'))
		{
			crossroadsTree.addNode(position);
			crossroadsTree.getLatestNode()->moveX = true;
			crossroadsTree.getLatestNode()->middleTravelled = true;
		} else if ((map[position.x][position.y+1] == '.' && map[position.x-1][position.y] == '.') || (map[position.x][position.y-1] == '.' && map[position.x-1][position.y] == '.'))
		{
			crossroadsTree.addNode(position);
			crossroadsTree.getLatestNode()->leftTravelled = true;
			crossroadsTree.getLatestNode()->moveX = true;
		} 

		if (moveLeft && map[position.x-1][position.y] == '.')
		{
			position.x--;
			cout << "Left" << endl;
		} else if (map[position.x][position.y+1] == '.' && map[position.x][position.y-1] == '.')
		{
			crossroadsTree.addNode(position);
			crossroadsTree.getLatestNode()->moveX = true;
			crossroadsTree.getLatestNode()->leftTravelled = true;
			moveRight = false;
			moveLeft = false;
			moveUp = false;
			moveDown = true;
			position.y++;
			cout << "Change Direction: Down" << endl;
		} else if (map[position.x][position.y+1] == '.')
		{
			moveRight = false;
			moveLeft = false;
			moveUp = false;
			moveDown = true;
			position.y++;
			cout << "Change Direction: Down" << endl;
		} else if (map[position.x][position.y-1] == '.')
		{
			moveRight = false;
			moveLeft = false;
			moveDown = false;
			moveUp = true;
			position.y--;
			cout << "Change Direction: Up" << endl;
		} else
		{
			moveRight = false;
			moveLeft = false;
			moveUp = false;
			moveDown = false;
		}
		cout << position.x << ", " << position.y << endl;
	}

	while (this->moveDown)
	{
		if (position.x == finish.x && position.y == finish.y)
		{
			cout << "finish" << endl;
			moveRight = false;
			moveLeft = false;
			moveUp = false;
			moveDown = false;
			return;
		}
		if ((map[position.x][position.y+1] == '.' && map[position.x-1][position.y] == '.') && (map[position.x][position.y+1] == '.' && map[position.x+1][position.y] == '.'))
		{
			crossroadsTree.addNode(position);

			//moveX is false because the maze solver was moving on the y-axis when it ran into this node.
			crossroadsTree.getLatestNode()->moveX = false;
			crossroadsTree.getLatestNode()->middleTravelled = true;
		} else if ((map[position.x][position.y+1] == '.' && map[position.x-1][position.y] == '.') || (map[position.x][position.y+1] == '.' && map[position.x+1][position.y] == '.'))
		{
			crossroadsTree.addNode(position);
			crossroadsTree.getLatestNode()->moveX = false;
			crossroadsTree.getLatestNode()->leftTravelled = true;
		}

		if (moveDown && map[position.x][position.y+1] == '.')
		{
			position.y++;
			cout << "Down" << endl;
		} else if (map[position.x+1][position.y] == '.' && map[position.x-1][position.y] == '.')
		{
			crossroadsTree.addNode(position);
			crossroadsTree.getLatestNode()->moveX = false;
			crossroadsTree.getLatestNode()->leftTravelled = true;
			moveRight = false;
			moveUp = false;
			moveDown = false;
			moveLeft = true;
			position.x--;
			cout << "Change Direction: Left" << endl;
		} else if (map[position.x+1][position.y] == '.')
		{
			moveLeft = false;
			moveUp = false;
			moveDown = false;
			moveRight = true;
			position.x++;
			cout << "Change Direction: Right" << endl;
		} else if (map[position.x-1][position.y] == '.')
		{
			moveRight = false;
			moveUp = false;
			moveDown = false;
			moveLeft = true;
			position.x--;
			cout << "Change Direction: Left" << endl;
		} else
		{
			moveDown = false;
			moveRight = false;
			moveLeft = false;
			moveUp = false;
		}
		cout << position.x << ", " << position.y << endl;
		
	}

	while (this->moveUp)
	{
		if (position.x == finish.x && position.y == finish.y)
		{
			cout << "finish" << endl;
			moveRight = false;
			moveLeft = false;
			moveUp = false;
			moveDown = false;
			return;
		}
		if (map[position.x][position.y-1] == '.' && map[position.x-1][position.y] == '.' && map[position.x+1][position.y] == '.')
		{
			crossroadsTree.addNode(position);
			crossroadsTree.getLatestNode()->moveX = false;
			crossroadsTree.getLatestNode()->middleTravelled = true;
			cout << "Taking Middle Path" << endl;
		} else if ((map[position.x][position.y-1] == '.' && map[position.x-1][position.y] == '.') || (map[position.x][position.y-1] == '.' && map[position.x+1][position.y] == '.'))
		{
			crossroadsTree.addNode(position);
			crossroadsTree.getLatestNode()->moveX = false;
			crossroadsTree.getLatestNode()->leftTravelled = true;
			cout << "Taking Left Path" << endl;
		}

		if (map[position.x][position.y-1] == '.')
		{
			position.y--;
			cout << "Up" << endl;
		} else if (map[position.x+1][position.y] == '.' && map[position.x-1][position.y] == '.')
		{
			crossroadsTree.addNode(position);
			crossroadsTree.getLatestNode()->moveX = false;
			crossroadsTree.getLatestNode()->leftTravelled = true;
			moveRight = false;
			moveUp = false;
			moveDown = false;
			moveLeft = true;
			position.x--;
			cout << "Change Direction: Left" << endl;
		} else if (map[position.x+1][position.y] == '.')
		{
			moveLeft = false;
			moveUp = false;
			moveDown = false;
			moveRight = true;
			position.x++;
			cout << "Change Direction: Right" << endl;
		} else if (map[position.x-1][position.y] == '.')
		{
			moveRight = false;
			moveUp = false;
			moveDown = false;
			moveLeft = true;
			position.x--;
			cout << "Change Direction: Left" << endl;
		} else
		{
			moveRight = false;
			moveLeft = false;
			moveUp = false;
			moveDown = false;
		}
		cout << position.x << ", " << position.y << endl;
	}

	//If the maze solver can not move in any direction that does not move it backwards then it has hit a dead end and must find a new path.
	if (moveRight == false && moveLeft == false && moveUp == false && moveDown == false)
	{
		findNewPath();
	}
}

//This method moves the maze solver to a node so it can explore a new path.
void Maze::findNewPath()
{
	cout << "Dead End: " << position.x << ", " << position.y << endl;
	position = crossroadsTree.getLatestNode()->location;
	cout << "Moving to Node at " << position.x << ", " << position.y << endl;

	//If the maze solver has travelled to the left and right of the latest node then the latest node is a dead end and the solver must move to a previous node.
	if (crossroadsTree.getLatestNode()->leftTravelled && crossroadsTree.getLatestNode()->rightTravelled)
	{
		crossroadsTree.moveLatestNodeToPreviousNode();

		//We must find a path for the solver to travel in the previous node.
		findNewPath();

		//Makes sure that the method findNewPath() ends when a new path is found.
		return;
	}

	//If moveX is true then the maze solver can only move up or down.
	if (crossroadsTree.getLatestNode()->moveX)
	{
		//If leftTravelled is true, that means that the maze solver has already travelled down the left path and should now take the right path.
		if (crossroadsTree.getLatestNode()->leftTravelled)
		{
			crossroadsTree.getLatestNode()->rightTravelled = true;

			//Checks to see which path is the right path.
			if (map[position.x][position.y-1] == '.')
			{
				position.y--;
				moveUp = true;
			} else if (map[position.x][position.y+1] == '.')
			{
				position.y++;
				moveDown = true;
			}
		} 
		
		//If middleTravelled is true, that means that the maze solver has not travelled down the right or left path and should not take the left path.
		else if (crossroadsTree.getLatestNode()->middleTravelled)
		{
			crossroadsTree.getLatestNode()->leftTravelled = true;

			//In this situation, the left path can only take the solver down because up would be considered the right path.
			position.y++;
			moveDown = true;
		}
	} 
	
	//If moveX is false, that means that the maze solver can go either right or left.
	else if (crossroadsTree.getLatestNode()->moveX == false)
	{
		if (crossroadsTree.getLatestNode()->leftTravelled)
		{
			crossroadsTree.getLatestNode()->rightTravelled = true;
			
			if (map[position.x+1][position.y] == '.')
			{
				position.x++;
				moveRight = true;
			} else if (map[position.x-1][position.y] == '.')
			{
				position.x--;
				moveLeft = true;
			}
		} else if (crossroadsTree.getLatestNode()->middleTravelled)
		{
			crossroadsTree.getLatestNode()->leftTravelled = true;
			position.x--;
			moveLeft = true;
		}
	}

	cout << position.x << ", " << position.y << endl;
}

//This method adds a new node to the binary tree.
void BinaryTree::addNode(Point crossroad)
{

	//This Node pointer will be used for swapping.
	Node* temp;

	//If the first node has not been added to the tree, then firstNode will be true.
	if (firstNode)
	{
		cout << "First Node: " << crossroad.x << ", " << crossroad.y << endl;
		first = new Node(crossroad);

		//The first node also becomes the latest node.
		latestNode = first;
		firstNode = false;
	} else
	{
		cout << "Add Node: " << crossroad.x << ", " << crossroad.y << endl;

		//rightTravelled would be the last variable to be set true, so we check this first so we don't add a Node where another node already existed.
		if (latestNode->rightTravelled)
		{
			//Since rightTravelled is true, the maze solver found this new node while taking the right path of the last node.
			latestNode->right = new Node(crossroad);

			//To keep the value of the last node it is assigned to temp.
			temp = latestNode;

			//The latest node is not the node that is to the right of the last node.
			latestNode = latestNode->right;

			//The node before the new node is now assigned to the previous pointer of the new node.
			latestNode->previous = temp;
		} else if (latestNode->leftTravelled)
		{
			latestNode->left = new Node(crossroad);
			temp = latestNode;
			latestNode = latestNode->left;
			latestNode->previous = temp;
		} else if (latestNode->middleTravelled)
		{
			latestNode->middle = new Node(crossroad);
			temp = latestNode;
			latestNode = latestNode->middle;
			latestNode->previous = temp;
		}
	}

	//No path has been taken on this new node yet.
	latestNode->rightTravelled = false;
	latestNode->leftTravelled = false;
	latestNode->middleTravelled = false;
}

//This method returns the latest node added to the binary tree.
BinaryTree::Node* BinaryTree::getLatestNode()
{
	return this->latestNode;
}

//This is the constructor for node and sets the node's location.
BinaryTree::Node::Node(Point location)
{
	this->location = location;
}

//This method sets the latest node to the node before it for when the solver must move back to the latest node that does not lead to a dead end.
void BinaryTree::moveLatestNodeToPreviousNode()
{
	latestNode = latestNode->previous;
}
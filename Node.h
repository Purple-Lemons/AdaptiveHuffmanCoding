/*
A class for storing all the information for each
node in a tree
*/
#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

class Node
{
public:
	Node();
	Node(char, int);
	~Node();

	void setLeft(Node*);
	void setRight(Node*);
	void setParent(Node*);
	void setFrequency(int);

	char getData();
	int getFrequency();

	Node* getLeft();
	Node* getRight();
	Node* getParent();

private:
	char data;
	int frequency;

	Node *left = NULL;
	Node *right = NULL;
	Node *parent = NULL;
};

#endif

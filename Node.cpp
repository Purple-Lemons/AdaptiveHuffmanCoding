#include "stdafx.h"
#include "Node.h"

Node::Node(char _data, int _frequency)
{
	data = _data;
	frequency = _frequency;
}

Node::Node()
{
}


Node::~Node()
{
}

void Node::setLeft(Node *node)
{
	//Sets this to be the parent of the new child node
	node->setParent(this);
	left = node;
}

void Node::setRight(Node *node)
{
	//Sets this to be the parent of the new child node
	node->setParent(this);
	right = node;
}

void Node::setParent(Node *node)
{
	parent = node;
}

void Node::setFrequency(int _frequency)
{
	frequency = _frequency;
}

char Node::getData()
{
	return data;
}

int Node::getFrequency()
{
	return frequency;
}

Node* Node::getLeft()
{
	return left;
}

Node* Node::getRight()
{
	return right;
}

Node* Node::getParent()
{
	return parent;
}

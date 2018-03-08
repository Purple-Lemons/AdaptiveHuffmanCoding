/*
This class performs all of the actions of adaptive huffman coding
*/
#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "Node.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class HuffmanTree 
{
public:
	HuffmanTree();
	~HuffmanTree();

	void addCharacter(char);
	void printTree(Node*, int);

	void generateCodes(Node*, string, vector<string>*, vector<char>*);

	string decode(vector<string>*);

	Node* getHead()
	{
		return head;
	}

private:
	Node* sameValueHigherOrder(Node*);
	Node* contains(char, Node*);//If a character exists in the tree
	Node* getDag(Node* tree);//Gets the dagger node at the end of the tree

	void updateTree(Node*);
	void updateFrequencies(Node*);
	int updateAllFrequencies(Node*);

	bool leftOrRight(Node*);
	bool isLeaf(Node*);

	char charFromCode(Node*, string);//Finds the character that corresponds to a code

	Node* head;

	bool first = true;//Temporary way of solving the first insertion problem

};

#endif

#include "HuffmanTree.h"

HuffmanTree::HuffmanTree()
{
	head = new Node();
	
	Node* dag = new Node('*', 0);
	head->setLeft(dag);
	head->setFrequency(0);
}

HuffmanTree::~HuffmanTree()
{
}

void HuffmanTree::addCharacter(char c)
{
	//If this is the first character to be added, it can just be place in the tree after the head
	if (first)
	{
		first = false;
		
		head->setRight(new Node(c, 1));
		head->setFrequency(1);
	}
	else if (contains(c, head) == NULL)
	{
		Node *newNode = new Node();//New Frequency node
		newNode->setFrequency(1);

		//The node of the new character
		Node *cNode = new Node(c, 1);

		//The dagger node
		Node *dag = getDag(head);

		//Set dags parent to be newNodes parent
		if (leftOrRight(dag))
		{
			dag->getParent()->setLeft(newNode);
		}
		else
		{
			dag->getParent()->setRight(newNode);
		}

		newNode->setLeft(cNode);
		newNode->setRight(dag);

		updateTree(newNode);
	}
	else
	{
		//If the tree already contains the character, the frequencies just need to be incremeneted
		Node* node = contains(c, head);
		node->setFrequency(node->getFrequency() + 1);

		updateTree(node);
	}

	
}

void HuffmanTree::updateTree(Node* newNode)
{
	updateFrequencies(newNode);

	while (newNode != head && newNode != nullptr)
	{
		newNode = sameValueHigherOrder(newNode);
		//updateAllFrequencies(head);

		newNode = newNode->getParent();
	}
}

//Checks if the current node is on the left or the right of it's parent
//returns true for left, false for right
bool HuffmanTree::leftOrRight(Node* node)
{
	if (node != head && node->getParent()->getLeft() == node)
	{
		return true;
	}
	
	return false;
}

//Checks if the there is a node in the tree with the same or a lower value in
//the tree with a higher order than the current node
Node* HuffmanTree::sameValueHigherOrder(Node* node)
{
	Node* parent = node->getParent();
	Node* grandParent = parent->getParent();

	//If the parent is not the head of the tree
	if (grandParent != NULL)
	{
		if (leftOrRight(parent))
		{
			if (node->getFrequency() > grandParent->getLeft()->getFrequency())
			{
				//Swaps the current node and the node with a higher order
				Node* temp = grandParent->getLeft();

				if (leftOrRight(node))
				{
					parent->setLeft(temp);
				}
				else
				{
					parent->setRight(temp);
				}

				grandParent->setLeft(node);

				return temp;
			}
		}
		else
		{
			if (node->getFrequency() > grandParent->getRight()->getFrequency())
			{
				Node* temp = grandParent->getRight();

				if (leftOrRight(node))
				{
					parent->setLeft(temp);
				}
				else
				{
					parent->setRight(temp);
				}

				grandParent->setRight(node);

				return temp;
			}
		}
	}

	return node;
}

//A depth first search that checks if a character is already in the tree
Node* HuffmanTree::contains(char c, Node* tree)
{
	if(isLeaf(tree))
	{
		//if the character has been found
		if (tree->getData() == c)
		{
			return tree;
		}

		return NULL;
	}
	else
	{
		Node* left = NULL;
		Node* right = NULL;
		
		left = contains(c, tree->getLeft());
		right = contains(c, tree->getRight());

		if (left != NULL)
		{
			return left;
		}
		else
		{
			return right;
		}

	}
}

//Incremenets a branch of the tree, when a new node is added
void HuffmanTree::updateFrequencies(Node* node)
{
	while (node != head)
	{
		node = node->getParent();

		node->setFrequency(node->getFrequency() + 1);
	}
}

int HuffmanTree::updateAllFrequencies(Node* head)
{
	if (isLeaf(head))
	{
		return head->getFrequency();
	}
	else
	{
		int lF = updateAllFrequencies(head->getLeft());
		int lR = updateAllFrequencies(head->getRight());

		head->setFrequency(lF + lR);
		return head->getFrequency();
	}
}

//Checks if a node is a leaf
bool HuffmanTree::isLeaf(Node* node)
{
	if (node == nullptr)
	{
		return false;
	}
	else if (node->getLeft() != NULL || node->getRight() != NULL)
	{
		return false;
	}

	return true;
}

//Find the dagger in the tree
Node* HuffmanTree::getDag(Node* tree)
{
	if (tree->getFrequency() == 0)
	{
		return tree;
	}
	else if (isLeaf(tree))
	{
		return NULL;
	}
	else
	{
		Node* left = getDag(tree->getLeft());
		Node* right = getDag(tree->getRight());

		if (left != NULL)
		{
			return left;
		}
		else
		{
			return right;
		}
	}
}

//Prints the level and frequency of each node 
void HuffmanTree::printTree(Node* tree, int level)
{
	if (isLeaf(tree))
	{
		cout << tree->getData() << " : " << tree->getFrequency() << " -- Level: " << level << endl;
	}
	else
	{
		cout << tree->getFrequency() << " -- Level: " << level << endl;
		level++;
		printTree(tree->getLeft(), level);
		printTree(tree->getRight(), level);
	}
}

//Create the codes for each leaf node in the tree
void HuffmanTree::generateCodes(Node* head, string code, vector<string>* codes, vector<char>* chars)
{
	if (isLeaf(head))
	{
		//Adds the code to the code array and the corresponding character to a corresponding array
		codes->push_back(code);
		chars->push_back(head->getData());
	}
	else
	{
		//Builds up the code, by adding a 0 if the node is on the left and a 1 if it's on the right
		generateCodes(head->getLeft(), code + "0", codes, chars);
		generateCodes(head->getRight(), code + "1", codes, chars);
	}
}

//Finds the characters that correspond to a string of codes
string HuffmanTree::decode(vector<string>* compressedMessage)
{
	string decodedMessage = "";

	for (int i = 0; i < compressedMessage->size(); i++)
	{
		decodedMessage += charFromCode(head, (*compressedMessage)[i]);
	}

	return decodedMessage;
}

//Find the character that coresponds to a code
char HuffmanTree::charFromCode(Node* tree, string code)
{
	//If the code is empty, the correct node must have been found
	if (code == "")
	{
		return tree->getData();
	}
	else
	{
		if (code[0] == '1')
		{
			//Removes characters from the code as we move through the tree, so that
			//when it's empty we have found the correct node
			code.erase(0, 1);
			return charFromCode(tree->getRight(), code);
		}
		else
		{
			code.erase(0, 1);
			return charFromCode(tree->getLeft(), code);
		}
	}
}

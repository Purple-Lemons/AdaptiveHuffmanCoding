#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>

#include "HuffmanTree.h"

using namespace std;

//Takes a string of 8 bits and returns the charcter value of those bits
char bitsToChar(string bitString)
{
	bitset<8> bits(bitString);
	unsigned char c = (bits.to_ulong() & 0xFF);
	return c;
}

//Takes a character and finds the bits that make up that byte
string charToBits(char byte)
{
	bitset<8> bits(byte);

	string bitString = "";

	for (int i = 7; i >= 0; i--)
	{
		if (bits[i] == 1)
		{
			bitString += "1";
		}
		else
		{
			bitString += "0";
		}
	}


	return bitString;
}

//Reads a a file as a string
string readFromFile(string path)
{
	string content = "";
	string c;

	ifstream file;
	file.open(path);

	if (file.is_open())
	{
		while (file >> c)
		{
			content += c;
		}

		file.close();
	}
	else
	{
		cout << "The file has not been found!" << endl;
		cout << "Make sure that 'uncompressed_m.txt' is in the same directory as the program" << endl;
	}

	return content;
}

//Writes a string to a file
void writeToFile(string content, string path)
{
	ofstream file(path);

	if (file.is_open())
	{
		file << content;
		file.close();
	}
}

//Creates a bit string to represent the compressed file, then converts the bit string into
//a corresponding set of bytes
string compressMessage(vector<string>* codes, vector<char>* codeChars, string message, int* paddingSize)
{
	//Bit string that represents the compressed message
	string compressedMessage = "";

	//Assigns a code to each character in the message
	for (int i = 0; i < message.size(); i++)
	{
		for (int j = 0; j < codeChars->size(); j++)
		{
			if (message[i] == (*codeChars)[j])
			{
				compressedMessage += (*codes)[j];
			}
		}
	}

	cout << "Bit String Of Message: " << compressedMessage << endl;

	//Convert string of bits to bits
	string messageToWrite = "";
	*paddingSize = 0;//The number of bits that are added to make up a full byte
	for (int i = 0; i < compressedMessage.size(); i += 8)
	{
		string character = compressedMessage.substr(i, i + 8);
		messageToWrite += bitsToChar(character);

		if (i + 8 > compressedMessage.size())
		{
			*paddingSize = i + 8 - compressedMessage.size();
		}
	}

	return messageToWrite;
}

//Takes a string of characters and converts them into a bit string of codes
string decompressMessage(string readMessage, int paddingSize)
{
	string compressedMessage = "";

	for (int i = 0; i < readMessage.size(); i++)
	{
		compressedMessage += charToBits(readMessage[i]);

		//Removes padding bits
		if (i == readMessage.size() - 1 && paddingSize != 0)
		{
			compressedMessage.erase(compressedMessage.size() - 8, paddingSize);
		}
	}

	return compressedMessage;
}

void buildTree(string message, HuffmanTree huffman)
{
	for (int i = 0; i < message.size(); i++)
	{
		huffman.addCharacter(message[i]);
	}
}

vector<string>* extractCodes(string compressedMessage, vector<string>* codes)
{
	vector<string> *messageList = new vector<string>();

	while (compressedMessage != "")
	{
		bool codeFound = false;
		int i = 0;

		string currentCode = "";
		//Finds a valid code in the current message
		while (!codeFound)
		{
			currentCode += compressedMessage[i];

			//Checks if the current string matches any valid codes
			for (int j = 0; j < codes->size(); j++)
			{
				if ((*codes)[j] == currentCode)
				{
					messageList->push_back(currentCode);
					compressedMessage.erase(0, i + 1);//Removes the found code from the message

					codeFound = true;

					i = 0;
					break;
				}
			}

			i++;
		}
	}

	return messageList;
}

int main()
{
	//inputs
	string message = readFromFile("uncompressed_m.txt");
	std::cout << "Message to be compressed: " << message << endl;

	//------------------------Compress Message----------------------//
	HuffmanTree huffman;

	//Build huffman tree
	buildTree(message, huffman);

	//Get codes that correspond to each character
	vector<string> *codes = new vector<string>();
	vector<char> *codeChars = new vector<char>();//The characters that correpond to each code

	huffman.generateCodes(huffman.getHead(), "", codes, codeChars);

	//Create a compressed message using the codes
	int paddingSize = 0;
	string compressedMessage = compressMessage(codes, codeChars, message, &paddingSize);
	writeToFile(compressedMessage, "compressed_m.txt");
	std::cout << "Compressed Message: " << compressedMessage << endl;

	//-------------------------Decode Compressed Message--------------------------------//
	//Convert from bits to string
	string readMessage = readFromFile("compressed_m.txt");
	compressedMessage = decompressMessage(readMessage, paddingSize);

	vector<string>* messageList = extractCodes(compressedMessage, codes);

	string decodedMessage = huffman.decode(messageList);
	std::cout << "Decompressed Message: " << decodedMessage << endl;

	std::cin.ignore();

    return 0;
}

/*
 * CS585
 *
 * Team Bammm
 * 	Alvaro Home
 * 	Matt Konstantinou
 * 	Michael Abramo
 *	Matt Witkowski
 *  Bradley Crusco
 * Description:
 * JSONParser cpp file.
 */

#include "JSONParser.h"
#include "JSONPrimitive.h"
#include "../Resources/DynamicArray.h"
#include "JSON.h"
#include "JSONArray.h"
#include <stdlib.h>

using namespace std;

namespace bammm
{

	JSONParser::JSONParser()
	{
	}

	void JSONParser::addRoot(JSON & newNode)
	{
		_rootMap.add(newNode.getName(), &newNode);
	}

	void JSONParser::addChild(JSON & rootNode, JSON & newNode)
	{
		rootNode.addChild(newNode);
	}

	HashMap<JSON*>* JSONParser::getMap()
	{
		return &_rootMap;
	}

	JSON* JSONParser::getRootNode(string key)
	{
		return _rootMap.getValue(key);
	}

	void JSONParser::printAllRoots()
	{
		DynamicArray<string>* keys = _rootMap.getAllKeys();
		for (unsigned int i = 0; i < keys->getSize(); i++)
		{
			cout << keys->get(i) << endl;
		}
	}

	bool JSONParser::parseFile(string filename)
	{

		ifstream input;
		char current;
		bool isKey = false;
		bool isValue = false;
		bool isDouble = false;
		JSON *currentNode = NULL;
		JSON *parentNode = NULL;
		JSON *arrayRootNode = NULL;
		bool skipGet = false;
		bool isArray = false;
		bool isNewArrayObject = false;
		string name = "";
		string value = "";

		input.open(filename.c_str());

		if (!input.is_open())
		{
			cout << "Failed to open file: " << filename << " does not exist."
					<< "\n";
			return false;
		}

		//cout << "File found and opened. Beginning parse. \n";
		//cout.flush();

		while (!input.eof())
		{
			if (!skipGet)
			{
				current = (char) input.get();
			}
			skipGet = false;

			switch (current)
			{

				case '{':
					//cout << "\n\nParsing { \n";
					//cout.flush();

					if (currentNode == NULL)
					{
						currentNode = new JSON("root");
						addRoot(*currentNode);
					}

					if (isArray)
					{
						if (!isNewArrayObject)
						{
							isNewArrayObject = true;
						}
						parentNode = arrayRootNode;		//parent should be dwarves array
						currentNode = new JSON();		//Children will be attributes
						currentNode->setParent(*parentNode);

					}

					isValue = false;
					isKey = true;

					break;

				case '}':
					if (parentNode == NULL)
					{
						/* This will be the final output of the parser. This should not output before completion. */
						//cout << "Completed parsing " << filename << ". \n";
						input.close();

						currentNode = NULL;
					}
					else if (isArray)
					{
						arrayRootNode->addChild(*currentNode->getParent());
						parentNode = arrayRootNode;
						currentNode = arrayRootNode;
					}
					else
					{
						currentNode = currentNode->getParent();
						parentNode = parentNode->getParent();

						isKey = true;
					}

					break;

				case '[':
					isArray = true;
					if (currentNode == NULL)
					{
						currentNode = new JSONArray(name);
						addRoot(*currentNode);
					}
					else
					{
						parentNode = currentNode->getParent();
						currentNode = new JSONArray(name);
						parentNode->addChild(*currentNode);
					}
					break;

				case ']':
					currentNode = arrayRootNode;
					parentNode = parentNode->getParent();
					arrayRootNode = parentNode;

					isKey = true;
					isArray = false;

					break;

				case '"':
					current = (char) input.get();
					name = "";
					value = "";
					while (!input.eof() && current != '"')
					{
						if (isValue == false && isKey == true)
						{
							name += current;
						}
						else if (isValue == true && isKey == false)
						{
							value += current;
						}
						current = (char) input.get();
					}

					break;

				case ':':

					isKey = false;
					isValue = true;

					value = "";

					/* check  what comes after colon so we may determine type */
					current = (char) input.peek();
					while (!input.eof() && current == ' ')
					{
						current = (char) input.get();
					}

					/* Now that we know the value, we may confirm the type of the JSON Node and create it. */

					parentNode = currentNode;

					if (current == '{')
					{
						if (isArray)
						{
							currentNode->addChild(*(new JSON(name)));
						}
						else
						{
							currentNode = new JSON(name);
						}
						isKey = true;
						isValue = false;
					}
					else if (current == '[')
					{
						if (isArray)
						{
							currentNode->addChild(*(new JSONArray(name)));
						}
						else
						{
							currentNode = new JSONArray(name);
							arrayRootNode = currentNode;
						}

						isArray = true;
						currentNode->setParent(*parentNode);
						parentNode->addChild(*currentNode);

						isKey = true;
						isValue = false;

					}
					else if (current == '"')
					{
						current = (char) input.get();
						current = (char) input.get();
						while (!input.eof() && current != '"')
						{
							value += current;
							current = (char) input.get();
						}

						if (isNewArrayObject)
						{
							currentNode->setName(value);
							isNewArrayObject = false;
						}

						//cout << "\nValue:  " << value << "\n";

						if (isArray)
						{
							currentNode->addChild(*(new JSONPrimitive(name, value, JSON_STRING)));

						}
						else
						{
							currentNode = new JSONPrimitive(name, value,
									JSON_STRING);
						}

					}
					else if (current == 'f' || current == 't')
					{

						while (!input.eof() && current != ' ')
						{
							current = (char) input.get();
							value += current;
						}

						bool boolVal;

						if (value == "false ")
						{
							boolVal = false;
						}
						else if (value == "true ")
						{
							boolVal = true;
						}
						else
						{
							cout << "Error parsing string or bool value \n";
							cout.flush();
							return false;
						}


						if (isArray)
						{
							currentNode->addChild(*(new JSONPrimitive(name, boolVal, JSON_BOOL)));
						}
						else
						{
							currentNode = new JSONPrimitive(name, boolVal, JSON_BOOL);
						}
g
					}
					else if (isdigit(current))
					{
						while (!input.eof() && current != ','&& current != '}')
						{
							current = (char) input.get();
							value += current;
						}

						/* Determine whether value has a decimal. If so, make DOUBLE. Else INT */

						for (unsigned int i; i < value.size(); i++)
						{
							if (value[i] == '.')
							{
								isDouble = true;
								break;
							}
							isDouble = false;
						}

						if (isDouble == false)
						{
							currentNode = new JSONPrimitive(name,
									atoi(value.c_str()), JSON_INT);
						}
						else
						{
							currentNode = new JSONPrimitive(name,
									atof(value.c_str()), JSON_DOUBLE);
						}

						skipGet = true;
						isValue = false;
						isKey = true;
					}
					else
					{
						cout  << "Error parsing value. Invalid character found. \n";
						return false;
					}

					currentNode->setParent(*parentNode);

					if (parentNode == NULL)
					{
						addRoot(*currentNode);
					}
					else
					{
						parentNode->addChild(*currentNode);
					}

					//cout << "\tName: " << name << " Value: " << value
					//		<< " Parent: " << currentNode->getParent()->getName() << endl;
					break;

				case ',':
					/* reset node to parent so next key/value may be added as a child to parent */
					currentNode = currentNode->getParent();
					parentNode = currentNode->getParent();

					isValue = false;
					isKey = true;

					break;

			}
		}

		//cout << "End of file reached \n";
		//cout.flush();

		currentNode = NULL;
		parentNode = NULL;
		delete currentNode;
		delete parentNode;

		return true;
	}
}

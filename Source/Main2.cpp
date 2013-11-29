#include <iostream>
#include "SceneManager/SceneManager.h"
#include "Controllers/PlayerController.h"
#include <random>
#include <exception>

using namespace bammm;
using namespace std;

void printWelcome();
void printOptions();
DynamicArray<string>* parseInput(string);

int main()
{

	printWelcome();
	bool printMap = true;

	/*
	Factory* actorFactory = new Factory();
	actorFactory->setup();
	SceneManager sceneManager;

	MeleeCombat meleeCombat;
	sceneManager.setMeleeCombat(meleeCombat);

	////Move to SceneManager?
	PlayerController controller;
	//controller.setup(*bob, meleeCombat, sceneManager.getSceneGraph());
	*/

	bool playGame = true;
	bool doTick = true;
	string command;
	unsigned int loopCounter = 0;

	DynamicArray<string>* input = new DynamicArray<string>();

	//Should this be a 2D array?
	DynamicArray<string>* controllerInput = new DynamicArray<string>();

	//string sleep = "sleep";
	string mine = "mine";
	string drink = "drink";
	string sing = "sing";
	string brawl = "brawl";
	string attack = "combat";
	string wait = "wait";

	float dTime = 0;
	while (playGame)
	{
		doTick = true;
		if (loopCounter == 0)
		{
			input->clear();

			if (printMap)
			{
				//cout << sceneManager.getSceneGraph().toString() << "\n";
			}

			//controller.printOptions();
			getline(cin, command);

			input = parseInput(command);
			command = input->get(0);

			if (command == wait)
			{
				//Wait [#]

				if (input->getSize() == 2)
				{
					string number_str = input->get(1);
					loopCounter = atoi(number_str.c_str());
				}
				else
				{
					cout << "Invalid input\n";
					doTick = false;
				}

				if (loopCounter == 0)
				{
					cout << "Invalid input\n";
					doTick = false;
				}

			}

			else if (command == "exit")
			{
				playGame = false;
			}


			if (!playGame)
			{
				break;
			}

			if (doTick)
			{
				//controller.input(input, dTime);
				//sceneManager.tick(0);
			}
		}
		else	//looping
		{
			cout << "1 GAME TICK" << endl;
			loopCounter--;
			//sceneManager.tick(0);
		}

	}
	delete input;

	cout << "Thanks for playing!  Press enter to quit." << "\n";
	return 0;
}

void printWelcome()
{
	cout << "================================================" << "\n";
	cout << "Welcome to BAMMM -  Alpha v0.2" << "\n";
	cout << "Creators: \tAlvaro Home - Matthew Konstantinou - Matthew Witkowski\n\t\tBradley Crusco - Michael Abramo"
			<< "\n";
	cout << "================================================" << "\n";
}

bool createActor()
{

	return false;
}

DynamicArray<string>* parseInput(string input)
{
	char delimiter = ' ';
	char current;

	string builder = "";
	DynamicArray<string>* result = new DynamicArray<string>();

	for (int i = 0; i < input.size(); i++)
	{
		current = input[i];

		if ( i == input.size()-1)
		{
			builder += current;
			result->add(builder);
			builder = "";
		}

		if (current == delimiter)
		{
			result->add(builder);
			builder = "";
		}
		else
		{
			builder += current;
		}
	}
	return result;
}
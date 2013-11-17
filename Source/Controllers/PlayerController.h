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
 * PlayerController header file.
 *
 */

#ifndef PLAYERCONTROLLER_H_
#define PLAYERCONTROLLER_H_

#include "Controller.h"
#include "../States/StateMachine.h"
#include "../SceneManager/SceneManager.h"
#include "../Weapons/MeleeCombat.h"
#include "../Weapons/RangedCombat.h"
#include "../Factories/Factory.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

using namespace std;

namespace bammm
{
	class PlayerController: public Controller
	{
		private:
			MeleeCombat* _meleeCombat;
			RangedCombat* _rangedCombat;
			SceneManager* _sceneManager;

		public:
			PlayerController(SceneManager* scene, MeleeCombat* meleeCombat);
			~PlayerController();

			/**
			 setup
			 @Pre-Condition- Takes an actor as input
			 @Post-Condition- Actor passed as argument is setup for use
			 */
			void setup(Actor* actor);

			/**
			 input
			 @Pre-Condition- Takes a single command and a detlaTime
			 @Post-Condition- The command and deltaTime are passed to the overloaded input
			 */
			void input(string command, float deltaTime);

			/**
			 input
			 @Pre-Condition- Takes a single command and a detlaTime
			 @Post-Condition- The input is executed
			 */
			void input(DynamicArray<string>* command, float deltaTime);

			/**
			 printOptions
			 @Pre-Condition- Takes no arguments
			 @Post-Condition- Prints out control options to the screen
			 */
			void printOptions();
	};

	PlayerController::PlayerController(SceneManager* scene,
			MeleeCombat* meleeCombat)
	{
		_sceneManager = scene;
		_meleeCombat = meleeCombat;
	}

	PlayerController::~PlayerController()
	{
		DynamicArray<State*>* temp = _states->getAllValues();
		for (int i = 0; i < (int) temp->getSize(); i++)
		{
			delete temp->get(i);
		}

		delete _states;
		delete _stateMachine;
	}

	void PlayerController::setup(Actor* actor)
	{
		Factory* actorFactory = new Factory();
		actorFactory->setup();

		_actor = actor;
		_states = new HashMap<State*>();
		_stateMachine = new StateMachine(_actor, _states);

		//DO NOT DELETE THE REFERENCES TO STATEMACHINE.  THE CODE WILL SEG FAULT IF YOU DO
		DrinkState* drinkState = new DrinkState(_actor, _stateMachine);
		MineState* mineState = new MineState(_actor, _stateMachine);
		SingState* singState = new SingState(_actor, _stateMachine);
		BrawlState* brawlState = new BrawlState(_actor, _stateMachine);
		SleepState* sleepState = new SleepState(_actor, _stateMachine);
		IdleState* idleState = new IdleState(_actor, _stateMachine);
		CombatState* combatState = new CombatState(_actor, _stateMachine);

		_stateMachine->initialState(idleState);

		_states->add(idleState->to_string(), idleState);
		_states->add(mineState->to_string(), mineState);
		_states->add(drinkState->to_string(), drinkState);
		_states->add(singState->to_string(), singState);
		_states->add(brawlState->to_string(), brawlState);
		_states->add(sleepState->to_string(), sleepState);
		_states->add(combatState->to_string(), combatState);
	}

	void PlayerController::input(string command, float deltaTime)
	{
		DynamicArray<string>* passValue = new DynamicArray<string>();
		passValue->add(command);
		input(passValue, deltaTime);
		delete passValue;
	}

	void PlayerController::input(DynamicArray<string>* multiInput,
			float deltaTime)
	{
		DynamicArray<State*>* currentStates = _stateMachine->getCurrentStates();

		//newStates will have 1 state
		//DynamicArray<State*>* newStates = new DynamicArray<State*>();

		for (int i = 0; i < (int) multiInput->getSize(); i++)
		{
			//Check to see if state is already running
			//If so, break it down

			State* newState = _states->getValue(multiInput->get(i));

			/*
			 This should be handled in the stateMachine
			 This logic should be in statemachine->add()
			 or switchState()
			 */
			if (currentStates->contains(newState))
			{
				//Switching newState with NULL calls breakdown on newState, the remove on currentStates

				//Do doTurn in MeleeCombat
				if (newState->to_string() == "combat")
				{
					if (!_meleeCombat->getFightHappening())
					{
						_stateMachine->removeState(newState);

					}
					else
					{
						_meleeCombat->useTurn();
					}
				}
				else
				{
					//breakdown and setup are not calling the correct functions
					_stateMachine->removeState(newState);
				}
			}
			else
			{
				//Special case for combat state
				if (newState->to_string() == "combat")
				{
					Actor* closestEnemy =
							_sceneManager->getSceneGraph()->getEnemy(
									_actor->getLocation(), _actor);
					if (closestEnemy)
					{
						_meleeCombat->setup(_actor, closestEnemy);
					}
				}
				_stateMachine->addState(newState);
			}
		}

		//Should input even call switchstate?
		//_stateMachine->switchState(newStates);
		_stateMachine->tick(deltaTime);
	}

	void PlayerController::printOptions()
	{
		DynamicArray<State*>* currentStates = _stateMachine->getCurrentStates();

		//TODO: Make this smarter.
		cout << "Select an activity for your dwarf:" << endl;

		//Mining gold options
		if (currentStates->contains(_states->getValue("mine")))
		{
			cout << "1. Stop mining gold" << endl;
		}
		else
		{
			cout << "1. Mine Gold" << endl;
		}

		//Drinking options
		if (currentStates->contains(_states->getValue("drink")))
		{
			cout << "2. Stop drinking ale" << endl;
		}
		else
		{
			cout << "2. Drink ale" << endl;
		}

		//Singing options
		if (currentStates->contains(_states->getValue("sing")))
		{
			cout << "3. Stop signing" << endl;
		}
		else
		{
			cout << "3. Sing a song" << endl;
		}

		//Fighting options
		if (currentStates->contains(_states->getValue("brawl")))
		{
			cout << "4. Stop fighting" << endl;
		}
		else
		{
			cout << "4. Fight a dwarf" << endl;
		}

		//Sleeping options
		if (currentStates->contains(_states->getValue("sleep")))
		{
			cout << "5. Wake up" << endl;
		}
		else
		{
			cout << "5. Go to sleep" << endl;
		}

		//Combat options
		if (currentStates->contains(_states->getValue("combat")))
		{
			cout << "6. Attack" << endl;
		}
		else
		{
			cout << "6. Fight Orc" << endl;
		}

		cout << "7. Continue" << endl;

		cout << "0. Quit" << endl;
	}
}

#endif

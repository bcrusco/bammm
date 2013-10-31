#ifndef MINESTATE_H_
#define MINESTATE_H_

#include "state.h"
#include <iostream>

#ifndef UINT
#define UINT
typedef unsigned int UINT;
#endif

namespace bammm
{
    class MineState : public State
    {
		private:
			int successChance;
			int maxGold;
        public:
            MineState(Actor* actor);
            void setup();
            void breakDown();
            void tick(float dTime);
    };

	MineState::MineState(Actor* actor)
	{
		_actor = actor;
	}

    /*
     * setup
     * Pre-Condition- no parameters
     * Post-Condition- sets _actor's member variables necessary for beginning a state
     */
    void MineState::setup()
    {
    	successChance = 30;
    	maxGold = 100;
    }

    void MineState::breakDown()
    {
    }

    void MineState::tick(float dTime)
    {
        //int random = rand() % 100 + 1;
    	_actor->reduceStamina(1);
    	_actor->addGold(1);
        cout << _actor->getName() << " lifts his pickaxe, and swings it at the rock. " << endl;
        
        if (_actor->getGold() > maxGold)
        {
        	cout << _actor->getName() << "'s purse is full!" << endl;
        	onTransition("drink");
        }

        /*
        if(random <= successChance)
        {
            cout << "A chunk of runite falls from the rock.\n";
        }
        else
        {
            cout << "Nothing useful chips off from the rock.";
        }
        */
    }

}
#endif

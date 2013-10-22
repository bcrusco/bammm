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

        public:
            MineState()
            {
            	successChance = 30;
            }
            void setup(Actor* actor);
            void breakDown();
            void tick(float dTime);
    };

    /*
     * setup
     * Pre-Condition- no parameters
     * Post-Condition- sets _actor's member variables necessary for beginning a state
     */
    void MineState::setup(Actor* actor)
    {
    	_actor = actor;
    }

    void MineState::breakDown()
    {
    }

    void MineState::tick(float dTime)
    {
        //int random = rand() % 100 + 1;
    	_actor->reduceStamina(1);
        cout << "The dwarf lifts his pickaxe, and swings it at the rock. ";
        
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

    void registerTransitionCallback(IStateCallback callback)
    {
    	callback.onTransition();
    }
}
#endif

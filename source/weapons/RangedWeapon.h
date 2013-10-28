/*
 * RangedWeapon.h
 *
 *  Created on: Oct 27, 2013
 *      Author: michael
 */

#ifndef RANGEDWEAPON_H_
#define RANGEDWEAPON_H_

#include "IWeaponType.h"
//#include "../actors/actor.h"

namespace bammm{

	class Actor;

	class RangedWeapon
	{
		private:

		WeaponData* weaponData;

		public:

		RangedWeapon();
		~RangedWeapon();
		virtual void attack(Actor*);
		virtual bool canAttack();
		virtual int getRange();

	};




}




#endif /* RANGEDWEAPON_H_ */

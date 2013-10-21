/*
 * CS585
 *
 * Team MMA
 *      Alvaro Home
 *      Matt Konstantino
 *      Michael Abramo
 *
 * Description:
 *      my_grid3d Header File
 *
 * Last Modified: Alvaro Home
 *
 */

#ifndef GRID3D_H_
#define GRID3D_H_

#include <iostream>
#include "dynamicarray.h"
using namespace std;

#ifndef NULL
#define NULL (void *)0
#endif

namespace bammm
{

	template<typename T> class Grid3d
	{
		private:
			DynamicArray<T> *grid;
			int width;
			int length;
			int height;

		public:
			Grid3d<T>();
			Grid3d<T>(int w, int l, int h);
			~Grid3d<T>();
			/*
			 access
			 @Pre-Condition- takes in x,y,z  point system
			 @Post-Condition- will return the value in that point system
			 */
			T* access(int x, int y, int z);
			/*
			 insert
			 @Pre-Condition- Takes in xyz coordinate and and object to insert
			 @Post-Condition- Inserts object into the grid space
			 */
			void insert(int x, int y, int z, T &obj);
			/*
			 remove
			 @Pre-Condition- Takes in xyz coordinate
			 @Post-Condition- Removes object specified by the coordinates
			 */
			void remove(int x, int y, int z);

	};


	//Creates an grid
	template<class T>
	Grid3d<T>::Grid3d<T>()
	{
		width = 0;
		length = 0;
		height = 0;
		grid = new DynamicArray<T>();
	}
	//creates a Grid with  size
	template<class T>
	Grid3d<T>::Grid3d<T>(int w, int l, int h)
	{
		width = w;
		length = l;
		height = h;
		grid = new DynamicArray<T>(width * length * height);
	}

	//destructor
	template<class T>
	Grid3d<T>::~Grid3d<T>()
	{
		delete grid;
	}


	template<class T>
	T* Grid3d<T>::access(int x, int y, int z)
	{
		return grid->at(x + (y * width) + (z * width * height));
	}


	template<class T>
	void Grid3d<T>::insert(int x, int y, int z, T &obj)
	{
		int pos = x + (y * width) + (z * width * height);
		grid->insert(pos, obj);
	}


	template<class T>
	void Grid3d<T>::remove(int x, int y, int z)
	{
		int pos = x + (y * width) + (z * width * height);
		grid->erase(pos);
	}
}

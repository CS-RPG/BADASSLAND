//World.hpp
#ifndef _World_hpp_included_
#define _World_hpp_included_

//Prototypes.
//class GameObject;

#include <GameObject.hpp>

#include <vector>

//============World=========================
//
class World {
public:

									World();
									World(int, int);

	void							resolveMapCollision(GameObject*, int, int);
	void							resolveObjectCollision(GameObject*, int);
	void							loadLevelMap(std::string);
	void							deleteLevelMap();

	std::vector<GameObject>&		getGameObjects();

	std::vector<std::vector<int>>	getLevelMap();
	int								getMapHeight();
	int								getMapWidth();

private:

	std::vector<std::vector<int>>	mLevelMap;
	std::vector<std::vector<bool>>	mCollisionMap;

	int								mMapHeight;
	int								mMapWidth;

	std::vector<GameObject>			mGameObjects;

};
//
//==========================================


#endif
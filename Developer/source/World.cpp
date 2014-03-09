//World.cpp
#include <SFML/Graphics.hpp>
#include <GameObject.hpp>
#include <fstream>

#include <World.hpp>


//============World=========================
//
World::World() {

	mMapHeight = 0;
	mMapWidth = 0;

}

World::World(int mapHeight, int mapWidth) {

	mMapHeight = mapHeight;
	mMapWidth = mapWidth;

}

void World::resolveMapCollision(GameObject* object, int direction, int tileSize) {

	sf::FloatRect rect = object->getPhysics()->getRect();
	sf::Vector2f movement = object->getPhysics()->getMovement();

	for(int i = rect.top / tileSize; i < (rect.top + rect.height) / tileSize; ++i)
		for(int j = rect.left / tileSize; j < (rect.left + rect.width) / tileSize; ++j) {

			if(mLevelMap[i][j] == 'B') {

				if((movement.x > 0) && (direction == 0)) {rect.left = j * tileSize - rect.width;	object->getInput()->setBadDirection(2);}
				if((movement.x < 0) && (direction == 0)) {rect.left = j * tileSize + tileSize;		object->getInput()->setBadDirection(4);}
				if((movement.y > 0) && (direction == 1)) {rect.top = i * tileSize - rect.height;	object->getInput()->setBadDirection(3);}
				if((movement.y < 0) && (direction == 1)) {rect.top = i * tileSize + tileSize;		object->getInput()->setBadDirection(1);}

			}

		}

	object->getPhysics()->setRect(rect);

}

void World::resolveObjectCollision(GameObject* object, int direction) {

	sf::FloatRect rect = object->getPhysics()->getRect();
	sf::Vector2f movement = object->getPhysics()->getMovement();

	for(int i = 0; i < mGameObjects.size(); ++i) {

		sf::FloatRect currentRect = mGameObjects[i].getPhysics()->getRect();
		if((rect.intersects(currentRect)) && (object != &mGameObjects[i])) {

			if((movement.x > 0) && (direction == 0)) {rect.left -= (rect.left + rect.width) - currentRect.left;			object->getInput()->setBadDirection(2);}
			if((movement.x < 0) && (direction == 0)) {rect.left += (currentRect.left + currentRect.width) - rect.left;	object->getInput()->setBadDirection(4);}
			if((movement.y > 0) && (direction == 1)) {rect.top -= (rect.top + rect.height) - currentRect.top;			object->getInput()->setBadDirection(3);}
			if((movement.y < 0) && (direction == 1)) {rect.top += (currentRect.top + currentRect.height) - rect.top;	object->getInput()->setBadDirection(1);}

		}

	}

	object->getPhysics()->setRect(rect);

}

void World::loadLevelMap(std::string filename) {

	std::ifstream inputFile(filename);

	inputFile >> mMapHeight >> mMapWidth;

	//Resizing the array.
	mLevelMap.resize(mMapHeight);
	for(int i = 0; i < mMapHeight; ++i)
		mLevelMap[i].resize(mMapWidth);

	inputFile.get();
	for(int i = 0; i < mMapHeight; ++i) {
		for(int j = 0; j < mMapWidth; ++j) {
			char temp = inputFile.get();
			mLevelMap[i][j] = int(temp);
		}
		inputFile.get();
	}

	inputFile.close();

}

void World::deleteLevelMap() {
	mLevelMap.clear();
	mGameObjects.clear();
}

std::vector<GameObject>& World::getGameObjects() {
	return mGameObjects;
}

std::vector<std::vector<int>> World::getLevelMap() {
	return mLevelMap;
}

int World::getMapHeight() {
	return mMapHeight;
}

int World::getMapWidth() {
	return mMapWidth;
}
//
//==========================================
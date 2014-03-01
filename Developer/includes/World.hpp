#pragma once
#include <SFML/Graphics.hpp>
#include <Enemy.hpp>
#include <DropItem.hpp>

class World {
public:

									World();
									World(int, int);
	void							resolveCollision(sf::FloatRect& rect, sf::Vector2f movement, int direction, int tileSize);
	void							loadLevelMap(std::string);
	void							deleteLevelMap();

	std::vector<Enemy>&				getEnemies();
	std::vector<DropItem>&			getDrops();

	std::vector<std::vector<int>>	getLevelMap();
	int								getMapHeight();
	int								getMapWidth();

private:

	std::vector<std::vector<int>>	mLevelMap;
	int								mMapHeight;
	int								mMapWidth;

	//Player							mPlayer;
	std::vector<Enemy>				mEnemies;
	std::vector<DropItem>			mDrops;

};
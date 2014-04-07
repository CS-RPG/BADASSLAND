//LocalMap.hpp
#ifndef _LocalMap_hpp_included_
#define _LocalMap_hpp_included_

#include <GameObject.hpp>
#include <SFML/Graphics.hpp>
#include <DataTypes.hpp>
#include <State.hpp>

#include <vector>

//============LocalMap======================
//
class LocalMap : public State {
public:

									LocalMap(sf::Font& font, int fontCharacterSize, int tileSize);

	void							update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config);
	void							render(sf::RenderWindow& window, sf::View& view, config& config);

	void							resolveMapCollision(GameObject*, int, int);
	void							resolveObjectCollision(GameObject*, int);
	void							loadLevelMap(std::string);
	void							deleteLevelMap();

private:

	std::vector<std::vector<bool>>	mCollisionMap;

	sf::RectangleShape				mTile;

	//sf::Font						mFont;

	sf::Text						mTextHealth;
	sf::Text						mTextMana;
	sf::Text						mTextEnemyCount;
	sf::Text						mTextPlayerCoordinates;
	sf::Text						mTextMouseCoordinates;

};
//
//==========================================


#endif
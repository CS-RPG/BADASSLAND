//State.hpp
#ifndef _State_hpp_included_
#define _State_hpp_included_

#include <SFML/Graphics.hpp>
#include <DataTypes.hpp>
#include <GameObject.hpp>

//============State=========================
//
class State {
public:

	//virtual							~State();
	virtual void					update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config);
	virtual void					render(sf::RenderWindow& window, sf::View& view, config& config);
	//void							interpreter();

	std::vector<GameObject>&		getGameObjects();

	std::vector<std::vector<int>>&	getLevelMap();
	int								getMapHeight();
	int								getMapWidth();

	void							setMapHeight(int height);
	void							setMapWidth(int width);

private:

	std::vector<std::vector<int>>	mLevelMap;
	std::vector<GameObject>			mGameObjects;

	int								mMapHeight;
	int								mMapWidth;

};
//
//==========================================


#endif
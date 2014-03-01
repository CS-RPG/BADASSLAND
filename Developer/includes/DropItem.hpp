#pragma once
#include <SFML/Graphics.hpp>
#include <Player.hpp>

#include <DropItem.cpp>

class DropItem {
public:

									DropItem(sf::Texture&, std::string, int, int, int);
	void							update(float);
	void							action(Player&);

	sf::FloatRect					getRect();

	sf::Sprite						getSprite();

	bool							isMarkedForRemoval();

private:

	sf::FloatRect					mRect;
	sf::Sprite						mSprite;
	float							mCurrentFrame;
	int								mEffectValue;
	bool							mIsMarkedForRemoval;

};
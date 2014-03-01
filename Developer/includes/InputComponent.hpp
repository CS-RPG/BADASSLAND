#pragma once
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <Player.hpp>

class Player {
public:

									Player(sf::Texture&, sf::Texture&, int, int, sf::Font&);
	void							update(float, std::vector<std::vector<int>>, struct config*, World&);
	void							getMessage(int);
	void							takeDamage(float);
	void							heal(float);
	void							showStats(std::ostringstream*, std::ostringstream*);

	sf::FloatRect					getRect();
	sf::Sprite						getSprite();
	sf::Sprite						getHpSprite();
	sf::Text						getTextName();
	float							getSpeed();
	sf::Vector2f					getMovement();

	void							setSpeed(float);
	void							setMovement(sf::Vector2f);

private:

	InputComponent					mInput;

	sf::Vector2f					mMovement;
	float							mSpeed;

	sf::FloatRect					mRect;
	sf::Sprite						mSprite;
	sf::Sprite						mHpSprite;
	sf::String						mName;
	sf::Text						mTextName;

	float							mCurrentFrame;
	float							mAnimationSpeed;
	static const int				mFrameCount = 10;

	bool							mIsAlive;
	float							mHP;
	float							mMaxHp;
	int								mMP;

	std::vector<int>				mMessages;	
	
};
class InputComponent {
public:

	void update(Player& player) {

		sf::Vector2f movement = player.getMovement();

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))	movement.y -= player.getSpeed();
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	movement.y += player.getSpeed();
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	movement.x -= player.getSpeed();
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	movement.x += player.getSpeed();

		player.setMovement(movement);

	}

private:
	

};
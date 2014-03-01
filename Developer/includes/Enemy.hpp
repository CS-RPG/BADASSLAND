#pragma once
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <DataTypes.hpp>
#include <Player.hpp>

#include <Enemy.cpp>

class Enemy {
public:

									Enemy(sf::Texture&, int, int, sf::Font&);

	void							update(float, std::vector<std::vector<int>>, struct config*, World&);
	void							collision(std::vector<std::vector<int>>, struct config*);
	void							dealDamage(Player&);
	void							detectTargets(World&);
	void							getMessage(int);

	sf::FloatRect					getRect();
	sf::Sprite						getSprite();
	sf::Text						getTextName();
	bool							isReadyToAttack();

private:

	sf::Vector2f					mMovement;
	int								mDirection;
	float							mSpeed;

	sf::FloatRect					mRect;
	sf::Sprite						mSprite;

	float							mCurrentFrame;
	float							mAnimationSpeed;
	static const int				mFrameCount = 10;

	bool							mIsAlive;
	int								mHP;

	sf::Clock						mDamageClock;
	sf::Clock						mMovementClock;
	float							mDamage;

	sf::String						mName;
	sf::Text						mTextName;

	std::vector<int>				mMessages;

};
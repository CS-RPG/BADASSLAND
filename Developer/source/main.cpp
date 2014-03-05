//SFML includes.
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//std includes.
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <sstream>
#include <vector>


//FONT - BIG.
//http://patorjk.com/software/taag/


//  _____        _          _                         
// |  __ \      | |        | |                        
// | |  | | __ _| |_ __ _  | |_ _   _ _ __   ___  ___ 
// | |  | |/ _` | __/ _` | | __| | | | '_ \ / _ \/ __|
// | |__| | (_| | || (_| | | |_| |_| | |_) |  __/\__ \
// |_____/ \__,_|\__\__,_|  \__|\__, | .__/ \___||___/
//                               __/ | |              
//                              |___/|_|   
//


//DataTypes.hpp
struct Attributes {

     int							strength;				//Carry weight, melee damage. 
     int							endurance;				//HP, resistances.
     int							agility;				//Speed, ranged damage.
     int							intelligence;			//MP, magic damage.
     int							luck;					//Dodge chance, critical damage chance.

};

struct Resistances {

     int							poison;
     int							paralize;
     int							lightning;
     int							fire;
     int							ice;
     int							earth;

};

struct config {

	int								screenWidth;					
	int								screenHeight;

	int								tileSize;	

	int								playerStartingX;					
	int								playerStartingY;	

	int								gameSpeed;
	std::string						levelMapName;

	float							offsetX;				//Map scrolling
	float							offsetY;				//offset.
	
};

struct Message {

	int								id;
	int								contents;

};

//CLASS PROTOTYPES.
class World;
class Enemy;
class GameObject;

//   _____                                             _       
//  / ____|                                           | |      
// | |     ___  _ __ ___  _ __   ___  _ __   ___ _ __ | |_ ___ 
// | |    / _ \| '_ ` _ \| '_ \ / _ \| '_ \ / _ \ '_ \| __/ __|
// | |___| (_) | | | | | | |_) | (_) | | | |  __/ | | | |_\__ \
//  \_____\___/|_| |_| |_| .__/ \___/|_| |_|\___|_| |_|\__|___/
//                       | |                                   
//                       |_|
//


//Base.
class InputComponent {
public:

	virtual							~InputComponent();
	virtual void					update(GameObject&, World&) = 0;

	int								getBadDirection();
	void							setBadDirection(int);

private:

	int								mBadDirection;

};

class PhysicsComponent {
public:

	virtual							~PhysicsComponent();
	virtual void					update(GameObject&, World&, int, float) = 0;

	sf::FloatRect					getRect();
	sf::Vector2f					getMovement();
	int								getDirection();
	float							getSpeed();

	void							setRect(sf::FloatRect);
	void							setMovement(sf::Vector2f);
	void							setDirection(int);
	void							setSpeed(float);

private:

	sf::FloatRect					mRect;
	sf::Vector2f					mMovement;
	int								mDirection;
	float							mSpeed;

};

class GraphicsComponent {
public:

	virtual							~GraphicsComponent();
	virtual void					update(GameObject&, float) = 0;
	virtual void					draw(sf::RenderWindow&) = 0;

	sf::Texture&					getTexture();
	void							setTexture(sf::Texture&);

private:

	sf::Texture						mTexture;

};

class CombatComponent {
public:

	virtual							~CombatComponent();
	virtual void					attack(GameObject*) = 0;
	virtual void					takeDamage(float) = 0;
	//virtual void					heal(float) = 0;

	float							getHP();
	float							getMaxHP();
	float							getDT();
	bool							isAlive();

	float							getAttackRange();
	float							getAttackSpeed();
	float							getDamage();
	bool							isReadyToAttack();

	void							setHP(float);
	void							setMaxHP(float);
	void							setDT(float);
	void							kill();
	void							revive();

	void							setAttackRange(float);
	void							setAttackSpeed(float);
	void							setDamage(float);

	void							restartAttackClock();

private:

	float							mHP;
	float							mMaxHP;
	float							mDT;
	bool							mIsAlive;

	float							mAttackRange;
	float							mAttackSpeed;
	float							mDamage;

	sf::Clock						mAttackClock;						

};

class SocialComponent {
public:

	virtual							~SocialComponent();
	//virtual void					update(GameObject&) = 0;

	sf::String						getName();
	sf::String						getFaction();

	void							setName(sf::String);
	void							setFaction(sf::String);

private:

	sf::String						mName;
	sf::String						mFaction;

};

//Player.
class PlayerInputComponent : public InputComponent {
public:

	virtual void					update(GameObject&, World&);

};

class PlayerPhysicsComponent : public PhysicsComponent {
public:

									PlayerPhysicsComponent(sf::FloatRect, float);
	virtual void					update(GameObject&, World&, int, float);

};

class PlayerGraphicsComponent : public GraphicsComponent {
public:

									PlayerGraphicsComponent(sf::Texture*, sf::Texture*, sf::Font*);

	virtual void					update(GameObject&, float);
	virtual void					draw(sf::RenderWindow&);

private:

	sf::Texture*					mTexture;
	sf::Texture*					mHPBarTexture;

	sf::Sprite						mCurrentSprite;

	sf::Sprite						mSpriteStill;
	sf::Sprite						mSpriteUp;
	sf::Sprite						mSpriteDown;
	sf::Sprite						mSpriteLeft;
	sf::Sprite						mSpriteRight;

	sf::Sprite						mHPBarSprite;
	sf::Text						mText;

	float							mCurrentFrame;
	float							mAnimationSpeed;
	int								mFrameCount;

};

class PlayerCombatComponent : public CombatComponent {
public:

									PlayerCombatComponent(int, int, float, float, float);
	virtual void					attack(GameObject*);
	virtual void					takeDamage(float);
	//virtual void					heal(float);

};

class PlayerSocialComponent : public SocialComponent {
public:

									PlayerSocialComponent(sf::String, sf::String);

};

//Enemy.
class EnemyInputComponent : public InputComponent {
public:

	virtual void					update(GameObject&, World&);

private:

	float							mFraction;

};

class EnemyPhysicsComponent : public PhysicsComponent {
public:

	virtual void					update(Enemy&, World&, int, float);

};



class PassiveInputComponent : public InputComponent {
public:

									PassiveInputComponent(float);
	virtual void					update(GameObject&, World&);

	float							getChangeDirectionFrequency();
	void							setChangeDirectionFrequency(float);
	

private:

	sf::Clock						mMovementClock;
	float							mChangeDirectionFrequency;
	int								mDirection;

};

//   _____ _                         
//  / ____| |                        
// | |    | | __ _ ___ ___  ___  ___ 
// | |    | |/ _` / __/ __|/ _ \/ __|
// | |____| | (_| \__ \__ \  __/\__ \
//  \_____|_|\__,_|___/___/\___||___/
//       


//GameObject.hpp
class GameObject {
public:

									GameObject( InputComponent*,
												PhysicsComponent*,
												GraphicsComponent*,
												CombatComponent*,
												SocialComponent* );

	void							update(float, std::vector<std::vector<int>>, struct config*, World&);

	InputComponent*&				getInput();
	PhysicsComponent*&				getPhysics();
	GraphicsComponent*&				getGraphics();
	CombatComponent*&				getCombat();
	SocialComponent*&				getSocial();


private:

	InputComponent*					mInput;
	PhysicsComponent*				mPhysics;
	GraphicsComponent*				mGraphics;
	CombatComponent*				mCombat;
	SocialComponent*				mSocial;

};

//DropItem.hpp
class DropItem {
public:

									DropItem(sf::Texture&, int, int, int);

	void							update(float);
	void							action(GameObject&);

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

//World.hpp
class World {
public:

									World(sf::Texture*, sf::Texture*, sf::Font*);
									World(int, int);

	void							resolveMapCollision(GameObject*, int, int);
	void							loadLevelMap(std::string);
	void							deleteLevelMap();

	GameObject*&					getPlayer();
	std::vector<GameObject>&		getEnemies();
	std::vector<DropItem>&			getDrops();

	std::vector<std::vector<int>>	getLevelMap();
	int								getMapHeight();
	int								getMapWidth();

private:

	GameObject*						mPlayer;

	std::vector<std::vector<int>>	mLevelMap;
	std::vector<std::vector<bool>>	mCollisionMap;

	int								mMapHeight;
	int								mMapWidth;

	std::vector<GameObject>			mEnemies;
	std::vector<DropItem>			mDrops;

};

//Game.hpp
class Game {
public:

									Game();

	void							run();
	void							update(sf::Time);
	void							processEvents();
	void							render();

	void							loadConfigFile(config&);

private:

	sf::RenderWindow				mWindow;
	int								mScreenWidth;					
	int								mScreenHeight;

	float							mInvincibilityTime;
	sf::Clock						mGameClock;
	sf::Clock						mInvincibilityClock;
	sf::Clock						mSpawnClock;
	sf::Time						mTimePerFrame;

	config							mConfig;
	std::vector<std::vector<int>>	mLevelMap;
			
	int								mTileSize;						
	int								mGameSpeed;
	float							mOffsetX;				//Map scrolling
	float							mOffsetY;				//offset.

	World							mWorld;

};


//   _____                                             _      __  __      _   _               _     
//  / ____|                                           | |    |  \/  |    | | | |             | |    
// | |     ___  _ __ ___  _ __   ___  _ __   ___ _ __ | |_   | \  / | ___| |_| |__   ___   __| |___ 
// | |    / _ \| '_ ` _ \| '_ \ / _ \| '_ \ / _ \ '_ \| __|  | |\/| |/ _ \ __| '_ \ / _ \ / _` / __|
// | |___| (_) | | | | | | |_) | (_) | | | |  __/ | | | |_   | |  | |  __/ |_| | | | (_) | (_| \__ \
//  \_____\___/|_| |_| |_| .__/ \___/|_| |_|\___|_| |_|\__|  |_|  |_|\___|\__|_| |_|\___/ \__,_|___/
//                       | |                                                                        
//                       |_|  
//


//
//BASE.
//

//InputComponent.cpp
InputComponent::~InputComponent() {}

int InputComponent::getBadDirection() {
	return mBadDirection;
}

void InputComponent::setBadDirection(int direction) {
	mBadDirection = direction;
}

//PhysicsComponent.cpp
PhysicsComponent::~PhysicsComponent() {}

sf::FloatRect PhysicsComponent::getRect() {
	return mRect;
}

sf::Vector2f PhysicsComponent::getMovement() {
	return mMovement;
}

int PhysicsComponent::getDirection() {
	return mDirection;
}

float PhysicsComponent::getSpeed() {
	return mSpeed;
}

void PhysicsComponent::setRect(sf::FloatRect rect) {
	mRect = rect;
}

void PhysicsComponent::setMovement(sf::Vector2f movement) {
	mMovement = movement;
}

void PhysicsComponent::setDirection(int direction) {
	mDirection = direction;
}

void PhysicsComponent::setSpeed(float speed) {
	mSpeed = speed;
}



//GraphicsComponent.cpp
GraphicsComponent::~GraphicsComponent() {}

sf::Texture& GraphicsComponent::getTexture() {
	return mTexture;
}

void GraphicsComponent::setTexture(sf::Texture& texture) {
	mTexture = texture;
}



//CombatComponent.cpp
CombatComponent::~CombatComponent() {}

float CombatComponent::getHP() {
	return mHP;
}

float CombatComponent::getMaxHP() {
	return mMaxHP;
}

float CombatComponent::getDT() {
	return mDT;
}

bool CombatComponent::isAlive() {
	return mIsAlive;
}

float CombatComponent::getAttackRange() {
	return mAttackRange;
}

float CombatComponent::getAttackSpeed() {
	return mAttackSpeed;
}

float CombatComponent::getDamage() {
	return mDamage;
}

bool CombatComponent::isReadyToAttack() {

	if(mAttackClock.getElapsedTime().asSeconds() > 1 / mAttackSpeed)
		return true;
	else
		return false;

}

void CombatComponent::setHP(float HP) {
	mHP = HP;
}

void CombatComponent::setMaxHP(float maxHP) {
	mMaxHP = maxHP;
}

void CombatComponent::setDT(float DT) {
	mDT = DT;
}

void CombatComponent::kill() {
	mIsAlive = false;
}

void CombatComponent::revive() {
	mIsAlive = true;
}

void CombatComponent::setAttackRange(float attackRange) {
	mAttackRange = attackRange;
}

void CombatComponent::setAttackSpeed(float attackSpeed) {
	mAttackSpeed = attackSpeed;
}

void CombatComponent::setDamage(float damage) {
	mDamage = damage;
}

void CombatComponent::restartAttackClock() {
	mAttackClock.restart();
}

//SocialComponent.cpp
SocialComponent::~SocialComponent() {}

sf::String SocialComponent::getName() {
	return mName;
}

sf::String SocialComponent::getFaction() {
	return mFaction;
}

void SocialComponent::setName(sf::String name) {
	mName = name;
}

void SocialComponent::setFaction(sf::String faction) {
	mFaction = faction;
}







//
//PLAYER
//
//PlayerInputComponent.cpp
void PlayerInputComponent::update(GameObject& player, World& world) {

	sf::Vector2f movement = player.getPhysics()->getMovement();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))	movement.y -= player.getPhysics()->getSpeed();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	movement.y += player.getPhysics()->getSpeed();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	movement.x -= player.getPhysics()->getSpeed();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	movement.x += player.getPhysics()->getSpeed();

	player.getPhysics()->setMovement(movement);

}



//PlayerPhysicsComponent.cpp
PlayerPhysicsComponent::PlayerPhysicsComponent(sf::FloatRect rect, float speed = 0.1) {

	setRect(rect);
	setSpeed(speed);
	setDirection(0);

}

void PlayerPhysicsComponent::update(GameObject& player, World& world, int tileSize, float deltaTime) {
	
	sf::Vector2f movement = player.getPhysics()->getMovement();

	if((movement.x == 0) && (movement.y == 0))	setDirection(0);
	if((movement.x == 0) && (movement.y < 0))	setDirection(1);
	if((movement.x > 0) && (movement.y < 0))	setDirection(2);
	if((movement.x > 0) && (movement.y == 0))	setDirection(3);
	if((movement.x > 0) && (movement.y > 0))	setDirection(4);
	if((movement.x == 0) && (movement.y > 0))	setDirection(5);
	if((movement.x < 0) && (movement.y > 0))	setDirection(6);
	if((movement.x < 0) && (movement.y == 0))	setDirection(7);
	if((movement.x < 0) && (movement.y < 0))	setDirection(8);

	sf::FloatRect rect = player.getPhysics()->getRect();
	rect.left += movement.x * deltaTime;
	player.getPhysics()->setRect(rect);
	world.resolveMapCollision(&player, 0, tileSize);

	rect = player.getPhysics()->getRect();
	rect.top += movement.y * deltaTime;
	player.getPhysics()->setRect(rect);
	world.resolveMapCollision(&player, 1, tileSize);

	movement.x = 0;
	movement.y = 0;

	player.getPhysics()->setMovement(movement);
	
}



//PlayerGraphicsComponent.cpp
PlayerGraphicsComponent::PlayerGraphicsComponent(sf::Texture* texture, sf::Texture* hpTexture, sf::Font* font)
:mTexture(texture) {

	mCurrentSprite.setTexture(*mTexture);
	mSpriteStill.setTexture(*mTexture);
	mSpriteUp.setTexture(*mTexture);
	mSpriteDown.setTexture(*mTexture);
	mSpriteLeft.setTexture(*mTexture);
	mSpriteRight.setTexture(*mTexture);

	mSpriteStill.setTextureRect(sf::IntRect(0, 15, 120, 120));
	mSpriteUp.setTextureRect(sf::IntRect(0, 787, 0, 0));
	mSpriteDown.setTextureRect(sf::IntRect(0, 535, 0, 0));
	mSpriteLeft.setTextureRect(sf::IntRect(0, 667, 0, 0));
	mSpriteRight.setTextureRect(sf::IntRect(0, 925, 0, 0));

	mHPBarSprite.setTexture(*hpTexture);
	mHPBarSprite.setTextureRect(sf::IntRect(0, 0, 100, 10));

	mText.setFont(*font);
	mText.setCharacterSize(30);
	mText.setStyle(sf::Text::Bold);
	mText.setColor(sf::Color::Red);

	mCurrentSprite = mSpriteStill;

	mFrameCount = 10;
	mCurrentFrame= 0;

}

void PlayerGraphicsComponent::update(GameObject& player, float deltaTime) {

	if(player.getPhysics()->getDirection() == 0) mCurrentSprite = mSpriteStill;

	if(player.getPhysics()->getDirection() == 3) mCurrentSprite = mSpriteRight;
	if(player.getPhysics()->getDirection() == 7) mCurrentSprite = mSpriteLeft;
	if(player.getPhysics()->getDirection() == 5) mCurrentSprite = mSpriteDown;
	if(player.getPhysics()->getDirection() == 1) mCurrentSprite = mSpriteUp;

	mAnimationSpeed = player.getPhysics()->getSpeed() * 0.05;
	mCurrentFrame += mAnimationSpeed * deltaTime;
	if(mCurrentFrame > mFrameCount) mCurrentFrame -= mFrameCount;
	
	if(player.getPhysics()->getDirection() != 0)
		mCurrentSprite.setTextureRect(sf::IntRect(	player.getPhysics()->getRect().width * int(mCurrentFrame),
													mCurrentSprite.getTextureRect().top,
													player.getPhysics()->getRect().width,
													player.getPhysics()->getRect().height  ));

	//HP bar.
	float hpPercentage = player.getCombat()->getHP() / player.getCombat()->getMaxHP();
	if(hpPercentage >= 0.6)
		mHPBarSprite.setColor(sf::Color::Green);
	else if((hpPercentage >= 0.35) && (hpPercentage < 0.6))
		mHPBarSprite.setColor(sf::Color::Yellow);
	else if(hpPercentage < 0.35)
		mHPBarSprite.setColor(sf::Color::Red);

	mHPBarSprite.setTextureRect(sf::IntRect(100 * (1 - hpPercentage), 0, 100, 10));
	mHPBarSprite.setPosition(player.getPhysics()->getRect().left, player.getPhysics()->getRect().top + player.getPhysics()->getRect().height);

	//Text.
	mText.setString(player.getSocial()->getName());
	mText.setPosition(player.getPhysics()->getRect().left, player.getPhysics()->getRect().top - mText.getCharacterSize());

	mCurrentSprite.setPosition(player.getPhysics()->getRect().left, player.getPhysics()->getRect().top);

}

void PlayerGraphicsComponent::draw(sf::RenderWindow& window) {
	
	window.draw(mCurrentSprite);
	window.draw(mHPBarSprite);
	window.draw(mText);

}



//PlayerCombatComponent.cpp
PlayerCombatComponent::PlayerCombatComponent(int HP, int maxHP, float damage, float attackRange, float attackSpeed) {
	
	setHP(HP);
	setMaxHP(maxHP);

	setDamage(damage);
	setAttackRange(attackRange);
	setAttackSpeed(attackSpeed);

}

void PlayerCombatComponent::takeDamage(float damage) {
	setHP(getHP() - damage);
}

void PlayerCombatComponent::attack(GameObject* target) {
	target->getCombat()->takeDamage(getDamage());
	restartAttackClock();
}

//PlayerSocialComponent.cpp
PlayerSocialComponent::PlayerSocialComponent(sf::String name, sf::String faction) {
	setName(name);
	setFaction(faction);
}





//
//ENEMY
//
//EnemyInputComponent.cpp
void EnemyInputComponent::update(GameObject& source, World& world) {

	GameObject* target = world.getPlayer();
	sf::Vector2f movement = source.getPhysics()->getMovement();

	float deltaX =  (target->getPhysics()->getRect().left + target->getPhysics()->getRect().width / 2) -
					(source.getPhysics()->getRect().left + source.getPhysics()->getRect().width / 2);

	float deltaY =  (target->getPhysics()->getRect().top + target->getPhysics()->getRect().height / 2) - 
					(source.getPhysics()->getRect().top + source.getPhysics()->getRect().height / 2);
	float distance = std::sqrt( deltaX * deltaX + deltaY * deltaY);

	bool movingHorizontal = true;
	bool movingVertical = false;
	bool changeDirection = false;
	

	if(distance > source.getCombat()->getAttackRange()) {

		/*
		if(target->getPhysics()->getRect().left > source.getPhysics()->getRect().left)
			movement.x += source.getPhysics()->getSpeed();
		else
			movement.x -= source.getPhysics()->getSpeed();

		if(target->getPhysics()->getRect().top > source.getPhysics()->getRect().top)
			movement.y += source.getPhysics()->getSpeed();
		else
			movement.y -= source.getPhysics()->getSpeed();
		*/

		if(std::abs(deltaX) > std::abs(deltaY) * 1.3) {
			movingHorizontal = true;
			movingVertical = false;
		}

		if(std::abs(deltaY) > std::abs(deltaX) * 1.3) {
			movingHorizontal = false;
			movingVertical = true;
		}

		if(movingHorizontal)
			if(target->getPhysics()->getRect().left > source.getPhysics()->getRect().left)
				movement.x += source.getPhysics()->getSpeed();
			else
				movement.x -= source.getPhysics()->getSpeed();
		
		if(movingVertical)
			if(target->getPhysics()->getRect().top > source.getPhysics()->getRect().top)
				movement.y += source.getPhysics()->getSpeed();
			else
				movement.y -= source.getPhysics()->getSpeed();
		
	} else {

		movement.x = 0;
		movement.y = 0;
		if(source.getCombat()->isReadyToAttack())
			source.getCombat()->attack(target);

	}

	source.getPhysics()->setMovement(movement);

}


//PassiveInputComponent.cpp
PassiveInputComponent::PassiveInputComponent(float changeDirectionFrequency = 4) {
	mChangeDirectionFrequency = changeDirectionFrequency;
	mDirection = rand() % 4 + 1;
	setBadDirection(0);
}

void PassiveInputComponent::update(GameObject& source, World& world) {

	sf::Vector2f movement = source.getPhysics()->getMovement();

	if(mMovementClock.getElapsedTime().asSeconds() > mChangeDirectionFrequency) {
		mDirection = rand() % 4 + 1;
		mMovementClock.restart();
	}

	if(getBadDirection() != 0) {
		mDirection = rand() % 4 + 1;
		setBadDirection(0);
	}

	if(mDirection == 1) {movement.x = 0;								movement.y = -source.getPhysics()->getSpeed();}
	if(mDirection == 2) {movement.x = source.getPhysics()->getSpeed();	movement.y = 0;}
	if(mDirection == 3) {movement.x = 0;								movement.y = source.getPhysics()->getSpeed();}
	if(mDirection == 4) {movement.x = -source.getPhysics()->getSpeed();	movement.y = 0;}

	source.getPhysics()->setMovement(movement);

}

float PassiveInputComponent::getChangeDirectionFrequency() {
	return mChangeDirectionFrequency;
}

void PassiveInputComponent::setChangeDirectionFrequency(float changeDirectionFrequency) {
	mChangeDirectionFrequency = changeDirectionFrequency;
}

//   _____ _                                  _   _               _     
//  / ____| |                                | | | |             | |    
// | |    | | __ _ ___ ___     _ __ ___   ___| |_| |__   ___   __| |___ 
// | |    | |/ _` / __/ __|   | '_ ` _ \ / _ \ __| '_ \ / _ \ / _` / __|
// | |____| | (_| \__ \__ \   | | | | | |  __/ |_| | | | (_) | (_| \__ \
//  \_____|_|\__,_|___/___/   |_| |_| |_|\___|\__|_| |_|\___/ \__,_|___/
//                                                                      
                                

//
//World.cpp
//
World::World(sf::Texture* playerTexture, sf::Texture* hpBar, sf::Font* font) {

	mMapHeight = 0;
	mMapWidth = 0;

	mPlayer = new GameObject( new PassiveInputComponent(), 
							  new PlayerPhysicsComponent(sf::FloatRect(250, 200, 120, 120), 0.05), 
							  new PlayerGraphicsComponent(&*playerTexture, &*hpBar, &*font),
							  new PlayerCombatComponent(150, 150, 40, 40, 40),
							  new PlayerSocialComponent("Player", "players") );

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
	mEnemies.clear();
	mDrops.clear();
}

GameObject*& World::getPlayer() {
	return mPlayer;
}

std::vector<GameObject>& World::getEnemies() {
	return mEnemies;
}

std::vector<DropItem>& World::getDrops() {
	return mDrops;
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
//Game.cpp
//
void Game::run() {

	loadConfigFile(mConfig);

	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	mTimePerFrame = sf::seconds(1.f/60.f);

	while(mWindow.isOpen()) {

		sf::Time elapsedTime = mGameClock.restart();
		timeSinceLastUpdate = elapsedTime;
		while(timeSinceLastUpdate > mTimePerFrame) {

			timeSinceLastUpdate -= mTimePerFrame;

			processEvents();
			update(mTimePerFrame);

		}

	}

}

void Game::processEvents() {

	sf::Event event;

	while(mWindow.pollEvent(event)) {

		switch(event.type) {

			case(sf::Event::Closed)
				:mWindow.close();
				break;

		}

	}

}

void Game::update(sf::Time) {

	std::vector<GameObject> enemies = mWorld.getEnemies();
	std::vector<DropItem> drops = mWorld.getDrops();
	std::vector<std::vector<int>> levelMap = mWorld.getLevelMap();


}

void Game::render() {

}

void Game::loadConfigFile(config& config) {

	std::ifstream inputFile;
	inputFile.open("config.txt");
	
	std::string temp;

	getline(inputFile, temp);
	inputFile >> config.screenWidth >> config.screenHeight;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> config.tileSize;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> config.playerStartingX >> config.playerStartingY;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> config.gameSpeed;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> config.levelMapName;

	inputFile.close();
	return;

}


//
//GameObject.cpp
//
GameObject::GameObject( InputComponent* input, 
						PhysicsComponent* physics,
						GraphicsComponent* graphics,
						CombatComponent* combat,
						SocialComponent* social )
:mInput(input),
mPhysics(physics),
mGraphics(graphics),
mCombat(combat),
mSocial(social) {

}

void GameObject::update(float deltaTime, std::vector<std::vector<int>> levelMap, struct config* config, World& world) {

	mInput->update(*this, world);
	mPhysics->update(*this, world, config->tileSize, deltaTime);
	mGraphics->update(*this, deltaTime);

}

InputComponent*& GameObject::getInput() {
	return mInput;
}

PhysicsComponent*& GameObject::getPhysics() {
	return mPhysics;
}

GraphicsComponent*& GameObject::getGraphics() {
	return mGraphics;
}

CombatComponent*& GameObject::getCombat() {
	return mCombat;
}

SocialComponent*& GameObject::getSocial() {
	return mSocial;
}



//
//DropItem.cpp
//



DropItem::DropItem(sf::Texture& texture, int effect, int x, int y) {
	mSprite.setTexture(texture);
	mRect = sf::FloatRect(x, y, 32, 32);
	mSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	mCurrentFrame = 0;
	mEffectValue = effect;
	mIsMarkedForRemoval = false;
}

void DropItem::update(float time) {
	mSprite.setPosition(mRect.left, mRect.top);
}

void DropItem::action(GameObject& player) {
	//player.heal(mEffectValue);
	mIsMarkedForRemoval = true;
}

sf::FloatRect DropItem::getRect() {
	return mRect;
}

sf::Sprite DropItem::getSprite() {
	return mSprite;
}

bool DropItem::isMarkedForRemoval() {
	return mIsMarkedForRemoval;
}




//   _____           _                    __                  _   _                 
//  / ____|         | |                  / _|                | | (_)                
// | (___  _   _ ___| |_ ___ _ __ ___   | |_ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//  \___ \| | | / __| __/ _ \ '_ ` _ \  |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//  ____) | |_| \__ \ ||  __/ | | | | | | | | |_| | | | | (__| |_| | (_) | | | \__ \
// |_____/ \__, |___/\__\___|_| |_| |_| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//          __/ |                                                                   
//         |___/  
//

//
//
//	SOME STUFF.
//
//

//  ______             _               __                  _   _                 
// |  ____|           (_)             / _|                | | (_)                
// | |__   _ __   __ _ _ _ __   ___  | |_ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// |  __| | '_ \ / _` | | '_ \ / _ \ |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |____| | | | (_| | | | | |  __/ | | | |_| | | | | (__| |_| | (_) | | | \__ \
// |______|_| |_|\__, |_|_| |_|\___| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                __/ |                                                          
//               |___
//


void loadConfigFile(config& config, std::string filename = "config.txt") {

	std::ifstream inputFile;
	inputFile.open(filename);
	
	std::string temp;

	getline(inputFile, temp);
	inputFile >> config.screenWidth >> config.screenHeight;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> config.tileSize;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> config.playerStartingX >> config.playerStartingY;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> config.gameSpeed;
	inputFile.get();
	inputFile.get();

	getline(inputFile, temp);
	inputFile >> config.levelMapName;

	inputFile.close();
	return;

}

//FACTORY.



//  __  __       _       
// |  \/  |     (_)      
// | \  / | __ _ _ _ __  
// | |\/| |/ _` | | '_ \ 
// | |  | | (_| | | | | |
// |_|  |_|\__,_|_|_| |_|
//                       

int main() {
	
	srand(time(NULL));

	//Loading config file.
	config config;
	loadConfigFile(config, "config.txt");

	//Game game;
	//game.run();


	//Clocks.
	sf::Clock gameClock;
	sf::Clock invincibilityClock;
	sf::Clock spawnClock;


	//
	//GRAPHICS.
	//
	//
	//
	sf::RectangleShape rectangle(sf::Vector2f(config.tileSize, config.tileSize));	//For tiles (temporary).

	//Loading font.
	sf::Font font;
	font.loadFromFile("sansation.ttf");

	//HUD Health.
	sf::Text textHealth("", font, 30);
	textHealth.setStyle(sf::Text::Bold);
	textHealth.setColor(sf::Color::Red);
	textHealth.setPosition(0, 0);

	//HUD Mana.
	sf::Text textMana("", font, 30);
	textMana.setStyle(sf::Text::Bold);
	textMana.setColor(sf::Color::Blue);
	textMana.setPosition(0, textMana.getCharacterSize());

	//HUD Enemy count.
	sf::Text textEnemyCount("", font, 30);
	textEnemyCount.setStyle(sf::Text::Bold);
	textEnemyCount.setColor(sf::Color::Color(184, 138, 0));
	textEnemyCount.setPosition(0, textEnemyCount.getCharacterSize() * 2);

	//HUD Player coordinates.
	sf::Text textPlayerCoordinates("", font, 30);
	textPlayerCoordinates.setStyle(sf::Text::Bold);
	textPlayerCoordinates.setColor(sf::Color::Color(125, 145, 176));
	textPlayerCoordinates.setPosition(0, textPlayerCoordinates.getCharacterSize() * 3);

	//HUD Mouse coordinates.
	sf::Text textMouseCoordinates("", font, 30);
	textMouseCoordinates.setStyle(sf::Text::Bold);
	textMouseCoordinates.setColor(sf::Color::Color(125, 145, 176));
	textMouseCoordinates.setPosition(0, textMouseCoordinates.getCharacterSize() * 6);

	//Loading textures.
	sf::Texture hpBar;
	sf::Texture tileSet;
	sf::Texture playerTexture;
	sf::Texture enemyTexture;
	sf::Texture healthPotionTexture;

	if(!hpBar.loadFromFile("./textures/HPBar.png"))							return 1;
	if(!tileSet.loadFromFile("./textures/testTileSet.png"))					return 1;
	if(!playerTexture.loadFromFile("./textures/playerSpriteList.png"))		return 1;
	if(!enemyTexture.loadFromFile("./textures/enemySpriteList.png"))		return 1;
	if(!healthPotionTexture.loadFromFile("./textures/healthPotion.png"))	return 1;

	sf::Sprite tile(tileSet);

	//Sounds.
	sf::SoundBuffer emenyHitSoundBuffer;

	emenyHitSoundBuffer.loadFromFile("sound1.ogg");

	sf::Sound emenyHitSound(emenyHitSoundBuffer);

	//World.
	std::string levelMapName = "./levels/level1.txt";

	World world(&playerTexture, &hpBar, &font);
	world.loadLevelMap(levelMapName);
	std::vector<std::vector<int>> levelMap = world.getLevelMap();
	

	//Game objects.
	std::vector<DropItem> drops = world.getDrops();
	std::vector<GameObject> enemies = world.getEnemies();
	/*
	GameObject player(  new PlayerInputComponent(), 
						new PlayerPhysicsComponent(sf::FloatRect(config.playerStartingX, config.playerStartingY, 120, 120)), 
						new PlayerGraphicsComponent(&playerTexture, &hpBar, &font),
						new PlayerCombatComponent(150, 150, 40, 40, 40),
						new PlayerSocialComponent("Player", "players")  );
	*/
	GameObject* player = world.getPlayer();


	//Creating window, view.
	sf::RenderWindow mWindow(sf::VideoMode(config.screenWidth, config.screenHeight), "Badass Tales of BADASSLAND!!!!111");
	sf::View mView;
	mView.reset(sf::FloatRect(0, 0, config.screenWidth, config.screenHeight));
	mView.setViewport(sf::FloatRect(0, 0, 1, 1));
	sf::Vector2f mViewPosition;

	//Game cycle.
	while(mWindow.isOpen()) {

		float time = gameClock.getElapsedTime().asMicroseconds();
		gameClock.restart();
		time /= config.gameSpeed;

		//Processing events.
		sf::Event event;
		while(mWindow.pollEvent(event)) {
			switch(event.type) {
				
				case(sf::Event::Closed):
					mWindow.close();
					break;

			}
		}

		//Updating all objects.
		player->update(time, levelMap, &config, world);
		for(int i = 0; i < enemies.size(); ++i)	
			enemies[i].update(time, levelMap, &config, world);
		for(int i = 0; i < drops.size(); ++i)
			drops[i].update(time);

		//Resolving collisions with drop items.
		//for(int i = 0; i < drops.size(); ++i)
		//	if((player.getPhysics()->getRect().intersects(drops[i].getRect())) && (~drops[i].isMarkedForRemoval()) && (player.getHP() != player.getMaxHP()))
		//		drops[i].action(player);
	
		//Spawning elves.
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::G)) && (spawnClock.getElapsedTime().asSeconds() > 0.25)) {
			enemies.push_back( *(new GameObject(  new PassiveInputComponent(),
												  new PlayerPhysicsComponent(sf::FloatRect(config.playerStartingX, config.playerStartingY, 120, 120), 0.05),
												  new PlayerGraphicsComponent(&enemyTexture, &hpBar, &font),
												  new PlayerCombatComponent(150, 150, 40, 40, 2),
												  new PlayerSocialComponent("Dark Elf", "dark_elves")  )) );
			spawnClock.restart();
		}

		//Spawning health potions.
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::H)) && (spawnClock.getElapsedTime().asSeconds() > 0.25)) {
			drops.push_back( *(new DropItem(healthPotionTexture, 40, player->getPhysics()->getRect().left + (rand() % 2 - 1) * (rand() % 100), player->getPhysics()->getRect().top + (rand() % 2 - 1) * (rand() % 100))) );
			spawnClock.restart();
		}


		//HUD.
		std::ostringstream hudHealth;
		//std::ostringstream hudMana;
		std::ostringstream hudEnemyCount;
		std::ostringstream hudPlayerCoordinates;
		std::ostringstream hudMouseCoordinates;

		hudHealth << player->getCombat()->getHP();
		//hudMana << player.getMP();
		hudEnemyCount << "Number of enemies: " << enemies.size();
		hudPlayerCoordinates << "X: " << player->getPhysics()->getRect().left + config.tileSize / 2 << '\n'
							 << "Y: " << player->getPhysics()->getRect().top + config.tileSize / 2;
		hudMouseCoordinates << "X: " << sf::Mouse::getPosition(mWindow).x << '\n'
							<< "Y: " << sf::Mouse::getPosition(mWindow).y;

		//textMana.setString(hudMana.str());
		textHealth.setString(hudHealth.str());
		textEnemyCount.setString(hudEnemyCount.str());
		textPlayerCoordinates.setString(hudPlayerCoordinates.str());
		textMouseCoordinates.setString(hudMouseCoordinates.str());


		//View.
		mViewPosition.x = player->getPhysics()->getRect().left + config.tileSize / 2 - config.screenWidth / 2;
		mViewPosition.y = player->getPhysics()->getRect().top + config.tileSize / 2 - config.screenHeight / 2;
		
		if(mViewPosition.x < 0)																	mViewPosition.x = 0;
		if(mViewPosition.x > world.getMapWidth() * config.tileSize - config.screenWidth)		mViewPosition.x = world.getMapWidth() * config.tileSize - config.screenWidth;
		if(mViewPosition.y < 0)																	mViewPosition.y = 0;
		if(mViewPosition.y > world.getMapHeight() * config.tileSize - config.screenHeight)		mViewPosition.y = world.getMapHeight() * config.tileSize - config.screenHeight;

		mView.reset(sf::FloatRect(mViewPosition.x, mViewPosition.y, config.screenWidth, config.screenHeight));
		mWindow.setView(mView);

		textHealth.setPosition(mViewPosition.x, mViewPosition.y);
		//textMana.setPosition(mViewPosition.x, mViewPosition.y + textMana.getCharacterSize());
		textEnemyCount.setPosition(mViewPosition.x, mViewPosition.y + textEnemyCount.getCharacterSize() * 2);
		textPlayerCoordinates.setPosition(mViewPosition.x, mViewPosition.y + textPlayerCoordinates.getCharacterSize() * 3);
		textMouseCoordinates.setPosition(mViewPosition.x, mViewPosition.y + textPlayerCoordinates.getCharacterSize() * 6);


		//Clearing the screen.
		mWindow.clear(sf::Color::White);

		//Rendering level map tiles.
		for(int i = 0; i < world.getMapHeight(); ++i) {
			for(int j = 0; j < world.getMapWidth(); ++j) {
				
				switch(levelMap[i][j]) {
					case 'B': rectangle.setFillColor(sf::Color::Black); break;
					case '0': rectangle.setFillColor(sf::Color::Green); break;
					default: continue;
				}

				rectangle.setPosition(config.tileSize * j, config.tileSize * i);
				mWindow.draw(rectangle);
				//tile.setPosition(tileSize * j - offsetX, tileSize * i - offsetY);
				//mWindow.draw(tile);

			}
		}


		//Rendering all the objects.
		for(int i = 0; i < enemies.size(); ++i) 
			enemies[i].getGraphics()->draw(mWindow);

		for(int i = 0; i < drops.size(); ++i)
			mWindow.draw(drops[i].getSprite());


		player->getGraphics()->draw(mWindow);
		mWindow.draw(textHealth);
		mWindow.draw(textMana);
		mWindow.draw(textEnemyCount);
		mWindow.draw(textPlayerCoordinates);
		mWindow.draw(textMouseCoordinates);
		mWindow.display();

		//Deleting objects marked for removal.
		for(int i = 0; i < drops.size(); ++i)
			if(drops[i].isMarkedForRemoval()) {
				drops.erase(drops.begin() + i );
				--i;
			}

	}

	return 0;

}
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
struct attributes {

     int							strength;				//Carry weight, melee damage. 
     int							endurance;				//HP, resistances.
     int							agility;				//Speed, ranged damage.
     int							intelligence;			//MP, magic damage.
     int							luck;					//Dodge chance, critical damage chance.

};

struct resistances {

     int							poison;
     int							paralize;
     int							lightning;
     int							fire;
     int							ice;
     int							earth;

};

struct healthBar {

	sf::Sprite						mSprite;
	int								mWidth;
	int								mHeight;

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



//CLASS PROTOTYPES.
class World;
class Player;


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
	virtual void					update(Player&) = 0;

};

class PhysicsComponent {
public:

	virtual							~PhysicsComponent();
	virtual void					update(Player&, World&, int, float) = 0;

};

//Player.
class PlayerInputComponent : public InputComponent {
public:

	virtual void					update(Player&);

};

class PlayerPhysicsComponent : public PhysicsComponent {
public:

	virtual void					update(Player&, World&, int, float);

};


//   _____ _                         
//  / ____| |                        
// | |    | | __ _ ___ ___  ___  ___ 
// | |    | |/ _` / __/ __|/ _ \/ __|
// | |____| | (_| \__ \__ \  __/\__ \
//  \_____|_|\__,_|___/___/\___||___/
//       


//Player.hpp
class Player {
public:

									Player(sf::Texture&, sf::Texture&, int, int, sf::Font&, InputComponent*, PhysicsComponent*);
	void							update(float, std::vector<std::vector<int>>, struct config*, World&);
	void							recieveMessage(int);
	void							takeDamage(float);
	void							heal(float);

	sf::FloatRect					getRect();
	sf::Sprite						getSprite();
	sf::Sprite						getHpSprite();
	sf::Text						getTextName();
	float							getSpeed();
	sf::Vector2f					getMovement();
	float							getHP();
	float							getMaxHP();
	float							getMP();
	float							getMaxMP();

	void							setSpeed(float);
	void							setMovement(sf::Vector2f);
	void							setRect(sf::FloatRect);

private:

	InputComponent*					mInput;
	PhysicsComponent*				mPhysics;

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
	float							mMaxHP;
	float							mMP;
	float							mMaxMP;

	std::vector<int>				mMessages;	
	
};

//Enemy.hpp
class Enemy {
public:

									Enemy(sf::Texture&, int, int, sf::Font&);

	void							update(float, std::vector<std::vector<int>>, struct config*, World&, Player&);
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
	float							mAttackRange;

	sf::String						mName;
	sf::Text						mTextName;

	std::vector<int>				mMessages;

};

//DropItem.hpp
class DropItem {
public:

									DropItem(sf::Texture&, int, int, int);
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

//World.hpp
class World {
public:

									World();
									World(int, int);
	void							resolveMapCollision(sf::FloatRect&, sf::Vector2f, int, int);
	void							loadLevelMap(std::string);
	void							deleteLevelMap();

	std::vector<Enemy>&				getEnemies();
	std::vector<DropItem>&			getDrops();

	std::vector<std::vector<int>>	getLevelMap();
	int								getMapHeight();
	int								getMapWidth();

private:

	std::vector<std::vector<int>>	mLevelMap;
	std::vector<std::vector<bool>>	mCollisionMap;
	int								mMapHeight;
	int								mMapWidth;

	//Player							mPlayer;
	std::vector<Enemy>				mEnemies;
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

private:

	sf::RenderWindow				mWindow;
	int								mScreenWidth;					
	int								mScreenHeight;

	float							mInvincibilityTime;
	sf::Clock						mGameClock;
	sf::Clock						mInvincibilityClock;
	sf::Clock						mSpawnClock;
	sf::Time						mTimePerFrame;

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


//InputComponent.cpp
InputComponent::~InputComponent() {}

//PhysicsComponent.cpp
PhysicsComponent::~PhysicsComponent() {}

//PlayerInputComponent.cpp
void PlayerInputComponent::update(Player& player) {

	sf::Vector2f movement = player.getMovement();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))	movement.y -= player.getSpeed();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	movement.y += player.getSpeed();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	movement.x -= player.getSpeed();
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	movement.x += player.getSpeed();

	player.setMovement(movement);

}

//PlayerPhysicsComponent.cpp
void PlayerPhysicsComponent::update(Player& player, World& world, int tileSize, float deltaTime) {
	
	sf::FloatRect rect = player.getRect();
	sf::Vector2f movement = player.getMovement();

	rect.left += movement.x * deltaTime;
	world.resolveMapCollision(rect, movement, 0, tileSize);
	rect.top += movement.y * deltaTime;
	world.resolveMapCollision(rect, movement, 1, tileSize);

	//movement.x = 0;
	//movement.y = 0;

	//player.setMovement(movement);
	player.setRect(rect);
	
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
World::World() {

	mMapHeight = 0;
	mMapWidth = 0;

}

World::World(int mapHeight, int mapWidth) {

	mMapHeight = mapHeight;
	mMapWidth = mapWidth;

}

void World::resolveMapCollision(sf::FloatRect& rect, sf::Vector2f movement, int direction, int tileSize) {

	for(int i = rect.top / tileSize; i < (rect.top + rect.height) / tileSize; ++i)
		for(int j = rect.left / tileSize; j < (rect.left + rect.width) / tileSize; ++j) {

			if(mLevelMap[i][j] == 'B') {
					if((movement.x > 0) && (direction == 0)) rect.left = j * tileSize - rect.width;
					if((movement.x < 0) && (direction == 0)) rect.left = j * tileSize + tileSize;
					if((movement.y > 0) && (direction == 1)) rect.top = i * tileSize - rect.height;
					if((movement.y < 0) && (direction == 1)) rect.top = i * tileSize + tileSize;
			}

		}

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

std::vector<Enemy>& World::getEnemies() {
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

	std::vector<Enemy> enemies = mWorld.getEnemies();
	std::vector<DropItem> drops = mWorld.getDrops();
	std::vector<std::vector<int>> levelMap = mWorld.getLevelMap();


}

void Game::render() {

}



//
//Player.cpp
//
Player::Player(sf::Texture& texture, sf::Texture& hpImage, int x, int y, sf::Font& font, InputComponent* input, PhysicsComponent* physics)
:mInput(input),
mPhysics(physics) {

	mSprite.setTexture(texture);
	mRect = sf::FloatRect(x, y, 120, 110);					//Character x, y, width, height.
		
	mName = "Player 1";
	mTextName.setString(mName);
	mTextName.setFont(font);
	mTextName.setCharacterSize(30);
	mTextName.setStyle(sf::Text::Bold);
	mTextName.setColor(sf::Color::Red);
		
	mHpSprite.setTexture(hpImage);
	mHpSprite.setTextureRect(sf::IntRect(0, 0, 100, 10));

	mSprite.setTextureRect(sf::IntRect(0, 15, 120, 110));
	
	mSpeed = 0.1;
	mMovement.x = 0;
	mMovement.y = 0;

	mCurrentFrame = 0;
	mAnimationSpeed = 0.005;

	mIsAlive = true;
	mHP = 150;
	mMaxHP = 150;
	mMP = 100;
	mMaxMP = 100;

}

void Player::update(float deltaTime, std::vector<std::vector<int>> levelMap, struct config* config, World& world) {

	mInput->update(*this);
	mPhysics->update(*this, world, config->tileSize, deltaTime);

	//Player animation.
	mCurrentFrame += mAnimationSpeed * deltaTime;
	if(mCurrentFrame > mFrameCount) mCurrentFrame -= mFrameCount;

	if(mMovement.x > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 925, mRect.width, mRect.height));
	if(mMovement.x < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 667, mRect.width, mRect.height));
	if(mMovement.y > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 535, mRect.width, mRect.height));
	if(mMovement.y < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 787, mRect.width, mRect.height));

	mSprite.setPosition(mRect.left, mRect.top);

	//HP bar.
	float hpPercentage = mHP / mMaxHP;
	if(hpPercentage >= 0.6)
		mHpSprite.setColor(sf::Color::Green);
	else if((hpPercentage >= 0.35) && (hpPercentage < 0.6))
		mHpSprite.setColor(sf::Color::Yellow);
	else if(hpPercentage < 0.35)
		mHpSprite.setColor(sf::Color::Red);

	mHpSprite.setTextureRect(sf::IntRect(100 * (1 - hpPercentage), 0, 100, 10));
	mHpSprite.setPosition(mRect.left, mRect.top + mRect.height);
	mTextName.setPosition(mRect.left, mRect.top - mTextName.getCharacterSize());

	//Stopping the player.
	mMovement.x = 0;
	mMovement.y = 0;

}

void Player::recieveMessage(int message) {
	mMessages.push_back(message);
}

void Player::takeDamage(float damage) {
	mHP -= damage;
}

void Player::heal(float hp) {
	if(mHP + hp > mMaxHP)
		mHP += mMaxHP - mHP;
	else
		mHP += hp;
}

sf::FloatRect Player::getRect() {
	return mRect;
}

sf::Sprite Player::getSprite() {
	return mSprite;
}

sf::Sprite Player::getHpSprite() {
	return mHpSprite;
}

sf::Text Player::getTextName() {
	return mTextName;
}

float Player::getSpeed() {
	return mSpeed;
}

sf::Vector2f Player::getMovement() {
	return mMovement;
}

float Player::getHP() {
	return mHP;
}

float Player::getMaxHP() {
	return mMaxHP;
}

float Player::getMP() {
	return mMP;
}

float Player::getMaxMP() {
	return mMaxMP;
}

void Player::setSpeed(float speed) {
	mSpeed = speed;
}

void Player::setMovement(sf::Vector2f movement) {
	mMovement = movement;
}

void Player::setRect(sf::FloatRect rect) {
	mRect = rect;
}



//
//Enemy.cpp
//
Enemy::Enemy(sf::Texture& texture, int x, int y, sf::Font& font) {
	mSprite.setTexture(texture);
	mRect = sf::FloatRect(x, y, 120, 110);					//Character x, y, width, height.

	mName = "Enemy";
	mTextName.setString(mName);
	mTextName.setFont(font);
	mTextName.setCharacterSize(30);
	mTextName.setStyle(sf::Text::Bold);
	mTextName.setColor(sf::Color::Color(192, 192, 192));

	mSprite.setTextureRect(sf::IntRect(0, 15, 120, 110));

	mDirection = rand() % 4;
	mSpeed = 0.05;

	mCurrentFrame = 0;
	mAnimationSpeed = mSpeed * 0.05;

	mIsAlive = true;
	mHP = 100;

	mDamage = 30;
	mAttackRange = 80;
}

void Enemy::update(float deltaTime, std::vector<std::vector<int>> levelMap, struct config* config, World& world, Player& player) {

	float deltaX = (player.getRect().left + player.getRect().width / 2) - (mRect.left + mRect.width / 2);
	float deltaY = (player.getRect().top + player.getRect().height / 2) - (mRect.top + mRect.height / 2);
	float distance = std::sqrt( deltaX * deltaX + deltaY * deltaY);

	//New awesome super-AI.
	if(distance > mAttackRange) {
		if(player.getRect().left > mRect.left)
			mMovement.x = mSpeed;
		else
			mMovement.x = -mSpeed;

		if(player.getRect().top > mRect.top)
			mMovement.y = mSpeed;
		else
			mMovement.y = -mSpeed;
	} else {
		mMovement.x = 0;
		mMovement.y = 0;
		if(isReadyToAttack())
			dealDamage(player);
	}

	/*
	//Stupid random AI.
	if(mMovementClock.getElapsedTime().asSeconds() > 5) {
		mDirection = rand() % 4;
		mMovementClock.restart();
	}

	if(mDirection == 0) {mMovement.x = 0;		mMovement.y = -mSpeed;}
	if(mDirection == 1) {mMovement.x = mSpeed;	mMovement.y = 0;}
	if(mDirection == 2) {mMovement.x = 0;		mMovement.y = mSpeed;}
	if(mDirection == 3) {mMovement.x = -mSpeed;	mMovement.y = 0;}
	*/

	mRect.left += mMovement.x * deltaTime;
	//collision(levelMap, config);
	world.resolveMapCollision(mRect, mMovement, 0, config->tileSize);
	mRect.top += mMovement.y * deltaTime;
	//collision(levelMap, config);
	world.resolveMapCollision(mRect, mMovement, 1, config->tileSize);

	//Enemy animation.
	mCurrentFrame += mAnimationSpeed * deltaTime;
	if(mCurrentFrame > mFrameCount) mCurrentFrame -= mFrameCount;

	if(std::abs(deltaX) > std::abs(deltaY)) {
		if(mMovement.x > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 925, mRect.width, mRect.height));
		if(mMovement.x < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 667, mRect.width, mRect.height));
	} else {
		if(mMovement.y > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 535, mRect.width, mRect.height));
		if(mMovement.y < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 787, mRect.width, mRect.height));
	}

	mSprite.setPosition(mRect.left, mRect.top);
	mTextName.setPosition(mRect.left, mRect.top - mTextName.getCharacterSize());

}

void Enemy::collision(std::vector<std::vector<int>> levelMap, struct config* config) {
	for(int i = mRect.top / config->tileSize; i < (mRect.top + mRect.height) / config->tileSize; ++i)
		for(int j = mRect.left / config->tileSize; j < (mRect.left + mRect.width) / config->tileSize; ++j) {
				
			if(levelMap[i][j] == 'B') {
				if(mMovement.x > 0) mRect.left = j * config->tileSize - mRect.width;
				if(mMovement.x < 0) mRect.left = j * config->tileSize + config->tileSize;
				if(mMovement.y > 0) mRect.top = i * config->tileSize - mRect.height;
				if(mMovement.y < 0) mRect.top = i * config->tileSize + config->tileSize;
				int temp = rand() % 4;
				if(temp != mDirection) mDirection = temp;
				//direction = rand() % 4;		//???
			}
						
		}
}

void Enemy::dealDamage(Player& player) {
	float damage = rand() % int(mDamage);
	player.takeDamage(damage);
	mDamageClock.restart();
}

void detectTargets(World& world) {
	
}
	
//Inner communication between objects (between the components in the future).
void Enemy::getMessage(int message) {
	mMessages.push_back(message);
}

sf::FloatRect Enemy::getRect() {
	return mRect;
}

sf::Sprite Enemy::getSprite() {
	return mSprite;
}

sf::Text Enemy::getTextName() {
	return mTextName;
}

bool Enemy::isReadyToAttack() {
	if(mDamageClock.getElapsedTime().asSeconds() > mSpeed * 10)
		return true;
	else
		return false;
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

void DropItem::action(Player& player) {
	player.heal(mEffectValue);
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


	//World.
	std::string levelMapName = "./levels/level1.txt";

	World world;
	world.loadLevelMap(levelMapName);
	std::vector<std::vector<int>> levelMap = world.getLevelMap();
	

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



	//Game objects.
	std::vector<DropItem> drops = world.getDrops();
	std::vector<Enemy> enemies = world.getEnemies();

	Player player(playerTexture, hpBar, config.playerStartingX, config.playerStartingY, font, new PlayerInputComponent(), new PlayerPhysicsComponent);


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
			if(event.type == sf::Event::Closed)
				mWindow.close();
		}

		//Updating all objects.
		player.update(time, levelMap, &config, world);
		for(int i = 0; i < enemies.size(); ++i)	
			enemies[i].update(time, levelMap, &config, world, player);
		for(int i = 0; i < drops.size(); ++i)
			drops[i].update(time);


		//Resolving collisions. Should move to World class methods.
		//for(int i = 0; i < enemies.size(); ++i)
		//	if((player.getRect().intersects(enemies[i].getRect())) && (enemies[i].isReadyToAttack()))
		//		enemies[i].dealDamage(player);

		for(int i = 0; i < drops.size(); ++i)
			if((player.getRect().intersects(drops[i].getRect())) && (~drops[i].isMarkedForRemoval()) && (player.getHP() != player.getMaxHP()))
				drops[i].action(player);
	
		//Spawning elves.
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::G)) && (spawnClock.getElapsedTime().asSeconds() > 0.25)) {
			enemies.push_back(*(new Enemy(enemyTexture, 240, 240, font)));
			spawnClock.restart();
		}

		//Spawning health potions.
		if((sf::Keyboard::isKeyPressed(sf::Keyboard::H)) && (spawnClock.getElapsedTime().asSeconds() > 0.25)) {
			drops.push_back(*(new DropItem(healthPotionTexture, 40, player.getRect().left + (rand() % 2 - 1) * (rand() % 100), player.getRect().top + (rand() % 2 - 1) * (rand() % 100))));
			spawnClock.restart();
		}


		//HUD.
		std::ostringstream hudHealth;
		std::ostringstream hudMana;
		std::ostringstream hudEnemyCount;
		std::ostringstream hudPlayerCoordinates;
		std::ostringstream hudMouseCoordinates;

		hudHealth << player.getHP();
		hudMana << player.getMP();
		hudEnemyCount << "Number of enemies: " << enemies.size();
		hudPlayerCoordinates << "X: " << player.getRect().left + config.tileSize / 2 << '\n' << "Y: " << player.getRect().top + config.tileSize / 2;
		hudMouseCoordinates << "X: " << sf::Mouse::getPosition(mWindow).x << '\n' << "Y: " << sf::Mouse::getPosition(mWindow).y;

		textMana.setString(hudMana.str());
		textHealth.setString(hudHealth.str());
		textEnemyCount.setString(hudEnemyCount.str());
		textPlayerCoordinates.setString(hudPlayerCoordinates.str());
		textMouseCoordinates.setString(hudMouseCoordinates.str());


		//View.
		mViewPosition.x = player.getRect().left + config.tileSize / 2 - config.screenWidth / 2;
		mViewPosition.y = player.getRect().top + config.tileSize / 2 - config.screenHeight / 2;
		
		if(mViewPosition.x < 0)																	mViewPosition.x = 0;
		if(mViewPosition.x > world.getMapWidth() * config.tileSize - config.screenWidth)		mViewPosition.x = world.getMapWidth() * config.tileSize - config.screenWidth;
		if(mViewPosition.y < 0)																	mViewPosition.y = 0;
		if(mViewPosition.y > world.getMapHeight() * config.tileSize - config.screenHeight)		mViewPosition.y = world.getMapHeight() * config.tileSize - config.screenHeight;

		mView.reset(sf::FloatRect(mViewPosition.x, mViewPosition.y, config.screenWidth, config.screenHeight));
		mWindow.setView(mView);

		textHealth.setPosition(mViewPosition.x, mViewPosition.y);
		textMana.setPosition(mViewPosition.x, mViewPosition.y + textMana.getCharacterSize());
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
		for(int i = 0; i < enemies.size(); ++i) {
			mWindow.draw(enemies[i].getSprite());
			mWindow.draw(enemies[i].getTextName());
		}

		for(int i = 0; i < drops.size(); ++i)
			mWindow.draw(drops[i].getSprite());

		mWindow.draw(player.getSprite());
		mWindow.draw(player.getHpSprite());
		mWindow.draw(player.getTextName());

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
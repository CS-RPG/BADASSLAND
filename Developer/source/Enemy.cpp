#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <Player.hpp>

//Enemy.cpp
Enemy::Enemy(sf::Texture& texture, int x, int y, sf::Font& font) {
	mSprite.setTexture(texture);
	mRect = sf::FloatRect(x, y, 120, 110);					//Character x, y, width, height.

	mName = "Enemy";
	mTextName.setString(mName);
	mTextName.setFont(font);
	mTextName.setCharacterSize(30);
	mTextName.setStyle(sf::Text::Bold);
	mTextName.setColor(sf::Color::Magenta);

	mSprite.setTextureRect(sf::IntRect(0, 15, 120, 110));

	mDirection = rand() % 4;
	mSpeed = 0.05;

	mCurrentFrame = 0;
	mAnimationSpeed = mSpeed * 0.05;

	mIsAlive = true;
	mHP = 100;

	mDamage = 30;
}

void Enemy::update(float deltaTime, std::vector<std::vector<int>> levelMap, struct config* config, World& world) {

	if(mMovementClock.getElapsedTime().asSeconds() > 5) {
		mDirection = rand() % 4;
		mMovementClock.restart();
	}

	if(mDirection == 0) {mMovement.x = 0;		mMovement.y = -mSpeed;}
	if(mDirection == 1) {mMovement.x = mSpeed;	mMovement.y = 0;}
	if(mDirection == 2) {mMovement.x = 0;		mMovement.y = mSpeed;}
	if(mDirection == 3) {mMovement.x = -mSpeed;	mMovement.y = 0;}

	mRect.left += mMovement.x * deltaTime;
	collision(levelMap, config);
	//world.resolveCollision(mRect, mMovement, 0, config->tileSize);
	mRect.top += mMovement.y * deltaTime;
	collision(levelMap, config);
	//world.resolveCollision(mRect, mMovement, 1, config->tileSize);

	//Enemy animation.
	mCurrentFrame += mAnimationSpeed * deltaTime;
	if(mCurrentFrame > mFrameCount) mCurrentFrame -= mFrameCount;

	if(mMovement.x > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 925, mRect.width, mRect.height));
	if(mMovement.x < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 667, mRect.width, mRect.height));
	if(mMovement.y > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 535, mRect.width, mRect.height));
	if(mMovement.y < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 787, mRect.width, mRect.height));

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
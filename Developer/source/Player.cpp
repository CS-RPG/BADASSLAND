#include <SFML\Graphics.hpp>
#include <InputComponent.hpp>
#include <DataTypes.hpp>
#include <World.hpp>

Player::Player(sf::Texture& texture, sf::Texture& hpImage, int x, int y, sf::Font& font) {

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
	mHP = 100;
	mMaxHp = 100;
	mMP = 100;
}

void Player::update(float deltaTime, std::vector<std::vector<int>> levelMap, struct config* config, World& world) {

	//Player controls (testing).
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))	mMovement.y -= mSpeed;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	mMovement.y += mSpeed;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	mMovement.x -= mSpeed;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	mMovement.x += mSpeed;
	//mInput.update(*this);


	//Movement and resolving colisions (via the World class).
	mRect.left += mMovement.x * deltaTime;
	world.resolveCollision(mRect, mMovement, 0, config->tileSize);
	mRect.top += mMovement.y * deltaTime;
	world.resolveCollision(mRect, mMovement, 1, config->tileSize);

	//Player animation.
	mCurrentFrame += mAnimationSpeed * deltaTime;
	if(mCurrentFrame > mFrameCount) mCurrentFrame -= mFrameCount;

	if(mMovement.x > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 925, mRect.width, mRect.height));
	if(mMovement.x < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 667, mRect.width, mRect.height));
	if(mMovement.y > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 535, mRect.width, mRect.height));
	if(mMovement.y < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 787, mRect.width, mRect.height));

	mSprite.setPosition(mRect.left, mRect.top);

	//HP.
	if(mHP / mMaxHp >= 0.6)
		mHpSprite.setColor(sf::Color::Green);
	else if((mHP / mMaxHp >= 0.35) && (mHP / mMaxHp < 0.6))
		mHpSprite.setColor(sf::Color::Yellow);
	else if(mHP / mMaxHp < 0.35)
		mHpSprite.setColor(sf::Color::Red);

	mHpSprite.setTextureRect(sf::IntRect(100 * (1 - mHP / mMaxHp), 0, 100, 10));
	mHpSprite.setPosition(mRect.left, mRect.top + mRect.height);
	mTextName.setPosition(mRect.left, mRect.top - mTextName.getCharacterSize());						//-

	//Stopping the player.
	mMovement.x = 0;
	mMovement.y = 0;

}

void Player::getMessage(int message) {
	mMessages.push_back(message);
}

void Player::takeDamage(float damage) {
	mHP -= damage;
}

void Player::heal(float hp) {
	mHP += hp;
}

//On-screen stats.
//sf::Text should be used to display stats.
void Player::showStats(std::ostringstream* hudHealth, std::ostringstream* hudMana) {
	*hudHealth << mHP;
	*hudMana << mMP;
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

void Player::setSpeed(float speed) {
	mSpeed = speed;
}

void Player::setMovement(sf::Vector2f movement) {
	mMovement = movement;
}

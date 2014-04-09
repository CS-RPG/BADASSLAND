//HumanoidGraphicsComponent.cpp
#include <HumanoidGraphicsComponent.hpp>

extern TextureHolder		gTextureHolder;
extern sf::Font				gFont;
extern int					gFontSize;


//============HumanoidGraphicsComponent=====
//
HumanoidGraphicsComponent::HumanoidGraphicsComponent(Textures::ID textureID) {

	//===============Sprites=======================
	mCurrentSprite.setTexture(gTextureHolder.get(textureID));
	mSpriteStill.setTexture(gTextureHolder.get(textureID));
	mSpriteUp.setTexture(gTextureHolder.get(textureID));
	mSpriteDown.setTexture(gTextureHolder.get(textureID));
	mSpriteLeft.setTexture(gTextureHolder.get(textureID));
	mSpriteRight.setTexture(gTextureHolder.get(textureID));

	mSpriteStill.setTextureRect(sf::IntRect(0, 15, 120, 120));
	mSpriteUp.setTextureRect(sf::IntRect(0, 787, 0, 0));
	mSpriteDown.setTextureRect(sf::IntRect(0, 535, 0, 0));
	mSpriteLeft.setTextureRect(sf::IntRect(0, 667, 0, 0));
	mSpriteRight.setTextureRect(sf::IntRect(0, 925, 0, 0));

	//===============HP-Bar========================
	mHPBarSprite.setTexture(gTextureHolder.get(Textures::HP_Bar));
	mHPBarSprite.setTextureRect(sf::IntRect(0, 0, 100, 10));

	//===============Text==========================
	mText.setFont(gFont);
	mText.setCharacterSize(gFontSize);
	mText.setStyle(sf::Text::Bold);
	mText.setColor(sf::Color::Red);

	mCurrentSprite = mSpriteStill;

	mFrameCount = 10;		//!!!Replace with global variable.
	mCurrentFrame= 0;

}

void HumanoidGraphicsComponent::update(GameObject& player, float deltaTime) {

	//===============Animation=====================
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

	//===============HP-Bar========================
	float hpPercentage = player.getCombat()->getHP() / player.getCombat()->getMaxHP();
	if(hpPercentage >= 0.6)
		mHPBarSprite.setColor(sf::Color::Green);
	else if((hpPercentage >= 0.35) && (hpPercentage < 0.6))
		mHPBarSprite.setColor(sf::Color::Yellow);
	else if(hpPercentage < 0.35)
		mHPBarSprite.setColor(sf::Color::Red);

	mHPBarSprite.setTextureRect(sf::IntRect(100 * (1 - hpPercentage), 0, 100, 10));
	mHPBarSprite.setPosition(player.getPhysics()->getRect().left, player.getPhysics()->getRect().top + player.getPhysics()->getRect().height);

	//===============Text==========================
	mText.setString(player.getSocial()->getName());
	mText.setPosition(player.getPhysics()->getRect().left, player.getPhysics()->getRect().top - mText.getCharacterSize());

	mCurrentSprite.setPosition(player.getPhysics()->getRect().left, player.getPhysics()->getRect().top);

}

void HumanoidGraphicsComponent::draw(sf::RenderWindow& window) {
	
	window.draw(mCurrentSprite);
	window.draw(mHPBarSprite);
	window.draw(mText);

}
//
//==========================================
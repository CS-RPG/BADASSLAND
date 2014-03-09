//HumanoidGraphicsComponent.hpp
#ifndef _HumanoidGraphicsComponent_hpp_included_
#define _HumanoidGraphicsComponent_hpp_included_

//Prototypes.
class GameObject;


//============HumanoidGraphicsComponent=====
//
class HumanoidGraphicsComponent : public GraphicsComponent {
public:

									HumanoidGraphicsComponent(sf::Texture*, sf::Texture*, sf::Font*);

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
//
//==========================================


#endif
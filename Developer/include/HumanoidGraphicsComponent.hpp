//HumanoidGraphicsComponent.hpp
#ifndef _HumanoidGraphicsComponent_hpp_included_
#define _HumanoidGraphicsComponent_hpp_included_

#include <_includes_badass.hpp>
#include <_includes_system.hpp>

//============HumanoidGraphicsComponent=====
//
class HumanoidGraphicsComponent : public GraphicsComponent {
public:

									HumanoidGraphicsComponent(Textures::ID textureID);

	virtual void					update(GameObject&, float);
	virtual void					draw(sf::RenderWindow&);

private:

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
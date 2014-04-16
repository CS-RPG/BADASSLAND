//InputComponent.hpp

#ifndef _InputComponent_hpp_included_
#define _InputComponent_hpp_included_

//Prototypes.
class World;
class GameObject;

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>



//============InputComponent================
//
class InputComponent {
public:

	virtual							~InputComponent();
	virtual void					update(GameObject&, World&) = 0;
	void							captureTarget(GameObject&, World&);
	void							moveToTarget(GameObject& object, GameObject& target);
	bool							createPath(sf::Vector2i source, sf::Vector2i destination, World& world);

	int								getBadDirection();
	bool							isTargeting();
	GameObject*						getTarget();
	std::vector<sf::Vector2i>&		getPath();

	void							setBadDirection(int);
	void							setTargeting(bool);
	void							setTarget(GameObject*);

private:

	std::vector<sf::Vector2i>		mPath;

	int								mBadDirection;
	bool							mIsTargeting;
	GameObject*						mTarget;
	
};
//
//==========================================



#endif
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

	int								getBadDirection();
	bool							isTargeting();
	GameObject*						getTarget();

	void							setBadDirection(int);
	void							setTargeting(bool);
	void							setTarget(GameObject*);

private:

	int								mBadDirection;
	bool							mIsTargeting;
	GameObject*						mTarget;
	
};
//
//==========================================



#endif
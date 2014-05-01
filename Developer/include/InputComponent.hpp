//InputComponent.hpp

#ifndef _InputComponent_hpp_included_
#define _InputComponent_hpp_included_

//Prototypes.
class World;
class GameObject;

#include <_includes_system.hpp>
//#include <_includes_badass.hpp>
#include <DataTypes.hpp>



//============InputComponent================
//
class InputComponent {
public:

	virtual							~InputComponent();
	virtual void					update(GameObject& object, World& world, config& config) = 0;
	void							captureTarget(GameObject&, World&);
	void							moveToTarget(GameObject& object, GameObject& target, World& world, int tileSize);
	void							moveToTile(GameObject& object, sf::Vector2i tile, int tileSize);

	void							stop(GameObject& object);
	void							moveLeft(GameObject& object);
	void							moveRight(GameObject& object);
	void							moveUp(GameObject& object);
	void							moveDown(GameObject& object);

	void							moveLeft(GameObject& object, float speed);
	void							moveRight(GameObject& object, float speed);
	void							moveUp(GameObject& object, float speed);
	void							moveDown(GameObject& object, float speed);

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
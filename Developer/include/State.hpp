//State.hpp
#ifndef _State_hpp_included_
#define _State_hpp_included_

#include <_includes_system.hpp>
#include <DataTypes.hpp>
#include <GameObject.hpp>

//Prototypes.
class StateMachine;


//============State=========================
//
class State {
public:

	//virtual							~State();

	void							onCreate();
	void							onExit();

	virtual void					update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config);
	virtual void					render(sf::RenderWindow& window, sf::View& view, config& config);
	//void							executeCommand(std::string commandID, ...);
	virtual void					handleInput(config& config);

	std::vector<GameObject>&		getGameObjects();
	std::vector<GameObject>&		getSharedObjects();

	std::vector<std::vector<int>>&	getLevelMap();
	int								getMapHeight();
	int								getMapWidth();
	StateMachine*&					getStateMachine();

	void							setMapHeight(int height);
	void							setMapWidth(int width);
	void							setStateMachine(StateMachine* stateMachine);

private:

	StateMachine*					mStateMachine;

	std::vector<std::vector<int>>	mLevelMap;
	std::vector<GameObject>			mGameObjects;
	/*static*/ std::vector<GameObject>	mSharedObjects;

	int								mMapHeight;
	int								mMapWidth;

};
//
//==========================================


#endif
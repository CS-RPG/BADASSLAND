//StateMachine.hpp
#ifndef _StateMachine_hpp_included_
#define _StateMachine_hpp_included_

#include <DataTypes.hpp>
#include <State.hpp>
#include <fstream>
#include <World.hpp>
#include <SFML/Graphics.hpp>

//============StateMachine==================
//
class StateMachine {
public:

									StateMachine();
	void							run();
	void							update(float deltaTime);
	void							render();

	void							changeState();
	void							addState();

	void							loadConfigFile(std::string fileName);


private:

	//Graphics.
	sf::RenderWindow				mWindow;
	sf::View						mView;
	float							mFPS;
	sf::Font						mFont;

	config							mConfig;

	State*							mCurrentState;
	std::map<std::string, State>	mStates;
	//Save							mSave;			

};
//
//==========================================


#endif
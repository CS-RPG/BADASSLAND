//StateMachine.hpp
#ifndef _StateMachine_hpp_included_
#define _StateMachine_hpp_included_

#include <_includes_badass.hpp>
#include <_includes_system.hpp>


namespace States {

	enum ID {

		MainMenu,
		World,
		Local,
		InGame

	};

}

//============StateMachine==================
//
class StateMachine {
public:

									StateMachine();

	void							initializeControlsMap();
	void							initializeStatesMap();

	void							run();
	void							update(float deltaTime);
	void							render();
	void							processEvents();

	void							changeState(std::string stateName);
	void							addState();

	bool							loadConfigFile(std::string fileName);


private:

	float							mDisplayAspectRatio;
	sf::Vector2u					mPreviousWindowSize;

	sf::RenderWindow*				mWindow;
	sf::View						mView;
	float							mFPS_CAP;

	config							mConfig;
	bool							mTerminateGame;
	std::map<std::string,
		sf::Keyboard::Key>			mControlsMap;

	std::map<std::string,
		States::ID>					mStatesMap;

	State*							mCurrentState;
	std::map<std::string, State>	mStates;			

};
//
//==========================================


#endif
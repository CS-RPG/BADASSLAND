//StateMachine.hpp
#ifndef _StateMachine_hpp_included_
#define _StateMachine_hpp_included_

#include <_includes_badass.hpp>
#include <_includes_system.hpp>


//============StateMachine==================
//
class StateMachine {
public:

									StateMachine();
	void							run();
	void							update(float deltaTime);
	void							render();
	void							processEvents();

	void							changeState();
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

	State*							mCurrentState;
	std::map<std::string, State>	mStates;			

};
//
//==========================================


#endif
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

	void							loadConfigFile(std::string fileName);


private:

	sf::RenderWindow*				mWindow;
	sf::View						mView;
	float							mFPS_CAP;
	sf::Font						mFont;

	config							mConfig;

	State*							mCurrentState;
	std::map<std::string, State>	mStates;
	//Save							mSave;			

};
//
//==========================================


#endif
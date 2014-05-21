//State.hpp
#ifndef _State_hpp_included_
#define _State_hpp_included_

#include <_includes_system.hpp>
#include <DataTypes.hpp>
#include <GameObject.hpp>

//Prototypes.
class StateMachine;
class Button;


//============State=========================
class State {
public:

	typedef void(State::*script)(std::string);

	//virtual							~State();

	void							onCreate();
	void							onExit();

	virtual void					initializeScripts();

	virtual void					update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config);
	virtual void					updateButtons(sf::RenderWindow& window, sf::View& view, config& config, State& state);

	virtual void					render(sf::RenderWindow& window, sf::View& view, config& config);

	virtual void					handleInput(config& config);

	virtual void					runScript(std::string script, std::string args);
	virtual void					changeState(std::string args);


	std::vector<GameObject>&		getGameObjects();
	std::vector<GameObject>&		getSharedObjects();

	StateMachine*&					getStateMachine();
	sf::Clock&						getButtonClock();
	std::map<std::string, script>&	getScripts();
	std::vector<Button>&			getButtons();

	void							setStateMachine(StateMachine* stateMachine);

private:

	typedef void(State::*script)(std::string);

	StateMachine*					mStateMachine;

	std::vector<GameObject>			mGameObjects;
	/*static*/ std::vector<GameObject>	mSharedObjects;

	std::map<std::string, script>	mScripts;

	sf::Clock						mButtonClock;

	std::vector<Button>				mButtons;

};

#endif

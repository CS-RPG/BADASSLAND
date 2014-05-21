//MainMenu.hpp
#ifndef _MainMenu_hpp_included_
#define _MainMenu_hpp_included_

#include <_includes_system.hpp>
#include <_includes_badass.hpp>


class MainMenu : public State {
public:

	typedef void(State::*script)(std::string);

									MainMenu(StateMachine* game, sf::RenderWindow& window, sf::View& view, config& config);

	virtual void					initializeScripts();

	virtual void					update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config);
	virtual void					updateButtons(sf::RenderWindow& window, sf::View& view, config& config, State& state);

	virtual void					render(sf::RenderWindow& window, sf::View& view, config& config);
	virtual void					handleInput(config& config);

	virtual void					changeState(std::string args);

private:

	typedef void(State::*script)(std::string);

};


#endif

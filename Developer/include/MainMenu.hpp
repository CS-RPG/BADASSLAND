//MainMenu.hpp
#ifndef _MainMenu_hpp_included_
#define _MainMenu_hpp_included_

#include <_includes_system.hpp>
#include <_includes_badass.hpp>

//class Button;

class MainMenu : public State {
public:

									MainMenu();

	virtual void					update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config);
	virtual void					render(sf::RenderWindow& window, sf::View& view, config& config);

	//std::vector<Button>&			getButtons();


private:


	//std::vector<Button>				mButtons;


};


#endif
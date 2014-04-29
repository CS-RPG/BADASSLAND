//Button.hpp
#ifndef _Button_hpp_
#define _Button_hpp_

#include <_includes_badass.hpp>
#include <_includes_system.hpp>


//============BUTTON========================
class Button {
public:

									Button();

	void							update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config);
	void							render(sf::RenderWindow& window, sf::View& view, config& config);


private:

	sf::Text						mLabel;

	sf::Sprite						mStatic;
	sf::Sprite						mOver;
	sf::Sprite						mPress;


};
//==========================================


#endif
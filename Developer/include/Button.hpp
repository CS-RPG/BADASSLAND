//Button.hpp
#ifndef _Button_hpp_
#define _Button_hpp_

#include <_includes_badass.hpp>
#include <_includes_system.hpp>


typedef void(State::*script)(std::string);


//============BUTTON========================
class Button {
public:

									Button(sf::FloatRect rect, std::string onClick, std::string onClickArgs);

	void							update(sf::RenderWindow& window, sf::View& view, config& config, State& state);
	void							render(sf::RenderWindow& window, sf::View& view, config& config);

	//void							setOnClick(std::string onClick, std::string onClickArgs);

private:

	std::string						mOnClick;
	std::string						mOnClickArgs;

	sf::Text						mLabel;

	sf::Texture						mTexture;
	
	sf::Sprite						mCurrent;
	sf::Sprite						mStatic;
	sf::Sprite						mOver;
	sf::Sprite						mPress;

	sf::FloatRect					mRect;

};

#endif

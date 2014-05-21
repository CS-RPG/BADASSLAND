//Button.cpp
#include <Button.hpp>


Button::Button(sf::FloatRect rect, std::string onClick, std::string onClickArgs) {

	mTexture.loadFromFile("./textures/button.png");

	mStatic.setTexture(mTexture);
	mStatic.setTextureRect(sf::IntRect(0, 0, 200, 100));	//HARDCODE.

	mRect = rect;

	mOnClick = onClick;
	mOnClickArgs = onClickArgs;

}

void Button::update(sf::RenderWindow& window, sf::View& view, config& config, State& state) {

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

		sf::FloatRect rect;
		rect.left = sf::Mouse::getPosition(window).x;
		rect.top = sf::Mouse::getPosition(window).y;
		rect.width = 1;
		rect.height = 1;

		if(rect.intersects(mRect))
			state.runScript(mOnClick, mOnClickArgs);

	}

}

void Button::render(sf::RenderWindow& window, sf::View& view, config& config) {

	window.draw(mStatic);

}

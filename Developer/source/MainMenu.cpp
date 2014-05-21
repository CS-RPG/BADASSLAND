//MainMenu.cpp
#include <MainMenu.hpp>

extern TextureHolder		gTextureHolder;
extern sf::Font				gFont;
extern int					gFontSize;

MainMenu::MainMenu(StateMachine* game, sf::RenderWindow& window, sf::View& view, config& config) {

	setStateMachine(game);
	initializeScripts();

	view.reset(sf::FloatRect(0, 0, config.screenWidth, config.screenHeight));
	window.setView(view);

	getButtons().push_back(* (new Button(sf::FloatRect(0, 0, 200, 100), "changeState", "World " + config.levelMapName)));	//HARDCODE.

}

void MainMenu::initializeScripts() {

	getScripts().insert(std::make_pair("changeState", &State::changeState));

}

void MainMenu::update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config) {

	updateButtons(window, view, config, *this);
	
	handleInput(config);

}

void MainMenu::updateButtons(sf::RenderWindow& window, sf::View& view, config& config, State& state) {
	
	for(int i = 0; i < getButtons().size(); ++i)
		getButtons()[i].update(window, view, config, *this);

}

void MainMenu::render(sf::RenderWindow& window, sf::View& view, config& config) {

	for(int i = 0; i < getButtons().size(); ++i)
		getButtons()[i].render(window, view, config);

}

void MainMenu::handleInput(config& config) {

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && getButtonClock().getElapsedTime().asSeconds() > config.spawnDelay) {
		
		getStateMachine()->changeState("World " + config.levelMapName);
		getButtonClock().restart();

	}

}

void MainMenu::changeState(std::string args) {
	getStateMachine()->changeState(args);
}

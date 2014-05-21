//State.cpp
#include <State.hpp>

typedef void(State::*script)(std::string);


//============State=========================
void State::initializeScripts() {

}

void State::update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config) {

}

void State::updateButtons(sf::RenderWindow& window, sf::View& view, config& config, State& state) {

}

void State::render(sf::RenderWindow& window, sf::View& view, config& config) {

}

void State::handleInput(config& config) {

}

void State::runScript(std::string scriptID, std::string args) {
	//mScripts[script](args);
	script f = mScripts[scriptID];
	(this->*f)(args);
}

void State::changeState(std::string args) {
	//getStateMachine()->changeState(args);
	std::cout << "State::changeState\n";
}

std::vector<GameObject>& State::getGameObjects() {
	return mGameObjects;
}

std::vector<GameObject>& State::getSharedObjects() {
	return mSharedObjects;
}

StateMachine*& State::getStateMachine() {
	return mStateMachine;
}

sf::Clock& State::getButtonClock() {
	return mButtonClock;
}

std::map<std::string, script>& State::getScripts() {
	return mScripts;
}

std::vector<Button>& State::getButtons() {
	return mButtons;
}

void State::setStateMachine(StateMachine* stateMachine) {
	mStateMachine = stateMachine;
}

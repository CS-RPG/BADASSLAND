//State.cpp
#include <State.hpp>

//============State=========================
//
void State::update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config) {

}

void State::render(sf::RenderWindow& window, sf::View& view, config& config) {

}

void State::handleInput(config& config) {

}

std::vector<std::vector<int>>& State::getLevelMap() {
	return mLevelMap;
}

std::vector<GameObject>& State::getGameObjects() {
	return mGameObjects;
}

std::vector<GameObject>& State::getSharedObjects() {
	return mSharedObjects;
}

int State::getMapHeight() {
	return mMapHeight;
}

int State::getMapWidth() {
	return mMapWidth;
}

StateMachine*& State::getStateMachine() {
	return mStateMachine;
}

void State::setMapHeight(int height) {
	mMapHeight = height;
}

void State::setMapWidth(int width) {
	mMapWidth = width;
}

void State::setStateMachine(StateMachine* stateMachine) {
	mStateMachine = stateMachine;
}

//
//==========================================
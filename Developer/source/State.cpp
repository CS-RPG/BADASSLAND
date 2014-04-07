//State.cpp
#include <State.hpp>

//============State=========================
//
void State::update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config) {

}

void State::render(sf::RenderWindow& window, sf::View& view, config& config) {

}

std::vector<std::vector<int>>& State::getLevelMap() {
	return mLevelMap;
}

std::vector<GameObject>& State::getGameObjects() {
	return mGameObjects;
}

int State::getMapHeight() {
	return mMapHeight;
}

int State::getMapWidth() {
	return mMapWidth;
}

void State::setMapHeight(int height) {
	mMapHeight = height;
}

void State::setMapWidth(int width) {
	mMapWidth = width;
}

//
//==========================================
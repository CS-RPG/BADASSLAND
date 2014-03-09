//SocialComponent.cpp
#include <SFML/Graphics.hpp>

#include <SocialComponent.hpp>


//============SocialComponent===============
//
SocialComponent::~SocialComponent() {}

sf::String SocialComponent::getName() {
	return mName;
}

sf::String SocialComponent::getFaction() {
	return mFaction;
}

void SocialComponent::setName(sf::String name) {
	mName = name;
}

void SocialComponent::setFaction(sf::String faction) {
	mFaction = faction;
}
//
//==========================================
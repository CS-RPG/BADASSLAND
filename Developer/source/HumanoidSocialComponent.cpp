//HumanoidSocialComponent.cpp

#include <HumanoidSocialComponent.hpp>


//============HumanoidSocialComponent=======
HumanoidSocialComponent::HumanoidSocialComponent(sf::String name, sf::String faction) {

	setName(name);
	setFaction(faction);

}

HumanoidSocialComponent::HumanoidSocialComponent(objectSocial settings) {

	setName(settings.name);
	setFaction(settings.faction);

}

//SocialComponent.hpp
#ifndef _SocialComponent_hpp_included_
#define _SocialComponent_hpp_included_

#include <SFML/Graphics.hpp>


//============SocialComponent===============
//
class SocialComponent {
public:

	virtual							~SocialComponent();
	//virtual void					update(GameObject&) = 0;

	sf::String						getName();
	sf::String						getFaction();

	void							setName(sf::String);
	void							setFaction(sf::String);

private:

	sf::String						mName;
	sf::String						mFaction;

};
//
//==========================================


#endif
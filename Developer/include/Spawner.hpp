//Spawner.hpp
#ifndef _Spawner_hpp_included_
#define _Spawner_hpp_included_

#include <_includes_system.hpp>
#include <_includes_badass.hpp>


extern TextureHolder		gTextureHolder;


namespace Objects {

	enum ID {

		Elf

	};


}

class Spawner {
public:

	GameObject*						get(Objects::ID objectID, sf::Vector2f coordinates, std::string name, std::string faction, config& config);

private:

	InputComponent*					mInput;
	PhysicsComponent*				mPhysics;
	GraphicsComponent*				mGraphics;
	CombatComponent*				mCombat;
	SocialComponent*				mSocial;

};

#endif
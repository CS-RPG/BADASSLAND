//Spawner.cpp
#include <Spawner.hpp>


GameObject* Spawner::get(Objects::ID objectID, sf::Vector2f coordinates, std::string name, std::string faction, config& config) {
	
	switch(objectID) {

		case(Objects::Elf):
			mPhysics = new DynamicPhysicsComponent(sf::FloatRect(coordinates.x, coordinates.y, config.tileSize, config.tileSize), 0.1);
			mCombat = new HumanoidCombatComponent(150, 150, 40, 130, 2);
			mGraphics = new HumanoidGraphicsComponent(Textures::Elf_Red);

	}

	mInput = new BotActiveInputComponent();
	mSocial = new HumanoidSocialComponent(name, faction);


	return new GameObject(	mInput,
							mPhysics,
							mGraphics,
							mCombat,
							mSocial);

}
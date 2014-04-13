//World.hpp
#ifndef _World_hpp_included_
#define _World_hpp_included_

#include <_includes_system.hpp>
#include <_includes_badass.hpp>

extern TextureHolder		gTextureHolder;

namespace Objects {

	enum ID {

		Player,
		Elf_Enemy,
		Elf_Friendly,
		Elf_Minion

	};


}

//============WORLD=========================
//
class World : public State {
public:

									World(std::string fileName, config& config);

	virtual void					update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config);
	virtual void					render(sf::RenderWindow& window, sf::View& view, config& config);

	void							resolveMapCollision(GameObject*, int, int);
	void							resolveObjectCollision(GameObject*, int);
	void							loadLevelMap(std::string);
	void							deleteLevelMap();

	void							spawnObject(Objects::ID objectID, sf::Vector2i coordinates, config& config);

private:

	sf::Clock						mSpawnClock;

	//Spawner							mSpawner;

	std::vector<std::vector<bool>>	mCollisionMap;

	std::map<std::string,
		Objects::ID>				mObjectMap;

	sf::RectangleShape				mTile;

	sf::Text						mTextHealth;
	sf::Text						mTextMana;
	sf::Text						mTextEnemyCount;
	sf::Text						mTextPlayerCoordinates;
	sf::Text						mTextMouseCoordinates;

};
//
//==========================================


#endif
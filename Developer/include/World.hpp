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
	void							updateMouseCoordinates(sf::RenderWindow& window, config& config, sf::Vector2f viewPosition);

	void							resolveMapCollision(GameObject*, int, int);
	void							resolveObjectCollision(GameObject*, int);

	bool							loadLevelMap(std::string);
	void							buildCollisionMap();

	void							deleteLevelMap();

	void							spawnObject(Objects::ID objectID, sf::Vector2i coordinates, config& config);

	std::vector<std::vector<bool>>&	getCollisionMap();
	float							getViewWidth();
	float							getViewHeight();
	sf::Vector2i					getMouseCoordinates();

private:

	sf::Clock						mSpawnClock;

	sf::Vector2i					mMouseCoordinates;

	std::vector<std::vector<bool>>	mCollisionMap;

	std::map<std::string,
		Objects::ID>				mObjectMap;

	sf::RectangleShape				mTile;

	float							mViewWidth;
	float							mViewHeight;

	sf::Text						mTextHealth;
	sf::Text						mTextMana;
	sf::Text						mTextEnemyCount;
	sf::Text						mTextObjectCoordinates;
	sf::Text						mTextMouseCoordinates;
	sf::Text						mOutConsole;

	int								mCenterObjectN;	//Index of an object which acts as a view center.

};
//
//==========================================


#endif
//World.hpp
#ifndef _World_hpp_included_
#define _World_hpp_included_

#include <_includes_system.hpp>
#include <_includes_badass.hpp>

//#define myGameObjectIter std::vector<GameObject>::iterator
//#define myGameObjectConstIter std::vector<GameObject>::const_iterator

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

	void							initializeObjectMap();
	void							initializeFactionKarmaMap();

	void							onCreate();
	void							onExit();

	virtual void					update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config);

	virtual void					updateObjects(float deltaTime, config& config);
	virtual void					updateView(sf::RenderWindow& window, sf::View& view, config& config);
	void							updateMouseCoordinates(sf::RenderWindow& window, config& config, sf::Vector2f viewPosition);
	virtual void					updateGrid(config& config);
	virtual void					updateHUD(sf::RenderWindow& window);

	virtual void					render(sf::RenderWindow& window, sf::View& view, config& config);
	virtual void					handleInput(config& config);

	//Direction:
	//0 == horizontal
	//1 == vertical
	void							resolveMapCollision(GameObject*, int direction, int tileSize);
	void							resolveObjectCollision(GameObject*, int direction);

	std::vector<sf::Vector2i>		getAdjacentTiles(sf::Vector2i tile);
	bool							wavePathFind(sf::Vector2i source, sf::Vector2i destination, std::vector<sf::Vector2i>& path);

	bool							loadLevelMap(std::string);
	void							buildCollisionMap();
	void							deleteLevelMap();

	void							loadObjects();

	void							spawnObject(Objects::ID objectID, sf::Vector2i coordinates, config& config);
	bool							areEnemies(GameObject& object1, GameObject& object2);

	std::vector<std::vector<bool>>&	getCollisionMap();
	float							getViewWidth();
	float							getViewHeight();
	sf::Vector2i					getMouseCoordinates();
	std::map<std::string, bool>&	getFactionKarmaMap();
	bool							isPlayerAlive();

	void							setPlayerAlive(bool isAlive);

private:

	bool							mTerminateGame;

	sf::Clock						mSpawnClock;

	sf::Vector2i					mMouseCoordinates;

	std::vector<std::vector<bool>>	mCollisionMap;

	std::map<std::string,
		Objects::ID>				mObjectMap;

	std::map<Objects::ID, 
		objectGraphics>				mObjectGraphics;

	//Returns true/false depending on karma associated with the faction.
	//true == good
	//false == bad
	std::map<std::string, bool>		mFactionKarmaMap;

	sf::RectangleShape				mTile;

	bool							mPathHighlight;
	sf::RectangleShape				mPathTile;

	bool							mGridActive;
	sf::RectangleShape				mVerticalLine;
	sf::RectangleShape				mHorizontalLine;

	sf::Vector2f					mViewPosition;
	sf::FloatRect					mScreenCenter;
	float							mViewWidth;
	float							mViewHeight;

	sf::Text						mTextHealth;
	sf::Text						mTextMana;
	sf::Text						mTextEnemyCount;
	sf::Text						mTextObjectCoordinates;
	sf::Text						mTextMouseCoordinates;
	sf::Text						mOutConsole;

	bool							mIsPlayerAlive;
	myGameObjectIter				mCenterObject;	//!!!! To be used in future. I hope.
	int								mCenterObjectN;	//Index of an object which acts as a view center.

};
//
//==========================================


#endif

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
class World : public State {
public:

									World(std::string fileName, config& config, StateMachine* game);

	void							initializeObjectMap();
	void							initializeFactionKarmaMap();
	virtual void					initializeScripts();

	void							onCreate();
	void							onExit();

	//============UPDATE========================
	virtual void					update(float deltaTime, sf::RenderWindow& window, sf::View& view, config& config);
	virtual void					updateObjects(float deltaTime, config& config);
	virtual void					updateView(sf::RenderWindow& window, sf::View& view, config& config);
	void							updateMouseCoordinates(sf::RenderWindow& window, config& config, sf::Vector2f viewPosition);
	virtual void					updateGrid(config& config);
	virtual void					updateHUD(sf::RenderWindow& window);

	//============INPUT=========================
	virtual void					handleInput(config& config);

	//============RENDER========================
	virtual void					render(sf::RenderWindow& window, sf::View& view, config& config);
	virtual void					renderMap(sf::RenderWindow& window, sf::View& view, config& config);
	virtual void					renderObjects(sf::RenderWindow& window, sf::View& view, config& config);
	virtual void					renderHUD(sf::RenderWindow& window, sf::View& view, config& config);

	//============OTHER=========================
	void							resolveMapCollision(GameObject*, int direction, int tileSize);	//Direction:	0 == horizontal
	void							resolveObjectCollision(GameObject*, int direction);				//				1 == vertical

	std::vector<sf::Vector2i>		getAdjacentTiles(sf::Vector2i tile);
	bool							wavePathFind(sf::Vector2i source, sf::Vector2i destination, std::vector<sf::Vector2i>& path);

	//void							loadHUDInfo();
	bool							loadLevelMap(std::string fileName);
	void							buildCollisionMap();
	void							deleteLevelMap();

	bool							loadObject(std::string fileName);

	void							spawnObject(Objects::ID objectID, sf::Vector2i coordinates, config& config);
	bool							areEnemies(GameObject& object1, GameObject& object2);

	//============GET===========================
	std::vector<std::vector<int>>&	getLevelMap();
	int								getMapHeight();
	int								getMapWidth();
	std::vector<std::vector<bool>>&	getCollisionMap();

	float							getViewWidth();
	float							getViewHeight();
	sf::Vector2i					getMouseCoordinates();
	std::map<std::string, bool>&	getFactionKarmaMap();
	bool							isPlayerAlive();

	//============SET===========================
	void							setMapHeight(int height);
	void							setMapWidth(int width);
	void							setPlayerAlive(bool isAlive);

private:

	bool							mTerminateGame;

	sf::Clock						mSpawnClock;

	sf::Vector2i					mMouseCoordinates;

	std::vector<std::vector<int>>	mLevelMap;
	std::vector<std::vector<bool>>	mCollisionMap;
	int								mMapHeight;
	int								mMapWidth;

	std::map<std::string,
		Objects::ID>				mObjectMap;

	std::map<std::string,
		objectData>					mObjectDataMap;
	/*
	//Object component maps.
	std::map<std::string,
		InputComponent>				mObjectInputMap;

	std::map<std::string,
		PhysicsComponent>			mObjectPhysicsMap;

	std::map<std::string,
		GraphicsComponent>			mObjectGraphicsMap;

	std::map<std::string,
		CombatComponent>			mObjectCombatMap;

	std::map<std::string,
		SocialComponent>			mObjectSocialMap;
	*/

	std::map<Objects::ID, 
		objectGraphics>				mObjectGraphics;

	std::map<std::string, bool>		mFactionKarmaMap;	//true/false depending on karma associated with the faction.
														//true == good, false == bad.

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

#endif

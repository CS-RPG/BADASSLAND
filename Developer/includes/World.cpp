//World.cpp
void World::resolveCollision(sf::FloatRect& rect, sf::Vector2f movement, int direction, int tileSize) {

	for(int i = rect.top / tileSize; i < (rect.top + rect.height) / tileSize; ++i)
		for(int j = rect.left / tileSize; j < (rect.left + rect.width) / tileSize; ++j) {

			if(mLevelMap[i][j] == 'B') {
					if((movement.x > 0) && (direction == 0)) rect.left = j * tileSize - rect.width;
					if((movement.x < 0) && (direction == 0)) rect.left = j * tileSize + tileSize;
					if((movement.y > 0) && (direction == 1)) rect.top = i * tileSize - rect.height;
					if((movement.y < 0) && (direction == 1)) rect.top = i * tileSize + tileSize;
			}

		}

}

void World::loadLevelMap(std::string filename) {

	//Clearing memory.
	//for(int i = 0; i < mMapHeight; ++i)
	//	delete[] mLevelMap[i];
	//delete[] mLevelMap;

	std::ifstream inputFile(filename);

	inputFile >> mMapHeight >> mMapWidth;

	//Allocating memory.
	mLevelMap = new char*[mMapHeight];
	for(int i = 0; i < mMapHeight; ++i)
		mLevelMap[i] = new char[mMapWidth];

	inputFile.get();
	for(int i = 0; i < mMapHeight; ++i) {
		for(int j = 0; j < mMapWidth; ++j)
			inputFile.get(mLevelMap[i][j]);
		inputFile.get();
	}

	inputFile.close();

}

int World::getMapHeight() {
	return mMapHeight;
}

int World::getMapWidth() {
	return mMapWidth;
}
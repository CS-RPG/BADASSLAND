//Character.cpp
void Character::setHealthBar(sf::Texture& image, int width = 100, int height = 10) {
	mHealthBar.mSprite.setTexture(image);
	mHealthBar.mWidth = width;
	mHealthBar.mHeight = height;
	mHealthBar.mSprite.setTextureRect(sf::IntRect(0, 0, mHealthBar.mWidth, mHealthBar.mHeight));	
}

void Character::updateHealthBar(int offsetX, int offsetY) {
	if(mHP / mMaxHP >= 0.6)
		mHealthBar.mSprite.setColor(sf::Color::Green);
	else if((mHP / mMaxHP >= 0.35) && (mHP / mMaxHP < 0.6))
		mHealthBar.mSprite.setColor(sf::Color::Yellow);
	else if(mHP / mMaxHP < 0.35)
		mHealthBar.mSprite.setColor(sf::Color::Red);

	mHealthBar.mSprite.setTextureRect(sf::IntRect(mHealthBar.mWidth * (1 - mHP / mMaxHP), 0, mHealthBar.mWidth, mHealthBar.mHeight));
	mHealthBar.mSprite.setPosition(mRect.left - offsetX, mRect.top + mRect.height - offsetY);
}

void Character::drawHealthBar(sf::RenderWindow window) {
	window.draw(mHealthBar.mSprite);
}

void Character::setMovement(sf::Vector2f movement) {
	mMovement = movement;
}

void Character::move(sf::Vector2f movement) {
	mMovement = movement;

	if(movement.x > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 925, mRect.width, mRect.height));
	if(movement.x < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 667, mRect.width, mRect.height));
	if(movement.y > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 535, mRect.width, mRect.height));
	if(movement.y < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 787, mRect.width, mRect.height));

	mSprite.move(mMovement);
}

void Character::setDirection(int direction) {
	mDirection = direction;
}

void Character::setLevel(int level) {
	mLevel = level;
}

void Character::setHP(float HP) {
	mHP = HP;
}

void Character::setMaxHP(float maxHP) {
	mMaxHP = maxHP;
}

void Character::setMP(int MP) {
	mMP = MP;
}

void Character::setMaxMP(int maxMP) {
	mMaxMP = maxMP;
}

void Character::kill() {
	mIsAlive = false;
}

void Character::revive() {
	mIsAlive = true;
}

void Character::setAttributes(attributes& attributes) {
	////////////Copy all members.
}

void Character::setResistances(resistances& resistances) {
	////////////Copy all members.
}

sf::Vector2f Character::getMovement() {
	return mMovement;
}

int Character::getDirection() {
	return mDirection;
}

int Character::getLevel() {
	return mLevel;
}

float Character::getHP() {
	return mHP;
}

float Character::getMaxHP() {
	return mMaxHP;
}

float Character::getMP() {
	return mMP;
}

float Character::getMaxMP() {
	return mMaxMP;
}

bool Character::getIsAlive() {
	return mIsAlive;
}

attributes* Character::getAttributes() {
	return &mAttributes;	//*?
}

resistances* Character::getResistances() {
	return &mResistances;	//*?
}

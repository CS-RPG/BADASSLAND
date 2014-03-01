//Entity.cpp
void Entity::setName(sf::String name) {
	mName = name;
	mTextName.setString(mName);
}

void Entity::setRect(sf::FloatRect rect) {
	mRect = rect;
}

void Entity::setSprite(sf::Sprite sprite) {
	mSprite = sprite;
}

void Entity::setTextName(const sf::String& textName, const sf::Font& font, unsigned int characterSize = 30) {
	mTextName.setString(textName);
	mTextName.setFont(font);
	mTextName.setCharacterSize(characterSize);
}

std::string Entity::getName() {
	return mName;
}

sf::FloatRect Entity::getRect() {
	return mRect;
}

sf::Sprite Entity::getSprite() {
	return mSprite;
}

sf::Text Entity::getTextName() {
	return mTextName;
}

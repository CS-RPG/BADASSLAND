//Entity.hpp
class Entity {
public:

	void					setName(sf::String name);

	void					setRect(sf::FloatRect rect);
	void					setSprite(sf::Sprite sprite);
	void					setTextName(const sf::String& textName, const sf::Font& font, unsigned int characterSize);
	
	std::string				getName();

	sf::FloatRect			getRect();
	sf::Sprite				getSprite();
	sf::Text				getTextName();

protected:	//private

	sf::String				mName;

	sf::FloatRect			mRect;
	sf::Sprite				mSprite;
	sf::Text				mTextName;

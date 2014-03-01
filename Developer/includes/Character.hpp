//Character.hpp
class Character : public Entity {
public:

	void					setHealthBar(sf::Texture& image, int width, int height);
	void					updateHealthBar(int offsetX, int offsetY);
	void					drawHealthBar(sf::RenderWindow window);

	void					setMovement(sf::Vector2f movement);
	void					move(sf::Vector2f movement);
	void					setDirection(int direction);

	void					setLevel(int level);
	void					setHP(float HP);
	void					setMaxHP(float maxHP);
	void					setMP(int MP);
	void					setMaxMP(int maxMP);
	void					kill();
	void					revive();
	void					setAttributes(attributes& attributes);
	void					setResistances(resistances& resistances);

	sf::Vector2f			getMovement();
	int						getDirection();

	int						getLevel();
	float					getHP();
	float					getMaxHP();
	float					getMP();
	float					getMaxMP();
	bool					getIsAlive();
	attributes*				getAttributes();
	resistances*			getResistances();

protected:	//private

	sf::Vector2f			mMovement;
	int						mDirection;

	float					mCurrentFrame;

	int						mLevel;
	int						mHP;
	int						mMaxHP;
	float					mMP;
	float					mMaxMP;
	bool					mIsAlive;
	attributes				mAttributes;
	resistances				mResistances;
	healthBar				mHealthBar;

};
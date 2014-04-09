//BotPassiveInputComponent.hpp
#ifndef _BotPassiveInputComponent_hpp_included_
#define _BotPassiveInputComponent_hpp_included_


//============BotPassiveInputComponent======
//
class BotPassiveInputComponent : public InputComponent {
public:

									BotPassiveInputComponent(float changeDirectionFrequency);
	virtual void					update(GameObject& source, World& world);

	float							getChangeDirectionFrequency();
	void							setChangeDirectionFrequency(float changeDirectionFrequency);
	

private:

	sf::Clock						mMovementClock;
	float							mChangeDirectionFrequency;
	int								mDirection;

};
//
//==========================================


#endif
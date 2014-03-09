//BotPassiveInputComponent.hpp
#ifndef _BotPassiveInputComponent_hpp_included_
#define _BotPassiveInputComponent_hpp_included_


//============BotPassiveInputComponent======
//
class BotPassiveInputComponent : public InputComponent {
public:

									BotPassiveInputComponent(float);
	virtual void					update(GameObject&, World&);

	float							getChangeDirectionFrequency();
	void							setChangeDirectionFrequency(float);
	

private:

	sf::Clock						mMovementClock;
	float							mChangeDirectionFrequency;
	int								mDirection;

};
//
//==========================================


#endif
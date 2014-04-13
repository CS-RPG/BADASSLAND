//CombatComponent.hpp
#ifndef _CombatComponent_hpp_included_
#define _CombatComponent_hpp_included_

//Prototypes.
class GameObject;


//============CombatComponent===============
//
class CombatComponent {
public:

	virtual							~CombatComponent();
	virtual void					update(GameObject&, World&) = 0;
	virtual void					attack(GameObject& attacker, GameObject* target) = 0;
	virtual void					takeDamage(GameObject& attacker, float damage) = 0;
	//virtual void					heal(float) = 0;

	float							getHP();
	float							getMaxHP();
	float							getDT();
	bool							isAlive();
	bool							isMarkedForRemoval();

	float							getAttackRange();
	float							getAttackSpeed();
	float							getDamage();
	bool							isReadyToAttack();

	void							setHP(float);
	void							setMaxHP(float);
	void							setDT(float);
	void							kill();
	void							revive();
	void							setMarkedForRemoval(bool);

	void							setAttackRange(float);
	void							setAttackSpeed(float);
	void							setDamage(float);

	void							restartAttackClock();

private:

	float							mHP;
	float							mMaxHP;
	float							mDT;
	bool							mIsAlive;
	bool							mIsMarkedForRemoval;

	float							mAttackRange;
	float							mAttackSpeed;
	float							mDamage;

	sf::Clock						mAttackClock;

};
//
//==========================================


#endif
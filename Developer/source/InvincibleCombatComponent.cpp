//InvincibleCombatComponent.cpp
#include <InvincibleCombatComponent.hpp>


//============InvincibleCombatComponent=====
//
InvincibleCombatComponent::InvincibleCombatComponent(int HP, int maxHP, float damage, float attackRange, float attackSpeed) {
	
	setMaxHP(maxHP);
	setHP(maxHP);

	setDamage(damage);
	setAttackRange(attackRange);
	setAttackSpeed(attackSpeed);
	setMarkedForRemoval(false);

}

void InvincibleCombatComponent::update(GameObject& object, World& world) {

	if(!isAlive())
		setMarkedForRemoval(true);

}

void InvincibleCombatComponent::attack(GameObject& attacker, GameObject& target) {
	target.getCombat()->takeDamage(attacker, target, getDamage());
	restartAttackClock();
}

void InvincibleCombatComponent::takeDamage(GameObject& attacker, GameObject& target, float damage) {

	//setHP(getHP() - damage);
	std::string name = attacker.getSocial()->getName();
	std::printf("%s dares to attack %s!\n", std::string(attacker.getSocial()->getName()).c_str(), std::string(target.getSocial()->getName()).c_str());
	attacker.getCombat()->takeDamage(target, attacker, damage);
	
	if(getHP() <= 0) {

		kill();
		std::printf("%s is killed by %s.\n", std::string(target.getSocial()->getName()).c_str(), std::string(attacker.getSocial()->getName()).c_str());
		attacker.getInput()->setTargeting(false);

	}

}
//
//==========================================
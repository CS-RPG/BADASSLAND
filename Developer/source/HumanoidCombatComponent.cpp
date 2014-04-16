//HumanoidCombatComponent.cpp
#include <HumanoidCombatComponent.hpp>


//============HumanoidCombatComponent=======
//
HumanoidCombatComponent::HumanoidCombatComponent(int HP, int maxHP, float damage, float attackRange, float attackSpeed) {
	
	setMaxHP(maxHP);
	setHP(maxHP);

	setDamage(damage);
	setAttackRange(attackRange);
	setAttackSpeed(attackSpeed);
	setMarkedForRemoval(false);

}

void HumanoidCombatComponent::update(GameObject& object, World& world) {

	if(!isAlive())
		setMarkedForRemoval(true);

}

void HumanoidCombatComponent::attack(GameObject& attacker, GameObject& target) {
	target.getCombat()->takeDamage(attacker, target, getDamage());
	restartAttackClock();
}

void HumanoidCombatComponent::takeDamage(GameObject& attacker, GameObject& target, float damage) {

	setHP(getHP() - damage);
	std::string attackerName = std::string(attacker.getSocial()->getName());
	std::string targetName = std::string(target.getSocial()->getName());
	
	if(&attacker == &target)
		targetName = "himself";

	std::cout << attackerName << " deals " << damage << " damage to " << targetName << ".\n";
	//std::printf("%s deals %.0f damage to %s.\n", attackerName.c_str(), damage, targetName.c_str());
	
	if(getHP() <= 0) {

		kill();
		std::printf("%s is killed by %s.\n", std::string(target.getSocial()->getName()).c_str(), std::string(attacker.getSocial()->getName()).c_str());
		attacker.getInput()->setTargeting(false);

	}

}
//
//==========================================
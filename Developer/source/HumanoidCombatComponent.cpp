//HumanoidCombatComponent.cpp
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <GameObject.hpp>

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

void HumanoidCombatComponent::attack(GameObject& attacker, GameObject* target) {
	target->getCombat()->takeDamage(attacker, getDamage());
	restartAttackClock();
}

void HumanoidCombatComponent::takeDamage(GameObject& attacker, float damage) {
	setHP(getHP() - damage);
	if(getHP() <= 0) {
		kill();
		attacker.getInput()->setTargeting(false);
	}
}
//
//==========================================
//HumanoidCombatComponent.cpp
#include <SFML/Graphics.hpp>
#include <World.hpp>
#include <GameObject.hpp>

#include <HumanoidCombatComponent.hpp>


//============HumanoidCombatComponent=======
//
HumanoidCombatComponent::HumanoidCombatComponent(int HP, int maxHP, float damage, float attackRange, float attackSpeed) {
	
	setHP(HP);
	setMaxHP(maxHP);

	setDamage(damage);
	setAttackRange(attackRange);
	setAttackSpeed(attackSpeed);
	setMarkedForRemoval(false);

}

void HumanoidCombatComponent::update(GameObject& object, World& world) {

	if(!isAlive())
		setMarkedForRemoval(true);

}

void HumanoidCombatComponent::takeDamage(float damage) {
	setHP(getHP() - damage);
	if(getHP() <= 0)
		kill();
}

void HumanoidCombatComponent::attack(GameObject* target) {
	target->getCombat()->takeDamage(getDamage());
	restartAttackClock();
}
//
//==========================================
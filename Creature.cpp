#include "Creature.h"

void Creature::OnAttacked(Creature* attacker)
{
	int damage = attacker -> attack - defence;
	if (damage < 0) damage = 0;

	hp -= damage;
	if (hp < 0) hp = 0;
}
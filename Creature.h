#pragma once

enum CreatureType
{
	CT_PLAYER = 0,
	CT_MONSTER = 1
};

class Creature
{
public:
	Creature(int creatureType) : creatureType(creatureType), hp(0), attack(0), defence(0)
	{

	}
	virtual ~Creature()
	{

	}

	virtual void PrintInfo() = 0;
	virtual void PrintHp() = 0;

	void OnAttacked(Creature* attacker);
	bool IsDead() { return hp <= 0; }

protected:
	int creatureType;

public:
	int maxhp;
	int hp;
	int attack;
	int defence;
	int maxattack;
	int maxdefence;
};


#pragma once
#include "Creature.h"
enum PlayerClass
{
	PC_Knight = 1,
	PC_Archer = 2,
	PC_Mage = 3
};

class Player : public Creature
{
public:
	Player(int playerClass) : Creature(CT_PLAYER), playerClass(playerClass)
	{

	}
	virtual ~Player()
	{

	}

	virtual void PrintInfo();
	virtual void PrintHp();

protected:
	int playerClass;
};

class Knight : public Player
{
public:
	Knight() : Player(PC_Knight)
	{
		hp = 150;
		attack = 8;
		defence = 3;
		maxhp = 150;
		maxattack = 18;
		maxdefence = 8;
	}
};

class Archer : public Player
{
public:
	Archer() : Player(PC_Archer)
	{
		hp = 100;
		maxhp = 100;
		attack = 10;
		defence = 2;
		maxattack = 17;
		maxdefence = 5;
	}
};

class Mage : public Player
{
public:
	Mage() : Player(PC_Mage)
	{
		hp = 60;
		maxhp = 60;
		attack = 15;
		defence = 0;
		maxattack = 20;
		maxdefence = 5;
	}
};
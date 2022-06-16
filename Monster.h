#pragma once
#include "Creature.h"

enum MonsterType
{
	MT_SLIME = 1,
	MT_ORC = 2,
	MT_SKELETON = 3,
	MT_BOSS = 4
};

class Monster : public Creature
{
public:
	Monster(int monsterType) 
		: Creature(CT_MONSTER), monsterType(monsterType)
	{

	}

	virtual void PrintInfo();
	virtual void PrintHp();

	int monsterType;
};

class Slime : public Monster
{
public:
	Slime() : Monster(MT_SLIME)
	{
		hp = 50;
		attack = 5;
		defence = 0;
	}
};

class Orc : public Monster
{
public:
	Orc() : Monster(MT_ORC)
	{
		hp = 80;
		attack = 8;
		defence = 2;
	}
};

class Skeleton : public Monster
{
public:
	Skeleton() : Monster(MT_SKELETON)
	{
		hp = 100;
		attack = 10;
		defence = 4;
	}
};

class Boss : public Monster
{
public:
	Boss() : Monster(MT_BOSS)
	{
		hp = 20;
		attack = 15;
		defence = 5;
	}
};
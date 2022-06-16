#pragma once

class Player;
class Monster;

class Field
{
public:
	Field();
	~Field();

	void Update(Player* player);
	void CreateMonster(Player* player);

	void StartBattle(Player* player);

private:
	Monster* monster;
};


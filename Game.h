#pragma once

// ���漱��
class Player;
class Field;

class Game
{
public:
	Game();
	~Game();

	void FieldSpawn();
	void PlayerSpawn();

	void CreatePlayer();
private:
	Player* player;
	Field* field;
};


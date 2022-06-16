#include <iostream>
#include "Game.h"
using namespace std;

int main()
{
	srand((unsigned int)time(nullptr));

	cout << "----------------------TEXT RPG----------------------" << "\n" << endl;
	cout << "게임 규칙" << "\n" << endl;
	cout << "1. 몬스터는 랜덤으로 출현하며, 사냥에 성공하면 스킬포인트를 얻습니다." << endl;
	cout << "=> 직업마다 도달 가능한 최대 공격력과 최대 방어력이 다릅니다." << endl;
	cout << "2. 몬스터 사냥에 성공하면 체력이 회복됩니다." << endl;
	cout << "3. 캐릭터를 육성하여 보스 몬스터 사냥에 성공하면 게임에서 승리합니다!" << "\n" << "\n" << endl;

	Game game;
	game.FieldSpawn();
	game.PlayerSpawn();
	
	return 0;
}

#include <iostream>
#include "Field.h"
#include <stdlib.h>
#include "Creature.h"
#include "Monster.h"
#include "Player.h"
using namespace std;

Field::Field() : monster(nullptr)
{

}

Field::~Field()
{
	if (monster != nullptr)
		delete monster;
}

void Field::Update(Player* player)
{
	if (monster == nullptr)
		CreateMonster(player);

	StartBattle(player);
}

void Field::CreateMonster(Player* player)
{
	int randValue = 1 + rand() % 3;

	switch (randValue)
	{
	case MT_SLIME:
		monster = new Slime();
		cout << "야생의 슬라임(Lv.1)이(가) 발견되었다!" << endl;
		break;
	case MT_ORC:
		monster = new Orc();
		cout << "야생의 오크(Lv.2)이(가) 발견되었다!" << endl;
		break;
	case MT_SKELETON:
		monster = new Skeleton();
		cout << "야생의 스켈레톤(Lv.3)이(가) 발견되었다!" << endl;
		break;
	}
	player->PrintInfo();
	monster->PrintInfo();

	cout << "1)전투를 진행한다. ";
	cout << "2)새로운 몬스터를 찾는다. ";
	cout << "3)보스몬스터에 도전한다." << "\n" << endl;
	while (1) {
		cout << "숫자 입력: ";
		char num = 0; cin >> num;
		cout << endl;
		if (num - 48 == 1) {
			break;
		}
		else if (num - 48 == 2) {
			cout << "새로운 몬스터를 찾습니다." << "\n" << endl;
			delete monster;
			monster = nullptr;
			CreateMonster(player);
			return;
		}
		else if (num - 48 == 3) {
			cout << "보스 몬스터에게 도전합니다." << "\n" << endl;
			delete monster;
			monster = nullptr;
			monster = new Boss();
			monster->PrintInfo();
			break;
		}
		else cout << "다시 입력해주세요." << endl;
	}
	return;
}

void Field::StartBattle(Player* player)
{
	// 플레이어->몬스터 공격
	cout << "플레이어가 몬스터를 공격합니다." << "\n" << endl;
	monster->OnAttacked(player);
	monster->PrintHp();
	if (monster->IsDead())
	{
		if (monster->monsterType == 4) {
			cout << "보스몬스터를 처치하였습니다. 게임에서 승리하셨습니다!" << endl;
			cout << "Game Over" << endl;
			return;
		}

		cout << "축하드립니다! 몬스터를 사냥하였습니다." << "\n" << endl;
		cout << "스킬포인트 " << monster->monsterType * 2 << "을(를) 획득하였습니다." << endl;
		cout << "스킬 포인트를 사용합니다." << endl;
		cout << "1)공격력 증가 2)방어력 증가 3)hp 증가" << endl;
		while (1) {
			cout << "숫자 입력: ";
			char num = 0; cin >> num;
			cout << endl;
			if (num - 48 == 1) {
				player->attack += monster->monsterType * 2;
				if (player->attack > player->maxattack) {
					player->attack = player->maxattack;
					cout << "최대 공격력에 도달하였습니다." << endl;
				}
				else cout << "공격력이 " << monster->monsterType * 2 <<"증가하였습니다." << endl;
				break;
			}
			else if (num - 48 == 2) {
				player->defence += monster->monsterType * 2;
				if (player->defence > player->defence) {
					player->defence = player->defence;
					cout << "최대 방어력에 도달하였습니다." << endl;
				}
				else cout << "방어력이 " << monster->monsterType * 2 << "증가하였습니다." << endl;
				break;
			}
			else if (num - 48 == 3) {
				player->maxhp += monster->monsterType * 2;
				cout << "HP가 " << monster->monsterType * 2 << "증가하였습니다." << endl;
				break;
			}
			else cout << "다시 입력해주세요." << endl;
		}

		cout << "캐릭터의 체력이 회복됩니다." << "\n" << endl;
		player->hp = player->maxhp;
		player->PrintInfo();

		cout << "게임을 계속 하시겠습니까? (y/n)" << "\n" << endl;
		delete monster;
		monster = nullptr;
		while (1) {
			char answer; cin >> answer;
			if (answer == 'y') {
				Update(player);
				return;
			}
			else if (answer == 'n') {
				cout << "게임을 종료합니다." << "\n" << endl;
				return;
			}
			else cout << "다시 입력해주세요.(y/n)" << "\n" << endl;
		}
	}

	// 몬스터->플레이어 공격
	cout << "몬스터가 플레이어를 공격합니다." << "\n" << endl;
	player->OnAttacked(monster);
	player->PrintHp();
	if (player->IsDead()) {
		cout << "플레이어가 사망하였습니다." << endl;		
		delete monster;
		monster = nullptr;
		return;
	}
		cout << "-------------------------------------" << endl;
		cout << "계속해서 공격 하시겠습니까? (y/n)" << endl;
	while (1) {
		char answer; cin >> answer;
		if (answer == 'y') {
			StartBattle(player); //재귀
			return;
		}
		else if (answer == 'n') {
			cout << "몬스터가 너무 강해 도망갑니다.." << "\n" << endl;
			delete monster;
			monster = nullptr;
			Update(player);
			return;
		}
		else cout << "다시 입력해주세요.(y/n)" << "\n" << endl;
	}

	return;
}

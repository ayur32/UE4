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
		cout << "�߻��� ������(Lv.1)��(��) �߰ߵǾ���!" << endl;
		break;
	case MT_ORC:
		monster = new Orc();
		cout << "�߻��� ��ũ(Lv.2)��(��) �߰ߵǾ���!" << endl;
		break;
	case MT_SKELETON:
		monster = new Skeleton();
		cout << "�߻��� ���̷���(Lv.3)��(��) �߰ߵǾ���!" << endl;
		break;
	}
	player->PrintInfo();
	monster->PrintInfo();

	cout << "1)������ �����Ѵ�. ";
	cout << "2)���ο� ���͸� ã�´�. ";
	cout << "3)�������Ϳ� �����Ѵ�." << "\n" << endl;
	while (1) {
		cout << "���� �Է�: ";
		char num = 0; cin >> num;
		cout << endl;
		if (num - 48 == 1) {
			break;
		}
		else if (num - 48 == 2) {
			cout << "���ο� ���͸� ã���ϴ�." << "\n" << endl;
			delete monster;
			monster = nullptr;
			CreateMonster(player);
			return;
		}
		else if (num - 48 == 3) {
			cout << "���� ���Ϳ��� �����մϴ�." << "\n" << endl;
			delete monster;
			monster = nullptr;
			monster = new Boss();
			monster->PrintInfo();
			break;
		}
		else cout << "�ٽ� �Է����ּ���." << endl;
	}
	return;
}

void Field::StartBattle(Player* player)
{
	// �÷��̾�->���� ����
	cout << "�÷��̾ ���͸� �����մϴ�." << "\n" << endl;
	monster->OnAttacked(player);
	monster->PrintHp();
	if (monster->IsDead())
	{
		if (monster->monsterType == 4) {
			cout << "�������͸� óġ�Ͽ����ϴ�. ���ӿ��� �¸��ϼ̽��ϴ�!" << endl;
			cout << "Game Over" << endl;
			return;
		}

		cout << "���ϵ帳�ϴ�! ���͸� ����Ͽ����ϴ�." << "\n" << endl;
		cout << "��ų����Ʈ " << monster->monsterType * 2 << "��(��) ȹ���Ͽ����ϴ�." << endl;
		cout << "��ų ����Ʈ�� ����մϴ�." << endl;
		cout << "1)���ݷ� ���� 2)���� ���� 3)hp ����" << endl;
		while (1) {
			cout << "���� �Է�: ";
			char num = 0; cin >> num;
			cout << endl;
			if (num - 48 == 1) {
				player->attack += monster->monsterType * 2;
				if (player->attack > player->maxattack) {
					player->attack = player->maxattack;
					cout << "�ִ� ���ݷ¿� �����Ͽ����ϴ�." << endl;
				}
				else cout << "���ݷ��� " << monster->monsterType * 2 <<"�����Ͽ����ϴ�." << endl;
				break;
			}
			else if (num - 48 == 2) {
				player->defence += monster->monsterType * 2;
				if (player->defence > player->defence) {
					player->defence = player->defence;
					cout << "�ִ� ���¿� �����Ͽ����ϴ�." << endl;
				}
				else cout << "������ " << monster->monsterType * 2 << "�����Ͽ����ϴ�." << endl;
				break;
			}
			else if (num - 48 == 3) {
				player->maxhp += monster->monsterType * 2;
				cout << "HP�� " << monster->monsterType * 2 << "�����Ͽ����ϴ�." << endl;
				break;
			}
			else cout << "�ٽ� �Է����ּ���." << endl;
		}

		cout << "ĳ������ ü���� ȸ���˴ϴ�." << "\n" << endl;
		player->hp = player->maxhp;
		player->PrintInfo();

		cout << "������ ��� �Ͻðڽ��ϱ�? (y/n)" << "\n" << endl;
		delete monster;
		monster = nullptr;
		while (1) {
			char answer; cin >> answer;
			if (answer == 'y') {
				Update(player);
				return;
			}
			else if (answer == 'n') {
				cout << "������ �����մϴ�." << "\n" << endl;
				return;
			}
			else cout << "�ٽ� �Է����ּ���.(y/n)" << "\n" << endl;
		}
	}

	// ����->�÷��̾� ����
	cout << "���Ͱ� �÷��̾ �����մϴ�." << "\n" << endl;
	player->OnAttacked(monster);
	player->PrintHp();
	if (player->IsDead()) {
		cout << "�÷��̾ ����Ͽ����ϴ�." << endl;		
		delete monster;
		monster = nullptr;
		return;
	}
		cout << "-------------------------------------" << endl;
		cout << "����ؼ� ���� �Ͻðڽ��ϱ�? (y/n)" << endl;
	while (1) {
		char answer; cin >> answer;
		if (answer == 'y') {
			StartBattle(player); //���
			return;
		}
		else if (answer == 'n') {
			cout << "���Ͱ� �ʹ� ���� �������ϴ�.." << "\n" << endl;
			delete monster;
			monster = nullptr;
			Update(player);
			return;
		}
		else cout << "�ٽ� �Է����ּ���.(y/n)" << "\n" << endl;
	}

	return;
}

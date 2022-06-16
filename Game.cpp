#include <iostream>
#include "Game.h"
#include "Player.h"
#include "Field.h"
using namespace std;

Game::Game() : player(nullptr), field(nullptr)
{

}

Game::~Game()
{
	if (player != nullptr) delete player;
	if (field != nullptr) delete field;
}

void Game::FieldSpawn()
{
	field = new Field();
}

void Game::PlayerSpawn()
{
	if (player == nullptr) CreatePlayer();

	field->Update(player);

	if (player->IsDead()) {
		delete player;
		player = nullptr;
		while (1) {
			cout << "������ �ٽ� �����Ͻðڽ��ϱ�? (y/n)" << endl;
			char answer; cin >> answer;
			if (answer == 'y') {
				PlayerSpawn(); //���
				return;
			}
			else if (answer == 'n') {
				cout << "Game Over" << endl;
				return;
			}
		}		
	}	
}
void Game::CreatePlayer()
{
	cout << "-------��������-------" << endl;
	cout << "1)��� 2)�ü� 3)������" << endl;
	cout << "----------------------" << endl;

	while (player == nullptr) {
		cout << "���� �Է�: ";
		char num = 0; cin >> num;
		cout << endl;

		if (num - 48 == PC_Knight) {
			cout << "��縦 �����Ͽ����ϴ�." << endl;			
			player = new Knight(); player->PrintInfo();
			cout << "ĳ���͸� ���� �����Ͻðڽ��ϱ�?(y/n)" << endl;
		}
		else if (num - 48 == PC_Archer) {
			cout << "�ü��� �����Ͽ����ϴ�." << endl;			
			player = new Archer(); player->PrintInfo();
			cout << "ĳ���͸� �ü��� �����Ͻðڽ��ϱ�?(y/n)" << endl;
		}
		else if (num - 48 == PC_Mage) {
			cout << "�����縦 �����Ͽ����ϴ�." << endl;			
			player = new Mage(); player->PrintInfo();
			cout << "ĳ���͸� ������� �����Ͻðڽ��ϱ�?(y/n)" << endl;
		}
		else cout << "�ٽ� �Է����ּ���." << endl;
	}
	while (1) {
		char answer; cin >> answer;
		if (answer == 'y') {
			cout << "\n" << "���ϵ帳�ϴ�. ĳ���Ͱ� �����Ǿ����ϴ�!" << "\n" << endl;
			cout << "����� ���� �������� ���ϴ�." << "\n" << endl;
			return;
		}
		else if (answer == 'n') {
			delete player;
			player = nullptr;
			CreatePlayer();
			return;
		}
		else cout << "�ٽ� �Է����ּ���.(y/n)" << "\n" << endl;
	}

	return;	
}
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
			cout << "게임을 다시 시작하시겠습니까? (y/n)" << endl;
			char answer; cin >> answer;
			if (answer == 'y') {
				PlayerSpawn(); //재귀
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
	cout << "-------직업선택-------" << endl;
	cout << "1)기사 2)궁수 3)마법사" << endl;
	cout << "----------------------" << endl;

	while (player == nullptr) {
		cout << "숫자 입력: ";
		char num = 0; cin >> num;
		cout << endl;

		if (num - 48 == PC_Knight) {
			cout << "기사를 선택하였습니다." << endl;			
			player = new Knight(); player->PrintInfo();
			cout << "캐릭터를 기사로 선택하시겠습니까?(y/n)" << endl;
		}
		else if (num - 48 == PC_Archer) {
			cout << "궁수를 선택하였습니다." << endl;			
			player = new Archer(); player->PrintInfo();
			cout << "캐릭터를 궁수로 선택하시겠습니까?(y/n)" << endl;
		}
		else if (num - 48 == PC_Mage) {
			cout << "마법사를 선택하였습니다." << endl;			
			player = new Mage(); player->PrintInfo();
			cout << "캐릭터를 마법사로 선택하시겠습니까?(y/n)" << endl;
		}
		else cout << "다시 입력해주세요." << endl;
	}
	while (1) {
		char answer; cin >> answer;
		if (answer == 'y') {
			cout << "\n" << "축하드립니다. 캐릭터가 생성되었습니다!" << "\n" << endl;
			cout << "사냥을 위해 던전으로 갑니다." << "\n" << endl;
			return;
		}
		else if (answer == 'n') {
			delete player;
			player = nullptr;
			CreatePlayer();
			return;
		}
		else cout << "다시 입력해주세요.(y/n)" << "\n" << endl;
	}

	return;	
}
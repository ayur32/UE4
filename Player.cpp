#include "Player.h"
#include <iostream>
using namespace std;

void Player::PrintInfo()
{
	cout << "-------------------------------------" << endl;
	cout << "[�÷��̾� ����] " << "HP: " << hp << " ATT: " << attack << " DEF: " << defence << endl;
	cout << "-------------------------------------" << endl;
}

void Player::PrintHp()
{
	cout << "�÷��̾� HP: " << hp << "\n" << endl;
}
#include "Player.h"
#include <iostream>
using namespace std;

void Player::PrintInfo()
{
	cout << "-------------------------------------" << endl;
	cout << "[플레이어 정보] " << "HP: " << hp << " ATT: " << attack << " DEF: " << defence << endl;
	cout << "-------------------------------------" << endl;
}

void Player::PrintHp()
{
	cout << "플레이어 HP: " << hp << "\n" << endl;
}
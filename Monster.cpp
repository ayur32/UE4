#include "Monster.h"
#include <iostream>
using namespace std;

void Monster::PrintInfo()
{
	cout << "-------------------------------------" << endl;
	cout << "[���� ����] " << "HP: " << hp << " ATT: " << attack << " DEF: " << defence << endl;
	cout << "-------------------------------------" << endl;
}

void Monster::PrintHp()
{
	cout << "���� HP: " << hp << "\n" << endl;
}
#include "Monster.h"
#include <iostream>
using namespace std;

void Monster::PrintInfo()
{
	cout << "-------------------------------------" << endl;
	cout << "[몬스터 정보] " << "HP: " << hp << " ATT: " << attack << " DEF: " << defence << endl;
	cout << "-------------------------------------" << endl;
}

void Monster::PrintHp()
{
	cout << "몬스터 HP: " << hp << "\n" << endl;
}
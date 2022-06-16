#include "Monster.h"
#include <iostream>
using namespace std;

void Monster::PrintInfo()
{
	cout << "-------------------------------------" << endl;
	cout << "l몬스터 정보l " << "HP: " << hp << " ATT: " << attack << " DEF: " << defence << endl;
	cout << "-------------------------------------" << endl;
}

void Monster::PrintHp()
{
	cout << "몬스터 HP: " << hp << "\n" << endl;
}

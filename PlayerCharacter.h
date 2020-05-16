#pragma once
#include "Character.h"
#include "FireBurst.h"
class PlayerCharacter :
	public Character{

public:
	PlayerCharacter(HDC hDC);
	void changeState(STATE state);
	void die();
	FireBurst* fire();
	int fireCounter;
};


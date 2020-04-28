#include "SimpleAI.h"

void SimpleAI::act() {
	int action = rand() % 3;
	switch (action)
	{
	case 0:
		//this->changeState(S_RUNL);
		//this->SetVelocity(this->GetVelocity().x - 1, 0);
		break;
	case 1:
		this->changeState(S_RUNR);
		this->SetVelocity(this->GetVelocity().x+rand()%10, 0);
		break;
	case 2:
		this->changeState(S_IDLE);
		break;
	default:
		break;
	}
}

SimpleAI::SimpleAI(HDC hDC) : Character(hDC)
{
	
}

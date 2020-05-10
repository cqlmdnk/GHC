#include "SimpleAI.h"

void SimpleAI::act(int dir) {
	int action = rand() % 3;


	if (dir == 0) {
		this->changeState(S_RUNR);
		this->SetVelocity(this->GetVelocity().x + rand() % 5, 0);
	}
	else {
		this->changeState(S_RUNL);
		this->SetVelocity(this->GetVelocity().x - rand() % 5, 0);
	}
	/*switch (action)
	{
	case 0:
		//this->changeState(S_RUNL);
		//this->SetVelocity(this->GetVelocity().x - 1, 0);
		break;
	case 1:
		this->changeState(S_RUNR);
		this->SetVelocity(this->GetVelocity().x+rand()%5, 0);
		break;
	case 2:
		this->changeState(S_IDLE);
		break;
	default:
		break;
	}*/
}

SimpleAI::SimpleAI(HDC hDC) : Character(hDC)
{
	
}

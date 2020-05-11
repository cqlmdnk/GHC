#include "Character.h"

Character::Character(HDC hDC) // constructor içinden çağrılması gerekenleri buraya yazmak zorunda kaldım değişmesi lazım
{
	
	

}


bool Character::checkState(STATE state)
{
	return state == _bCharState;
}

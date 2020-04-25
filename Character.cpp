#include "Character.h"

void Character::loadChar(HDC hDC) // constructor içinden çağrılması gerekenleri buraya yazmak zorunda kaldım değişmesi lazım
{
	_bCharAnimIdle = new Bitmap(hDC, TEXT("resources/character_idle.bmp")); 
	_bCharAnimRunR = new Bitmap(hDC, TEXT("resources/character_run.bmp"));
	_bCharAnimRunL = new Bitmap(hDC, TEXT("resources/character_run_l.bmp"));
	_bCharAnimJump = new Bitmap(hDC, TEXT("resources/character_jump.bmp"));
	SetRect(&m_rcBounds, 0, 0, 1920, 1020);
	this->SetBitmap(_bCharAnimIdle);
	SetRect(&m_rcPosition,120, 1020, 100 + _bCharAnimIdle->GetWidth(),
		1020 + _bCharAnimIdle->GetHeight());
	
	this->SetNumFrames(4);
	this->SetFrameDelay(5);
	

}

void Character::changeState(STATE state)
{
	_bCharState = state;
	this->m_iCurFrame = 0;

	switch (state)
	{
	case S_IDLE:
		this->SetBitmap(_bCharAnimIdle); // iki animasyon arası koordinat farkı var 
		this->SetFrameDelay(5);          // geçiş sırasında kopukluk hissediliyor
		this->SetNumFrames(4);
		break;
	case S_RUNL:
		this->SetBitmap(_bCharAnimRunL);
		this->SetNumFrames(6);
		this->SetFrameDelay(1);

		break;
	case S_RUNR:
		this->SetBitmap(_bCharAnimRunR);
		this->SetNumFrames(6);
		this->SetFrameDelay(1);

		break;
	case S_LJUMP:
		this->SetVelocity(0, -30);
		this->SetBitmap(_bCharAnimJump); // bir kere bastıktan sonra 7 kareyi de basması ve animasyonu bitirmesi lazım
		this->SetNumFrames(7);
		this->SetFrameDelay(1);
		this->SetAnimDef(FALSE);
		break;
	case S_RJUMP:
		this->SetVelocity(0, -30);
		this->SetBitmap(_bCharAnimJump); // bir kere bastıktan sonra 7 kareyi de basması ve animasyonu bitirmesi lazım
		this->SetNumFrames(7);
		this->SetFrameDelay(1);
		this->SetAnimDef(FALSE);
		break;
	default:
		break;
	}
}

bool Character::checkState(STATE state)
{
	return state == _bCharState;
}

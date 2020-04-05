#include "Character.h"

void Character::loadChar(HDC hDC) // constructor içinden çağrılması gerekenleri buraya yazmak zorunda kaldım değişmesi lazım
{
	_bCharAnimIdle = new Bitmap(hDC, TEXT("resources/character_idle.bmp")); 
	_bCharAnimRun = new Bitmap(hDC, TEXT("resources/character_run.bmp"));
	_bCharAnimJump = new Bitmap(hDC, TEXT("resources/character_jump.bmp"));
	SetRect(&m_rcBounds, 0, 0, 1920, 980);
	this->SetBitmap(_bCharAnimIdle);
	SetRect(&m_rcPosition,100, 980, 100 + _bCharAnimIdle->GetWidth(),
		980 + _bCharAnimIdle->GetHeight());
	
	this->SetNumFrames(4);
	this->SetFrameDelay(5);
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = 0;
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	m_baBoundsAction = BA_STOP;
	m_bHidden = FALSE;

}

void Character::changeState(STATE state)
{
	switch (state)
	{
	case S_IDLE:
		this->SetBitmap(_bCharAnimIdle); // iki animasyon arası koordinat farkı var 
		this->SetFrameDelay(5);          // geçiş sırasında kopukluk hissediliyor
		this->SetNumFrames(4);
		break;
	case S_RUN:
		this->SetBitmap(_bCharAnimRun);
		this->SetNumFrames(6);
		this->SetFrameDelay(1);
		break;
	case S_JUMP:
		this->SetVelocity(0, -30);
		this->SetBitmap(_bCharAnimJump); // bir kere bastıktan sonra 7 kareyi de basması ve animasyonu bitirmesi lazım
		this->SetNumFrames(7);
		this->SetFrameDelay(2);
		break;
	default:
		break;
	}
}

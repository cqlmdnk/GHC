#include "PlayerCharacter.h"


PlayerCharacter::PlayerCharacter(HDC hDC) : Character(hDC) // default frame number and delay canbe parameters
{
	_bCharAnimIdle = new Bitmap(hDC, TEXT("resources/character_idle.bmp"));
	_bCharAnimRunR = new Bitmap(hDC, TEXT("resources/character_run.bmp"));
	_bCharAnimRunL = new Bitmap(hDC, TEXT("resources/character_run_l.bmp"));
	_bCharAnimJumpR = new Bitmap(hDC, TEXT("resources/character_jump.bmp"));
	_bCharAnimJumpL = new Bitmap(hDC, TEXT("resources/character_jump_l.bmp"));
	_bCharFire = new Bitmap(hDC, TEXT("resources/character_fire_burst.bmp"));
	_bCharFireL = new Bitmap(hDC, TEXT("resources/character_fire_burst_l.bmp"));


	_bCharFireBurstR = new Bitmap(hDC, TEXT("resources/fire_burst.bmp"));
	_bCharFireBurstL = new Bitmap(hDC, TEXT("resources/fire_burst_l.bmp"));

	SetRect(&m_rcBounds, 0, 0, 1920, 1020);
	
	this->SetBitmap(_bCharAnimIdle);
	SetRect(&m_rcPosition, 0, 0, _bCharAnimIdle->GetWidth(), _bCharAnimIdle->GetHeight());
	this->SetPosition(320, 1020);
	this->SetNumFrames(4);
	this->SetFrameDelay(5);

	int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 3;
	int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 3;
	CopyRect(&m_rcCollision, &m_rcPosition);



	InflateRect(&m_rcCollision, iXShrink, iYShrink);
	

	
}
void PlayerCharacter::changeState(STATE state) {
	_bCharState = state;
	this->m_iCurFrame = 0;

	switch (state)
	{
	case S_IDLE:
		this->SetBitmap(_bCharAnimIdle); // iki animasyon arası koordinat farkı var 
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + _bCharAnimIdle->GetWidth(), this->GetPosition().top + _bCharAnimIdle->GetHeight() });
		this->SetFrameDelay(5);          // geçiş sırasında kopukluk hissediliyor
		this->SetNumFrames(4);
		break;
	case S_RUNL:
		this->SetBitmap(_bCharAnimRunL);
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + _bCharAnimRunL->GetWidth(), this->GetPosition().top + _bCharAnimRunL->GetHeight() });

		this->SetNumFrames(6);
		this->SetFrameDelay(1);

		break;
	case S_RUNR:
		this->SetBitmap(_bCharAnimRunR);
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + _bCharAnimRunR->GetWidth(), this->GetPosition().top + _bCharAnimRunR->GetHeight() });

		this->SetNumFrames(6);
		this->SetFrameDelay(1);

		break;
	case S_LJUMP:
		this->SetVelocity(0, -50);
		this->SetBitmap(_bCharAnimJumpL); // bir kere bastıktan sonra 7 kareyi de basması ve animasyonu bitirmesi lazım
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + _bCharAnimJumpL->GetWidth(), this->GetPosition().top + _bCharAnimJumpL->GetHeight() });

		this->SetNumFrames(7);
		this->SetFrameDelay(2);
		this->SetAnimDef(FALSE);
		break;
	case S_RJUMP:
		this->SetVelocity(0, -50);
		this->SetBitmap(_bCharAnimJumpR); // bir kere bastıktan sonra 7 kareyi de basması ve animasyonu bitirmesi lazım
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + _bCharAnimJumpR->GetWidth(), this->GetPosition().top + _bCharAnimJumpR->GetHeight() });

		this->SetNumFrames(7);
		this->SetFrameDelay(2);
		this->SetAnimDef(FALSE);
		break;
	case S_RFIRE:
		this->SetBitmap(_bCharFire); // bir kere bastıktan sonra 7 kareyi de basması ve animasyonu bitirmesi lazım
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + _bCharFireL->GetWidth(), this->GetPosition().top + _bCharFireL->GetHeight() });

		this->SetNumFrames(2);
		this->SetFrameDelay(1);
		this->SetAnimDef(FALSE);
		break;
	case S_LFIRE:
		this->SetBitmap(_bCharFireL); // bir kere bastıktan sonra 7 kareyi de basması ve animasyonu bitirmesi lazım
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + _bCharFireL->GetWidth(), this->GetPosition().top + _bCharFireL->GetHeight() });

		this->SetNumFrames(2);
		this->SetFrameDelay(1);
		this->SetAnimDef(FALSE);
		break;
	default:
		break;
	}
}

FireBurst* PlayerCharacter::fire()
{
	FireBurst* fb = new FireBurst();
	if (_bCharState == S_LFIRE) {
		
		fb->SetBitmap(_bCharFireBurstL);
		fb->SetPosition(this->GetPosition().left,this->GetPosition().top +10);
		
		fb->SetVelocity(-50,0);
		
	}
	else {
		
		fb->SetBitmap(_bCharFireBurstR);
		fb->SetPosition(this->GetPosition().right, this->GetPosition().top + 10);
		
		fb->SetVelocity(50, 0);
		
		
	}
	fb->SetNumFrames(6);
	fb->SetFrameDelay(1);
	fb->SetBounds(RECT{ 0, 0, 1920, 1020 });
	fb->SetBoundsAction(BA_DIE);
	return fb;
}

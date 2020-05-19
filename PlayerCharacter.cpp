#include "PlayerCharacter.h"


PlayerCharacter::PlayerCharacter(HDC hDC, HINSTANCE _hInstance) : Character(hDC) // default frame number and delay canbe parameters
{
	_bCharAnimIdle = new Bitmap(hDC, IDB_CHAR_IDLE_R, _hInstance);
	_bCharAnimRunR = new Bitmap(hDC, IDB_CHAR_RUN_R, _hInstance);
	_bCharAnimRunL = new Bitmap(hDC, IDB_CHAR_RUN_L, _hInstance);
	_bCharAnimJumpR = new Bitmap(hDC, IDB_CHAR_JUMP_R, _hInstance);
	_bCharAnimJumpL = new Bitmap(hDC, IDB_CHAR_JUMP_L, _hInstance);
	_bCharFire = new Bitmap(hDC, IDB_CHAR_FIREBURST_R, _hInstance);
	_bCharFireL = new Bitmap(hDC, IDB_CHAR_FIREBURST_L, _hInstance);
	_bCharAnimAttR = new Bitmap(hDC, IDB_CHAR_ATT_R, _hInstance);
	_bCharAnimAttL = new Bitmap(hDC, IDB_CHAR_ATT_L, _hInstance);


	_bCharFireBurstR = new Bitmap(hDC, IDB_FIREBURST_R, _hInstance);
	_bCharFireBurstL = new Bitmap(hDC, IDB_FIREBURST_L, _hInstance);

	SetRect(&m_rcBounds, 0, 0, 1920, 1030);
	
	this->SetBitmap(_bCharAnimIdle);
	SetRect(&m_rcPosition, 0, 0, _bCharAnimIdle->GetWidth(), _bCharAnimIdle->GetHeight());
	this->SetPosition(320, 1020);
	this->SetNumFrames(4);
	this->SetFrameDelay(5);

	int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 5;
	int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 5;
	CopyRect(&m_rcCollision, &m_rcPosition);

	deathMark = FALSE;

	InflateRect(&m_rcCollision, iXShrink, iYShrink);
	this->fireCounter = 10;

	
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
	case S_RATT:
		this->SetBitmap(_bCharAnimAttR); // bir kere bastıktan sonra 7 kareyi de basması ve animasyonu bitirmesi lazım
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + _bCharFireL->GetWidth(), this->GetPosition().top + _bCharFireL->GetHeight() });

		this->SetNumFrames(6);
		this->SetFrameDelay(1);
		this->SetAnimDef(FALSE);
		break;
	case S_LATT:
		this->SetBitmap(_bCharAnimAttL); // bir kere bastıktan sonra 7 kareyi de basması ve animasyonu bitirmesi lazım
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + _bCharFireL->GetWidth(), this->GetPosition().top + _bCharFireL->GetHeight() });

		this->SetNumFrames(6);
		this->SetFrameDelay(1);
		this->SetAnimDef(FALSE);
		break;
	default:
		break;
	}
}

void PlayerCharacter::die()
{
}

FireBurst* PlayerCharacter::fire()
{
	FireBurst* fb = new FireBurst();
	if (_bCharState == S_LFIRE) {
		
		fb->SetBitmap(_bCharFireBurstL);
		fb->SetPosition(this->GetPosition().left,this->GetPosition().top +10);
		
		fb->SetVelocity(-30,0);
		
	}
	else {
		
		fb->SetBitmap(_bCharFireBurstR);
		fb->SetPosition(this->GetPosition().right, this->GetPosition().top + 10);
		
		fb->SetVelocity(30, 0);
		
		
	}
	fb->SetNumFrames(6);
	fb->SetFrameDelay(1);
	fb->SetBounds(RECT{ 50, 50, 1870, 1030 });
	fb->SetBoundsAction(BA_DIE);
	return fb;
}

﻿#include "SpellCaster.h"

void SpellCaster::act(int a) {
	int rng = rand() % 5;
	
	if (rng == 6) {
		this->changeState(S_RUNL);
	}
	else if (rng == 7) {
		this->changeState(S_RUNR);
	}
}
SpellCaster::SpellCaster(HDC hDC) : SimpleAI(hDC) {
	// bitmap load
	_bCharAnimIdle = new Bitmap(hDC, TEXT("resources/wizard_idle.bmp"));
	_bCharAnimIdleL = new Bitmap(hDC, TEXT("resources/wizard_idle_l.bmp"));
	_bCharAnimRunR = new Bitmap(hDC, TEXT("resources/wizard_idle.bmp"));
	_bCharAnimRunL = new Bitmap(hDC, TEXT("resources/wizard_idle_l.bmp"));
	_bCharAnimDeathR = new Bitmap(hDC, TEXT("resources/wizard_death.bmp"));
	_bCharAnimDeathL = new Bitmap(hDC, TEXT("resources/wizard_death_l.bmp"));

	_bSpell = new Bitmap(hDC, TEXT("resources/spell.bmp"));
	SetRect(&m_rcBounds, 0, 0, 1920, 1020);
	this->SetBitmap(_bCharAnimIdleL);
	SetRect(&m_rcPosition, 0, 0, _bCharAnimIdle->GetWidth(), _bCharAnimIdle->GetHeight());
	SetBoundsAction(BA_HALT);
	this->SetNumFrames(10);
	this->SetFrameDelay(2);
	this->m_iZOrder = 10;
	deathMark = FALSE;
	
}

SpellCaster::~SpellCaster() {
	// bitmap load
	delete _bCharAnimIdle;
	delete _bCharAnimIdleL;
	delete _bCharAnimRunR;
	delete _bCharAnimRunL;
	delete _bCharAnimDeathR;
	delete _bCharAnimDeathL;
	delete _bSpell ;
	

}

Spell* SpellCaster::fire(POINT target)
{
	return new Spell(_bSpell, target, this->GetPosition().left+(this->GetWidth()/2), this->GetPosition().top + (this->GetHeight() / 2)); // ai karakterin önünden spawn olan bir spell döndür
					// döndürdüğü değeri oyundan engine a ekle
					// üsttekilerin olması için spell classı tutan bir vektörü scene de yarat
}
void SpellCaster::changeState(STATE state) {
		_bCharState = state;
		this->m_iCurFrame = 0;
		
		switch (state)
		{
		case S_IDLE:
			this->SetBitmap(_bCharAnimIdleL);
			this->SetNumFrames(10);
			this->SetFrameDelay(2);
			break;
		case S_IDLEL:
			this->SetBitmap(_bCharAnimIdle);
			this->SetNumFrames(10);
			this->SetFrameDelay(2);
			break;
		case S_RUNL:
			this->SetBitmap(_bCharAnimRunL);
			this->SetVelocity(-5,0);
			this->SetNumFrames(10);
			this->SetFrameDelay(2);
			break;
		case S_RUNR:
			this->SetBitmap(_bCharAnimRunR);
			this->SetVelocity(5, 0);
			break;
		case S_RDEATH:
			this->SetBitmap(_bCharAnimDeathR);
			this->SetAnimDef(FALSE);
			//this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + _bCharAnimDeathR->GetWidth(), this->GetPosition().top + _bCharAnimJumpL->GetHeight() });

			this->SetNumFrames(10);
			this->SetFrameDelay(2);
			break;
		case S_LDEATH:
			this->SetBitmap(_bCharAnimDeathL);
			this->SetAnimDef(FALSE);
			//this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + _bCharAnimDeathL->GetWidth(), this->GetPosition().top + _bCharAnimJumpL->GetHeight() });

			this->SetNumFrames(10);
			this->SetFrameDelay(2);
			break;
		
			
			
		}
		
	

}
void SpellCaster::die() {
	deathMark = TRUE;
	STATE state = (m_pBitmap == _bCharAnimRunR || m_pBitmap == _bCharAnimIdle) ? S_LDEATH : S_RDEATH;
	changeState(state);
}
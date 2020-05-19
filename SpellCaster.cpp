#include "SpellCaster.h"

void SpellCaster::act(int a) {
	int rng = rand() % 5;
	
	if (rng == 6) {
		this->changeState(S_RUNL);
	}
	else if (rng == 7) {
		this->changeState(S_RUNR);
	}
}
SpellCaster::SpellCaster(HDC hDC, HINSTANCE _hInstance) : SimpleAI(hDC) {
	// bitmap load

	_bCharAnimIdle = new Bitmap(hDC, IDB_SPELLCASTER_R, _hInstance);
	_bCharAnimIdleL = new Bitmap(hDC, IDB_SPELLCASTER_IDLE_L, _hInstance);
	_bCharAnimRunR = new Bitmap(hDC, IDB_SPELLCASTER_R, _hInstance);
	_bCharAnimRunL = new Bitmap(hDC, IDB_SPELLCASTER_IDLE_L, _hInstance);
	_bCharAnimDeathR = new Bitmap(hDC, IDB_SPELLCASTER_DEATH_R, _hInstance);
	_bCharAnimDeathL = new Bitmap(hDC, IDB_SPELLCASTER_DEATH_L, _hInstance);

	_bSpell = new Bitmap(hDC, TEXT("resources/spell.bmp"));
	SetRect(&m_rcBounds, 0, 0, 1920, 1030);
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
			this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + m_pBitmap->GetWidth(), this->GetPosition().top + m_pBitmap->GetHeight() });

			this->SetNumFrames(10);
			this->SetFrameDelay(2);
			break;
		case S_IDLEL:
			this->SetBitmap(_bCharAnimIdle);
			this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + m_pBitmap->GetWidth(), this->GetPosition().top + m_pBitmap->GetHeight() });

			this->SetNumFrames(10);
			this->SetFrameDelay(2);
			break;
		case S_RUNL:
			this->SetBitmap(_bCharAnimRunL);
			this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + m_pBitmap->GetWidth(), this->GetPosition().top + m_pBitmap->GetHeight() });

			this->SetVelocity(-5,0);
			this->SetNumFrames(10);
			this->SetFrameDelay(2);
			break;
		case S_RUNR:
			this->SetBitmap(_bCharAnimRunR);
			this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + m_pBitmap->GetWidth(), this->GetPosition().top + m_pBitmap->GetHeight() });

			this->SetVelocity(5, 0);
			break;
		case S_RDEATH:
			this->SetBitmap(_bCharAnimDeathR);
			this->SetAnimDef(FALSE);
			this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + m_pBitmap->GetWidth(), this->GetPosition().top + m_pBitmap->GetHeight() });
			this->SetVelocity(0, 0);
			this->SetNumFrames(10);
			this->SetFrameDelay(2);
			break;
		case S_LDEATH:
			this->SetBitmap(_bCharAnimDeathL);
			this->SetAnimDef(FALSE);
			this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + m_pBitmap->GetWidth(), this->GetPosition().top + m_pBitmap->GetHeight() });
			this->SetVelocity(0, 0);
			this->SetNumFrames(10);
			this->SetFrameDelay(2);
			break;
		
			
			
		}
		
	

}
void SpellCaster::die() {
	if (!deathMark) {
		deathMark = TRUE;
		STATE state = (m_pBitmap == _bCharAnimRunR || m_pBitmap == _bCharAnimIdle ) ? S_LDEATH : S_RDEATH;
		changeState(state);
	}
}
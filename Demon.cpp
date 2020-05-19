#include "Demon.h"

Demon::Demon(HDC hDC, HINSTANCE _hInstance) :  SimpleAI(hDC){

	this->_bCharAnimRunR = new Bitmap(hDC, IDB_DEMON_RUN_R, _hInstance);
	this->_bCharAnimRunL = new Bitmap(hDC, IDB_DEMON_RUN_L, _hInstance);
	this->_bCharAnimAttR = new Bitmap(hDC, IDB_DEMON_ATT_R, _hInstance);
	this->_bCharAnimAttL = new Bitmap(hDC, IDB_DEMON_ATT_L, _hInstance);
	this->_bCharAnimDeathR = new Bitmap(hDC, IDB_DEMON_DEATH_L, _hInstance);
	this->_bCharAnimDeathL = new Bitmap(hDC, IDB_DEMON_DEATH_R, _hInstance);

	SetRect(&m_rcBounds, 0, 0, 1920, 1030);
	this->SetBitmap(_bCharAnimRunR);
	SetRect(&m_rcPosition, 0, 0, _bCharAnimRunR->GetWidth(), _bCharAnimRunR->GetHeight());
	SetBoundsAction(BA_HALT);
	this->SetNumFrames(13);
	this->SetFrameDelay(1);
	this->m_iZOrder = 10;


	deathMark = FALSE;
}
Demon::~Demon() {
	// bitmap load
	delete _bCharAnimAttR;
	delete _bCharAnimRunR;
	delete _bCharAnimRunL;
	delete _bCharAnimAttL;


}
void Demon::changeState(STATE state) {
	_bCharState = state;
	this->m_iCurFrame = 0;

	switch (state)
	{
	case S_RUNL:
		this->SetBitmap(_bCharAnimRunL);
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + m_pBitmap->GetWidth(), this->GetPosition().top + m_pBitmap->GetHeight() });
		this->SetVelocity(-2, 0);
		this->SetNumFrames(13);
		this->SetFrameDelay(1);

		break;
	case S_RUNR:
		this->SetBitmap(_bCharAnimRunR);
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + m_pBitmap->GetWidth(), this->GetPosition().top + m_pBitmap->GetHeight() });
		this->SetVelocity(2, 0);
		this->SetNumFrames(13);
		this->SetFrameDelay(1);

		break;
	case S_RDEATH:
		this->SetBitmap(_bCharAnimDeathR);
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + m_pBitmap->GetWidth(), this->GetPosition().top + m_pBitmap->GetHeight() });

		this->SetAnimDef(FALSE);
		this->SetVelocity(0, 0);
		this->SetNumFrames(8);
		this->SetFrameDelay(2);

		break;
	case S_LDEATH:
		this->SetBitmap(_bCharAnimDeathL);
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + m_pBitmap->GetWidth(), this->GetPosition().top + m_pBitmap->GetHeight() });

		this->SetAnimDef(FALSE);
		this->SetVelocity(0, 0);
		this->SetNumFrames(8);
		this->SetFrameDelay(2);

		break;
	case S_RATT:
		this->SetBitmap(_bCharAnimAttR);
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + m_pBitmap->GetWidth(), this->GetPosition().top + m_pBitmap->GetHeight() });

		this->SetAnimDef(FALSE);
		this->SetVelocity(0, 0);
		this->SetNumFrames(18);
		this->SetFrameDelay(1);

		break;
	case S_LATT:
		this->SetBitmap(_bCharAnimAttL);
		this->SetPosition(RECT{ this->GetPosition().left,this->GetPosition().top, this->GetPosition().left + m_pBitmap->GetWidth(), this->GetPosition().top + m_pBitmap->GetHeight() });

		this->SetAnimDef(FALSE);
		this->SetVelocity(0, 0);
		this->SetNumFrames(18);
		this->SetFrameDelay(1);

		break;



	}



}
void Demon::act(int a) {
	int rng = rand() % 50;

	if (rng == 0 && IsAnimDef()) {
		STATE state = (m_pBitmap == _bCharAnimRunR ) ? S_RUNL : S_RUNR;
		changeState(state);
	}
	if (IsAnimDef() &&  !checkState(S_RUNL) && !checkState(S_RUNR)) {
		changeState(S_RUNL);
	}
	
}

void Demon::die()
{
	
		deathMark = TRUE;
		STATE state = (m_pBitmap == _bCharAnimRunR) ? S_LDEATH : S_RDEATH;
		changeState(state);
	
}

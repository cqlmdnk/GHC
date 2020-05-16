#include "Demon.h"

Demon::Demon(HDC hDC) :  SimpleAI(hDC){

	this->_bCharAnimRunR = new Bitmap(hDC, TEXT("resources/demon_walk.bmp"));
	this->_bCharAnimRunL = new Bitmap(hDC, TEXT("resources/demon_walk_l.bmp"));
	this->_bCharAnimAttR = new Bitmap(hDC, TEXT("resources/demon_attack.bmp"));
	this->_bCharAnimAttL = new Bitmap(hDC, TEXT("resources/demon_attack_l.bmp"));
	this->_bCharAnimDeathL = new Bitmap(hDC, TEXT("resources/demon_death.bmp"));
	this->_bCharAnimDeathR = new Bitmap(hDC, TEXT("resources/demon_death_l.bmp"));

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



	}



}
void Demon::act(int a) {
	int rng = rand() % 50;

	if (rng == 0 ) {
		STATE state = (m_pBitmap == _bCharAnimRunR ) ? S_RUNL : S_RUNR;
		this->changeState(state);
	}
	
}

void Demon::die()
{
	
		deathMark = TRUE;
		STATE state = (m_pBitmap == _bCharAnimRunR) ? S_LDEATH : S_RDEATH;
		changeState(state);
	
}

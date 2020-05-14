#include "Demon.h"

Demon::Demon(HDC hDC) :  SimpleAI(hDC){

	this->_bCharAnimRunR = new Bitmap(hDC, TEXT("resources/demon_walk.bmp"));
	this->_bCharAnimRunL = new Bitmap(hDC, TEXT("resources/demon_walk_l.bmp"));
	this->_bCharAnimAttR = new Bitmap(hDC, TEXT("resources/demon_attack.bmp"));
	this->_bCharAnimAttL = new Bitmap(hDC, TEXT("resources/demon_attack_l.bmp"));

	
	this->SetBitmap(_bCharAnimRunR);
	SetRect(&m_rcPosition, 0, 0, _bCharAnimRunR->GetWidth(), _bCharAnimRunR->GetHeight());
	this->SetVelocity(5, 0);
	this->SetNumFrames(13);
	this->SetFrameDelay(1);
	SetRect(&m_rcBounds, 0, 0, 1920, 1020);
	SetBoundsAction(BA_HALT);

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
		this->SetVelocity(-5, 0);
		this->SetNumFrames(13);
		this->SetFrameDelay(1);
		break;
	case S_RUNR:
		this->SetBitmap(_bCharAnimRunR);
		this->SetVelocity(5, 0);
		this->SetNumFrames(13);
		this->SetFrameDelay(1);
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
void Demon::act(int a) {
	int rng = rand() % 5;

	if (rng == 6) {
		this->changeState(S_RUNL);
	}
	else if (rng == 7) {
		this->changeState(S_RUNR);
	}
}
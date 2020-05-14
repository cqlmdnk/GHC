#include "Spell.h"
#include <math.h>

Spell::Spell(Bitmap* _bitmap, POINT target, int x, int y)
{
	
	this->m_pBitmap = _bitmap;
	SetRect(&m_rcBounds, 20, 20, 1880, 1060);
	this->SetPosition(RECT{ x,y , x + this->m_pBitmap->GetWidth() , y + this->m_pBitmap->GetHeight() });
	this->target = target;
	this->SetNumFrames(5);
	this->SetFrameDelay(2);
	this->SetBoundsAction(BA_DIE);
	int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 3;
	int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 3;
	CopyRect(&m_rcCollision, &m_rcPosition);
	this->lifeTime = 90;

	this->m_iZOrder = 11;
	InflateRect(&m_rcCollision, iXShrink, iYShrink);
	deathMark = FALSE;

}

void Spell::calcNextPos(int y)
{
	// vector operations to target
	//unit vector will be multiplied with constant factor
	int factor = 10;
	this->target.y = y;
	double magnitude = sqrt(pow(double(this->target.x) - this->GetPosition().left, 2) + pow(double(this->target.y)  - this->GetPosition().top + sin(lifeTime)*5, 2 ));
	double xVector = ((double(this->target.x) - this->GetPosition().left) / magnitude) * log(magnitude);
	double yVector =  ((this->target.y + ((200 * (double)rand() / RAND_MAX) - 100) - this->GetPosition().top) / magnitude) ;

	POINT p = { int(xVector),int(yVector) };
	this->SetVelocity(p.x, p.y);
}

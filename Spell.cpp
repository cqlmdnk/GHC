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
}

void Spell::calcNextPos()
{
	// vector operations to target
	
	POINT p = { ((this->target.x - this->GetPosition().left) / 20), (this->target.y+ ((200 * (double)rand() / RAND_MAX)-100) -this->GetPosition().top ) / 20 };
	this->SetVelocity(p.x, p.y);
}

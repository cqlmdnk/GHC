#include "Spell.h"
#include <math.h>

Spell::Spell(Bitmap* _bitmap, POINT target)
{

	this->m_pBitmap = _bitmap;
	SetRect(&m_rcBounds, 0, 0, 1920, 1080);
	this->SetPosition(RECT{ 0,0 , this->m_pBitmap->GetWidth() , this->m_pBitmap->GetHeight() });
	
	this->target = target;
	this->SetNumFrames(5);
	this->SetFrameDelay(2);
	this->SetBoundsAction(BA_DIE);
	int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 3;
	int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 3;
	CopyRect(&m_rcCollision, &m_rcPosition);
	this->lifeTime = 150;

	this->m_iZOrder = 11;
	InflateRect(&m_rcCollision, iXShrink, iYShrink);
	deathMark = FALSE;
	
}

void Spell::calcNextPos(int x,int y)
{
	// vector operations to target
	//unit vector will be multiplied with constant factor
	int factor = 5;
	this->target.y = y;
	this->target.x = x;
	int xDist = (this->target.x - this->GetPosition().left);
	int yDist = (this->target.y - this->GetPosition().top);

	double magnitude = sqrt(pow(xDist, 2) + pow(yDist, 2))/factor;

	int xVector = xDist / magnitude;
	int yVector = yDist / magnitude;

	POINT p = { xVector,yVector };
	this->SetVelocity(p.x, p.y);
}
SPRITEACTION Spell::Update(int** map, int x)
{
	UpdateFrame();

	// Update the position
	POINT ptNewPosition, ptSpriteSize, ptBoundsSize;


	ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
	ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
	ptSpriteSize.x = m_rcPosition.right - m_rcPosition.left;
	ptSpriteSize.y = m_rcPosition.bottom - m_rcPosition.top;
	ptBoundsSize.x = m_rcBounds.right - m_rcBounds.left;
	ptBoundsSize.y = m_rcBounds.bottom - m_rcBounds.top;



	if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left ||
		ptNewPosition.x > m_rcBounds.right ||
		(ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top ||
		ptNewPosition.y > m_rcBounds.bottom)
		return SA_KILL;


	SetPosition(ptNewPosition);

	return SA_NONE;

}

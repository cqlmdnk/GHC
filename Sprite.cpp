#include "Sprite.h"

Sprite::Sprite(Bitmap* pBitmap)
{	 // Initialize the member variables
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = 0;
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	SetRect(&m_rcBounds, 0, 0, 640, 480);
	m_baBoundsAction = BA_STOP;
	m_bHidden = FALSE;
	deathMark = FALSE;

}

Sprite::Sprite(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction)
{
	// Calculate a random position
	int iXPos = rand() % (rcBounds.right - rcBounds.left);
	int iYPos = rand() % (rcBounds.bottom - rcBounds.top);
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = 0;
	// Initialize the member variables
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, iXPos, iYPos, iXPos + pBitmap->GetWidth(),
		iYPos + pBitmap->GetHeight());
	m_ptVelocity.x = m_ptVelocity.y = 0;
	m_iZOrder = 0;
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;
	m_bHidden = FALSE;
}

Sprite::Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity,
	int iZOrder, RECT& rcBounds, BOUNDSACTION baBoundsAction)
{
	m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = 0;
	// Initialize the member variables
	m_pBitmap = pBitmap;
	SetRect(&m_rcPosition, ptPosition.x, ptPosition.y, pBitmap->GetWidth(),
		pBitmap->GetHeight());
	m_ptVelocity = ptPosition;
	m_iZOrder = iZOrder;
	CopyRect(&m_rcBounds, &rcBounds);
	m_baBoundsAction = baBoundsAction;


	m_bHidden = FALSE;

}

Sprite::~Sprite()
{

}
SPRITEACTION Sprite::Update(int** map, int x)
{
	UpdateFrame();
	bool top = false, bottom = false, right = false, left = false;

	this->GetAbsX();
	//blok değişimi kontrolü

	if (!(m_rcPosition.bottom > 1080) && !(m_rcPosition.left > 1920) && !(m_rcPosition.left < 0)) {
		if (((m_rcPosition.left + m_ptVelocity.x + (x % PLATFORM_S)) / PLATFORM_S != m_rcPosition.left / PLATFORM_S) + (x % PLATFORM_S) &&
			map[(m_rcPosition.left + m_ptVelocity.x + (x % PLATFORM_S)) / PLATFORM_S][(m_rcPosition.bottom + m_rcPosition.top) / (2 * PLATFORM_S)] == 1)
			left = true;
		if (((m_rcPosition.right + m_ptVelocity.x + (x % PLATFORM_S)) / PLATFORM_S != m_rcPosition.right / PLATFORM_S) + (x % PLATFORM_S) &&
			map[(m_rcPosition.right + m_ptVelocity.x + (x % PLATFORM_S)) / PLATFORM_S][(m_rcPosition.bottom + m_rcPosition.top) / (2 * PLATFORM_S)] == 1)
			right = true;
		bool leftWBias  = map[(( m_rcPosition.left+ m_rcPosition.right)/2 + (x % PLATFORM_S)) / ( PLATFORM_S)][(m_rcPosition.bottom + m_ptVelocity.y) / PLATFORM_S] == 1;
		//bool rightWBias = map[( m_rcPosition.right-10 + (x % PLATFORM_S)) / ( PLATFORM_S)][(m_rcPosition.bottom + m_ptVelocity.y) / PLATFORM_S] == 1;
		int tempVelY = m_ptVelocity.y;
		int sign = (tempVelY > 0) - (tempVelY < 0);
		int memVelY = m_ptVelocity.y;
		m_ptVelocity.y = 0;
		while (abs(tempVelY) != 0) {
			m_ptVelocity.y += sign * 10;
			if (((m_rcPosition.bottom + m_ptVelocity.y) / PLATFORM_S != m_rcPosition.bottom / PLATFORM_S) && (leftWBias))//|| rightWBias))
			{
				bottom = true;
				break;
			}
				
			if (((m_rcPosition.top + m_ptVelocity.y) / 10 != m_rcPosition.top / 10) &&
				map[((m_rcPosition.left + m_rcPosition.right) + (x % PLATFORM_S)) / (2 * PLATFORM_S)][(m_rcPosition.top + m_ptVelocity.y) / PLATFORM_S] == 1 &&
				(m_rcPosition.top + m_ptVelocity.y) % 60 <= 30) {
				top = true;
				break;
			}
				
			tempVelY -= sign * 10;
		}
		if (!bottom && !top)
			m_ptVelocity.y = memVelY;
		
		
	}





	// Update the position
	POINT ptNewPosition, ptSpriteSize, ptBoundsSize;


	ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
	ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
	ptSpriteSize.x = m_rcPosition.right - m_rcPosition.left;
	ptSpriteSize.y = m_rcPosition.bottom - m_rcPosition.top;
	ptBoundsSize.x = m_rcBounds.right - m_rcBounds.left;
	ptBoundsSize.y = m_rcBounds.bottom - m_rcBounds.top;




	// en yakın blok bottom a set edilecek







	//write mix of BA_HALT and BA_BOUNCE


	if (m_baBoundsAction == BA_STOP) {

		if (ptNewPosition.x < m_rcBounds.left ||
			ptNewPosition.x >(m_rcBounds.right - ptSpriteSize.x))
		{
			ptNewPosition.x = max(m_rcBounds.left, min(ptNewPosition.x,
				m_rcBounds.right - ptSpriteSize.x));

			SetVelocity(0, m_ptVelocity.y);
		}


		if (ptNewPosition.y < m_rcBounds.top ||
			ptNewPosition.y >(m_rcBounds.bottom - ptSpriteSize.y))
		{
			ptNewPosition.y = max(m_rcBounds.top, min(ptNewPosition.y,
				m_rcBounds.bottom - ptSpriteSize.y));
			SetVelocity(m_ptVelocity.x, 0);
		}
	}
	else if (m_baBoundsAction == BA_HALT) {

		if (ptNewPosition.x < m_rcBounds.left ||
			ptNewPosition.x >(m_rcBounds.right - ptSpriteSize.x))
		{
			
			m_bStateHalt = TRUE; //sleep and wake functions can be written for this

			SetAbsX(x); //in case of halt save x position;
		}
		if (ptNewPosition.y < m_rcBounds.top ||
			ptNewPosition.y >(m_rcBounds.bottom - ptSpriteSize.y))
		{
			ptNewPosition.y = max(m_rcBounds.top, min(ptNewPosition.y,
				m_rcBounds.bottom - ptSpriteSize.y));
			SetVelocity(m_ptVelocity.x, 0);
		}

		


	}

	else if (m_baBoundsAction == BA_DIE)
	{
		if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left ||
			ptNewPosition.x > m_rcBounds.right ||
			(ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top ||
			ptNewPosition.y > m_rcBounds.bottom)
			return SA_KILL;
	}
	else if (m_baBoundsAction == BA_BOUNCE)
	{
		BOOL bBounce = FALSE;
		POINT ptNewVelocity = m_ptVelocity;

		//Bounce off left bound
		if ( left)
		{
			bBounce = TRUE;
			ptNewPosition.x = ptNewPosition.x - ptNewVelocity.x;
			ptNewVelocity.x = -ptNewVelocity.x;
		}

		//Bounce off right bound
		else if ( right)
		{
			bBounce = TRUE;
			ptNewPosition.x = ptNewPosition.x - ptNewVelocity.x;
			ptNewVelocity.x = -ptNewVelocity.x;
		}

		//Bounce off top bounnd
		if (ptNewPosition.y < m_rcBounds.top || top)
		{
			bBounce = TRUE;
			ptNewPosition.y = ptNewPosition.y - ptNewVelocity.y;
			ptNewVelocity.y = -ptNewVelocity.y;
		}

		//Bounce off bottom bound
		else if ((ptNewPosition.y + ptSpriteSize.y) > m_rcBounds.bottom ||bottom)
		{
			bBounce = TRUE;
			ptNewPosition.y = ptNewPosition.y - ptNewVelocity.y;
			ptNewVelocity.y = -ptNewVelocity.y;
		}

		if (bBounce)
			SetVelocity(ptNewVelocity);
		if (ptNewPosition.x < m_rcBounds.left)
		{
			m_bStateHalt = TRUE; //sleep and wake functions can be written for this
			m_ptVelocity.x = 10;
			SetPosition(GetPosition().left - GetPosition().left % PLATFORM_S, GetPosition().top);

			SetAbsX(x); //in case of halt save x position;
			
		}
		else if (ptNewPosition.x > (m_rcBounds.right - ptSpriteSize.x)) {
			m_bStateHalt = TRUE; //sleep and wake functions can be written for this
			m_ptVelocity.x = -10;

			SetPosition(GetPosition().left - GetPosition().left%PLATFORM_S, GetPosition().top);
			SetAbsX(x+1920); //in case of halt save x position;
		}
		
	}


	this->SetVelocity(this->GetVelocity().x, this->GetVelocity().y + 10);
	if (right) {
		if (GetVelocity().x > 0) {
			ptNewPosition.x = (ptNewPosition.x - m_ptVelocity.x);
			SetVelocity(0, this->GetVelocity().y);
		}
	}
	if (left) {
		if (GetVelocity().x < 0) {
			ptNewPosition.x = (ptNewPosition.x - m_ptVelocity.x);
			SetVelocity(0, this->GetVelocity().y);
		}
	}

	if (top) {
		if (GetVelocity().y < 0) {
			int sign = (m_ptVelocity.y > 0) - (m_ptVelocity.y < 0);
			ptNewPosition.y = (ptNewPosition.y + m_ptVelocity.y - 10*sign);
			SetVelocity(this->GetVelocity().x, 0);
		}
	}
	if (bottom) {
		if (GetVelocity().y > 0) {
			ptNewPosition.y = (((ptNewPosition.y + GetHeight()) / PLATFORM_S) * PLATFORM_S) - this->GetHeight() - 1;
			SetVelocity(this->GetVelocity().x, 0);
		}
	}



	

	SetPosition(ptNewPosition);

	return SA_NONE;

}

void Sprite::Draw(HDC hDC)
{
	// Draw the sprite if it isn’t hidden
	if (m_pBitmap != NULL && !m_bHidden)
	{
		int a = GetWidth();
		// Draw the appropriate frame, if necessary
		if (m_iNumFrames == 1)
			m_pBitmap->Draw(hDC, m_rcPosition.left, m_rcPosition.top, TRUE);
		else
			m_pBitmap->DrawPart(hDC, m_rcPosition.left, m_rcPosition.top,
				m_iCurFrame * GetWidth(), 0, GetWidth(), GetHeight(), TRUE);

	}
}





inline void Sprite::UpdateFrame()
{
	if ((m_iFrameDelay >= 0) && (--m_iFrameTrigger <= 0))
	{
		// Reset the frame trigger;
		m_iFrameTrigger = m_iFrameDelay;

		// Increment the frame
		if (++m_iCurFrame >= m_iNumFrames) { // jump bitince burası state i değiştirecek
			m_iCurFrame = 0;
			this->SetAnimDef(TRUE);
		}
	}

}
void Sprite::SetNumFrames(int iNumFrames)
{
	// Set the number of frames
	m_iNumFrames = iNumFrames;
	// Recalculate the position
	RECT rect = GetPosition();
	rect.right = rect.left + ((rect.right - rect.left) / iNumFrames);
	SetPosition(rect);
}
#include "Tile.h"
Tile::Tile(HDC hDC, HINSTANCE _hInstance) {
	this->m_pBitmap = new Bitmap(hDC, IDB_MTILE_1, _hInstance);
	this->SetBoundsAction(BA_DIE);
	SetRect(&m_rcBounds, 0, 0, 1920, 1080);
	deathMark = FALSE;
	SetBoundsAction(BA_BOUNCE);

	m_bHidden = FALSE;
	m_bStateHalt = FALSE;
	SetNumFrames(1);
	SetRect(&m_rcPosition, 0, 0, this->m_pBitmap->GetWidth(), this->m_pBitmap->GetHeight());

	//int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 5;
	//int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 5;
	CopyRect(&m_rcCollision, &m_rcPosition);



	//InflateRect(&m_rcCollision, iXShrink, iYShrink);
}

Tile::~Tile()
{
	delete m_pBitmap;
}

SPRITEACTION Tile::Update(int** map, int x) // always bounce&halt
{
	UpdateFrame();
	bool top = false, bottom = false, right = false, left = false;

	this->GetAbsX();
	//blok değişimi kontrolü

	int xShiftPlatfrom = 120;
	if (x < 120)
		xShiftPlatfrom = 0;

	SetPosition(GetPosition().left + xShiftPlatfrom, GetPosition().top);


	if (!(m_rcPosition.bottom > 1080) && !(m_rcPosition.left > 1920) && !(m_rcPosition.left < 0)) {
		if (((m_rcPosition.left + m_ptVelocity.x + (x % PLATFORM_S)) / PLATFORM_S != m_rcPosition.left / PLATFORM_S) + (x % PLATFORM_S) &&
			map[(m_rcPosition.left + m_ptVelocity.x + (x % PLATFORM_S)) / PLATFORM_S][(m_rcPosition.bottom + m_rcPosition.top) / (2 * PLATFORM_S)] == 1)
			left = true;
		if (((m_rcPosition.right + m_ptVelocity.x + (x % PLATFORM_S)) / PLATFORM_S != m_rcPosition.right / PLATFORM_S) + (x % PLATFORM_S) &&
			map[(m_rcPosition.right + m_ptVelocity.x + (x % PLATFORM_S)) / PLATFORM_S][(m_rcPosition.bottom + m_rcPosition.top) / (2 * PLATFORM_S)] == 1)
			right = true;
		bool leftWBias = map[(m_rcPosition.left + (x % PLATFORM_S)) / (PLATFORM_S)][(m_rcPosition.bottom + m_ptVelocity.y) / PLATFORM_S] == 1;
		bool rightWBias = map[(m_rcPosition.right + (x % PLATFORM_S)) / (PLATFORM_S)][(m_rcPosition.bottom + m_ptVelocity.y) / PLATFORM_S] == 1;
		int tempVelY = m_ptVelocity.y;
		int sign = (tempVelY > 0) - (tempVelY < 0);
		int memVelY = m_ptVelocity.y;
		m_ptVelocity.y = 0;
		while (abs(tempVelY) != 0) {
			m_ptVelocity.y += sign * 10;
			bool leftWBias = map[(m_rcPosition.left + (x % PLATFORM_S)) / (PLATFORM_S)][(m_rcPosition.bottom + m_ptVelocity.y) / PLATFORM_S] == 1;
			bool rightWBias = map[(m_rcPosition.right + (x % PLATFORM_S)) / (PLATFORM_S)][(m_rcPosition.bottom + m_ptVelocity.y) / PLATFORM_S] == 1;
			if (((m_rcPosition.bottom + m_ptVelocity.y) / PLATFORM_S != m_rcPosition.bottom / PLATFORM_S) && (leftWBias || rightWBias))
			{
				bottom = true;
				break;
			}
			leftWBias = map[(m_rcPosition.left + (x % PLATFORM_S)) / (PLATFORM_S)][(m_rcPosition.top + m_ptVelocity.y) / PLATFORM_S] == 1;
			rightWBias = map[(m_rcPosition.right + (x % PLATFORM_S)) / (PLATFORM_S)][(m_rcPosition.top + m_ptVelocity.y) / PLATFORM_S] == 1;
			if (((m_rcPosition.top + m_ptVelocity.y) / PLATFORM_S != m_rcPosition.top / PLATFORM_S) && (leftWBias || rightWBias))
			{
				top = true;
				break;
			}

			tempVelY -= sign * 10;
		}
		if (!bottom && !top)
			m_ptVelocity.y = memVelY;


	}

	SetPosition(GetPosition().left - xShiftPlatfrom, GetPosition().top);




	// Update the position
	POINT ptNewPosition, ptSpriteSize, ptBoundsSize;


	ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
	ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
	ptSpriteSize.x = m_rcPosition.right - m_rcPosition.left;
	ptSpriteSize.y = m_rcPosition.bottom - m_rcPosition.top;
	ptBoundsSize.x = m_rcBounds.right - m_rcBounds.left;
	ptBoundsSize.y = m_rcBounds.bottom - m_rcBounds.top;




	// en yakın blok bottom a set edilecek


	BOOL bBounce = FALSE;
	POINT ptNewVelocity = m_ptVelocity;

	//Bounce off left bound
	if (left)
	{
		bBounce = TRUE;
		ptNewPosition.x = ptNewPosition.x - ptNewVelocity.x;
		ptNewVelocity.x = -ptNewVelocity.x;
	}

	//Bounce off right bound
	else if (right)
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
	else if ((ptNewPosition.y + ptSpriteSize.y) > m_rcBounds.bottom || bottom)
	{
		bBounce = TRUE;
		ptNewPosition.y = ptNewPosition.y - ptNewVelocity.y;
		ptNewVelocity.y = -ptNewVelocity.y;
	}

	if (bBounce)
		SetVelocity(ptNewVelocity);
	if (ptNewPosition.x < m_rcBounds.left - 120)
	{
		m_bStateHalt = TRUE; //sleep and wake functions can be written for this
		

	}
	else if (ptNewPosition.x > (m_rcBounds.right + 120 - ptSpriteSize.x)) {
		m_bStateHalt = TRUE; //sleep and wake functions can be written for this
	
	}


	this->SetVelocity(this->GetVelocity().x, this->GetVelocity().y + 10); // gravity kicks in



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
			ptNewPosition.y = (ptNewPosition.y + m_ptVelocity.y - 10 * sign);
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

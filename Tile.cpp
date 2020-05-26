#include "Tile.h"
Tile::Tile(HDC hDC, HINSTANCE _hInstance) {
	this->m_pBitmap = new Bitmap(hDC, IDB_TILE1, _hInstance);
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

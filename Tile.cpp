#include "Tile.h"
Tile::Tile(HDC hDC) {
	this->m_pBitmap = new Bitmap(hDC, TEXT("resources/tile1.bmp"));
	this->SetBoundsAction(BA_DIE);
	SetRect(&m_rcBounds, 0, -60, 1920, 1140);
	deathMark = FALSE;
	
	m_bHidden = FALSE;
	m_bStateHalt = FALSE;
	SetNumFrames(1);
	SetRect(&m_rcPosition, 0, 0, this->m_pBitmap->GetWidth(), this->m_pBitmap->GetHeight());
}

Tile::~Tile()
{
	delete m_pBitmap;
}

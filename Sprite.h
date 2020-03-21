#pragma once
#include "Bitmap.h"

typedef WORD BOUNDSACTION;
const BOUNDSACTION BA_STOP = 0,
BA_WRAP = 1,
BA_BOUNCE = 2,
BA_DIE = 3;

class Sprite
{
protected:
	// Member Variables
	Bitmap * m_pBitmap;
	RECT m_rcPosition;
	POINT m_ptVelocity;
	int m_iZOrder;
	RECT m_rcBounds;
	BOUNDSACTION m_baBoundsAction;
	BOOL m_bHidden;

public:
	// Constructor(s)/Destructor
	Sprite(Bitmap* pBitmap);
	Sprite(Bitmap* pBitmap, RECT& rcBounds,
		BOUNDSACTION baBoundsAction = BA_STOP);
	Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
		RECT & rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
	virtual ~Sprite();

	// General Methods
	virtual void Update();
	void Draw(HDC hDC);
	BOOL IsPointInside(int x, int y);

	// Accessor Methods
	RECT & GetPosition() { return m_rcPosition; };
	void SetPosition(int x, int y);
	void SetPosition(POINT ptPosition);
	void SetPosition(RECT& rcPosition)
	{
		CopyRect(&m_rcPosition, &rcPosition);
	};
	void OffsetPosition(int x, int y);
	POINT GetVelocity() { return m_ptVelocity; };
	void SetVelocity(int x, int y);
	void SetVelocity(POINT ptVelocity);
	BOOL GetZOrder() { return m_iZOrder; };
	void SetZOrder(int iZOrder) { m_iZOrder = iZOrder; };
	void SetBounds(RECT& rcBounds) { CopyRect(&m_rcBounds, &rcBounds); };
	void SetBoundsAction(BOUNDSACTION ba) { m_baBoundsAction = ba; };
	BOOL IsHidden() { return m_bHidden; };
	void SetHidden(BOOL bHidden) { m_bHidden = bHidden; };
	int GetWidth() { return m_pBitmap->GetWidth(); };
	int GetHeight() { return m_pBitmap->GetHeight(); };

};
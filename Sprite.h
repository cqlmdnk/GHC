#pragma once
#include "Bitmap.h"

typedef WORD        SPRITEACTION;
const SPRITEACTION  SA_NONE = 0x0000L,
SA_KILL = 0x0001L,
SA_BLOCK_RIGHT = 0x0002L,
SA_BLOCK_LEFT = 0x0003L;


typedef WORD BOUNDSACTION;
const BOUNDSACTION BA_STOP = 0,
BA_WRAP = 1,
BA_BOUNCE = 2,
BA_DIE = 3,
BA_HALT = 4;

class Sprite
{
protected:
	// Member Variables
	Bitmap * m_pBitmap;
	RECT m_rcPosition, m_rcCollision;;
	POINT m_ptVelocity;
	int m_iZOrder;
	RECT m_rcBounds;
	BOUNDSACTION m_baBoundsAction;
	BOOL m_bHidden, m_bAnimDef, m_bStateHalt;
	int m_iFrameDelay = 0, m_iFrameTrigger = 0, m_iCurFrame, m_iNumFrames;
	int m_iAbsX = 0;
	
public:
	// Constructor(s)/Destructor
	Sprite() {
		m_iCurFrame = m_iFrameDelay = m_iFrameTrigger = 0;
		m_ptVelocity.x = m_ptVelocity.y = 0;
		m_iZOrder = 0;
		m_baBoundsAction = BA_STOP;
		m_bHidden = FALSE;
		m_bAnimDef = TRUE;
		m_bStateHalt = FALSE;
	}
	Sprite(Bitmap* pBitmap);
	Sprite(Bitmap* pBitmap, RECT& rcBounds,
		BOUNDSACTION baBoundsAction = BA_STOP);
	Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
		RECT & rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
	virtual ~Sprite();
	virtual void  CalcCollisionRect();
	
	// General Methods
	virtual SPRITEACTION Update(bool top, bool bottom, bool right, bool left, int x);
	void Draw(HDC hDC);
	BOOL                  IsPointInside(int x, int y);
	BOOL                  TestCollision(Sprite* pTestSprite);

	inline void UpdateFrame();

	

	// Accessor Methods
	RECT & GetPosition() { return m_rcPosition; };
	void SetPosition(int x, int y);
	void SetPosition(POINT ptPosition);
	void SetPosition(RECT& rcPosition)
	{
		CopyRect(&m_rcPosition, &rcPosition);
	};
	void OffsetPosition(int x, int y);
	RECT&   GetCollision() { return m_rcCollision; };
	void SetNumFrames(int inFrames);
	POINT GetVelocity() { return m_ptVelocity; };
	void SetVelocity(int x, int y);
	void SetVelocity(POINT ptVelocity);
	BOOL GetZOrder() { return m_iZOrder; };
	void SetZOrder(int iZOrder) { m_iZOrder = iZOrder; };
	void SetBounds(RECT& rcBounds) { CopyRect(&m_rcBounds, &rcBounds); };
	void SetBoundsAction(BOUNDSACTION ba) { m_baBoundsAction = ba; };
	BOOL IsHidden() { return m_bHidden; };
	void SetHidden(BOOL bHidden) { m_bHidden = bHidden; };
	BOOL IsAnimDef() { return m_bAnimDef; };
	void SetAnimDef(BOOL bAnimDef) { m_bAnimDef = bAnimDef; };
	BOOL IsStateHalt() { return m_bStateHalt; };
	void SetStateHalt(BOOL bStateHalt) { m_bStateHalt = bStateHalt; };
	void SetBitmap(Bitmap* pBitmap) { m_pBitmap = pBitmap; }
	int GetWidth() { return (m_pBitmap->GetWidth() / m_iNumFrames); };
	int GetHeight() { return m_pBitmap->GetHeight(); };
	void SetAbsX(int absX) { m_iAbsX = absX; }
	int GetAbsX() { return m_iAbsX; };
	void SetFrameDelay(int iFrameDelay) { m_iFrameDelay = iFrameDelay; };
};
	inline void Sprite::CalcCollisionRect()
	{
		int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 12;
		int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 12;
		CopyRect(&m_rcCollision, &m_rcPosition);
		InflateRect(&m_rcCollision, iXShrink, iYShrink);
	}

	//-----------------------------------------------------------------
	// Sprite Inline General Methods
	//-----------------------------------------------------------------
	inline BOOL Sprite::TestCollision(Sprite* pTestSprite)
	{
		RECT& rcTest = pTestSprite->GetCollision();
		return m_rcCollision.left <= rcTest.right &&
			rcTest.left <= m_rcCollision.right &&
			m_rcCollision.top <= rcTest.bottom &&
			rcTest.top <= m_rcCollision.bottom;
	}

	inline BOOL Sprite::IsPointInside(int x, int y)
	{
		POINT ptPoint;
		ptPoint.x = x;
		ptPoint.y = y;
		return PtInRect(&m_rcPosition, ptPoint);
	}

	inline void Sprite::SetPosition(int x, int y)
	{
		OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
		CalcCollisionRect();
	}

	inline void Sprite::SetPosition(POINT ptPosition)
	{
		OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left,
			ptPosition.y - m_rcPosition.top);
		CalcCollisionRect();
	}

	inline void Sprite::OffsetPosition(int x, int y)
	{
		OffsetRect(&m_rcPosition, x, y);
		CalcCollisionRect();
	}

	inline void Sprite::SetVelocity(int x, int y)
	{
		m_ptVelocity.x = x;
		m_ptVelocity.y = y;
	}

	inline void Sprite::SetVelocity(POINT ptVelocity)
	{
		m_ptVelocity.x = ptVelocity.x;
		m_ptVelocity.y = ptVelocity.y;
	}



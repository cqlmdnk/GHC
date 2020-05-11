#include "FireBurst.h"
FireBurst::FireBurst() {
	int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 3;
	int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 3;
	CopyRect(&m_rcCollision, &m_rcPosition);



	InflateRect(&m_rcCollision, iXShrink, iYShrink);
}
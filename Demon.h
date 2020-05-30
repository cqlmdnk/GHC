#pragma once
#include "SimpleAI.h"
class Demon :
	public SimpleAI
{
public:
	Demon(HDC hDC, HINSTANCE _hInstance);
	~Demon();
	void changeState(STATE state);
	void act(int a);
	void die() ;
	bool lastFrame()
	{
		return m_iCurFrame == m_iNumFrames-1;
	};
};


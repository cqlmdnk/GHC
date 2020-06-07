//-----------------------------------------------------------------
// Game Engine Object
// C++ Source - GameEngine.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "GameEngine.h"

//-----------------------------------------------------------------
// Static Variable Initialization
//-----------------------------------------------------------------
GameEngine* GameEngine::m_pGameEngine = NULL;

//-----------------------------------------------------------------
// Windows Functions
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	MSG         msg;
	static int  iTickTrigger = 0;
	int         iTickCount;

	if (GameInitialize(hInstance))
	{
		// Initialize the game engine
		if (!GameEngine::GetEngine()->Initialize(iCmdShow))
			return FALSE;

		// Enter the main message loop
		while (TRUE)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// Process the message
				if (msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// Make sure the game engine isn't sleeping
				if (!GameEngine::GetEngine()->GetSleep())
				{
					// Check the tick count to see if a game cycle has elapsed
					iTickCount = GetTickCount();
					if (iTickCount > iTickTrigger)
					{
						iTickTrigger = iTickCount +
							GameEngine::GetEngine()->GetFrameDelay();
						HandleKeys();

						GameCycle();


					}
				}
			}
		}
		return (int)msg.wParam;
	}

	// End the game
	GameEnd();

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Route all Windows messages to the game engine
	return GameEngine::GetEngine()->HandleEvent(hWindow, msg, wParam, lParam);
}

BOOL GameEngine::CheckSpriteCollision(Sprite* pTestSprite)
{
	// See if the sprite has collided with any other sprites
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		// Make sure not to check for collision with itself
		if (pTestSprite == (*siSprite) || pTestSprite->deathMark || (*siSprite)->deathMark)
			continue;

		// Test the collision
		if (pTestSprite->TestCollision(*siSprite))
			// Collision detected
			
			return SpriteCollision((*siSprite), pTestSprite);
	}

	// No collision
	return FALSE;
}


//-----------------------------------------------------------------
// GameEngine Constructor(s)/Destructor
//-----------------------------------------------------------------
GameEngine::GameEngine(HINSTANCE hInstance, LPTSTR szWindowClass,
	LPTSTR szTitle, WORD wIcon, WORD wSmallIcon, int iWidth, int iHeight)
{
	// Set the member variables for the game engine
	m_pGameEngine = this;
	m_hInstance = hInstance;
	m_hWindow = NULL;
	if (lstrlen(szWindowClass) > 0)
		lstrcpy(m_szWindowClass, szWindowClass);
	if (lstrlen(szTitle) > 0)
		lstrcpy(m_szTitle, szTitle);
	m_wIcon = wIcon;
	m_wSmallIcon = wSmallIcon;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iFrameDelay = 50;   // 20 FPS default
	m_bSleep = TRUE;
	m_uiMIDIPlayerID = 0;
}

GameEngine::~GameEngine()
{
}

//-----------------------------------------------------------------
// Game Engine General Methods
//-----------------------------------------------------------------
BOOL GameEngine::Initialize(int iCmdShow)
{
	WNDCLASSEX    wndclass;

	// Create the window class for the main window
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_hInstance;
	wndclass.hIcon = LoadIcon(m_hInstance,
		MAKEINTRESOURCE(GetIcon()));
	wndclass.hIconSm = LoadIcon(m_hInstance,
		MAKEINTRESOURCE(GetSmallIcon()));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = m_szWindowClass;

	// Register the window class
	if (!RegisterClassEx(&wndclass))
		return FALSE;

	// Calculate the window size and position based upon the game size
	int iWindowWidth = m_iWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2,
		iWindowHeight = m_iHeight + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 +
		GetSystemMetrics(SM_CYCAPTION);
	if (wndclass.lpszMenuName != NULL)
		iWindowHeight += GetSystemMetrics(SM_CYMENU);
	int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth) / 2,
		iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight) / 2;

	// Create the window
	m_hWindow = CreateWindow(m_szWindowClass, m_szTitle, WS_POPUPWINDOW |
		WS_CAPTION | WS_MINIMIZEBOX, iXWindowPos, iYWindowPos, iWindowWidth,
		iWindowHeight, NULL, NULL, m_hInstance, NULL);
	if (!m_hWindow)
		return FALSE;

	// Show and update the window
	ShowWindow(m_hWindow, iCmdShow);
	UpdateWindow(m_hWindow);

	return TRUE;
}

LRESULT GameEngine::HandleEvent(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Route Windows messages to game engine member functions
	switch (msg)
	{
	case WM_CREATE:
		// Set the game window and start the game
		SetWindow(hWindow);
		GameStart(hWindow);
		return 0;

	case WM_ACTIVATE:
		// Activate/deactivate the game and update the Sleep status
		if (wParam != WA_INACTIVE)
		{
			GameActivate(hWindow);
			SetSleep(FALSE);
		}
		else
		{
			GameDeactivate(hWindow);
			SetSleep(TRUE);
		}
		return 0;

	case WM_PAINT:
		HDC         hDC;
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWindow, &ps);

		// Paint the game
		GamePaint(hDC);

		EndPaint(hWindow, &ps);
		return 0;

	case WM_LBUTTONDOWN:
		// Handle left mouse button press
		MouseButtonDown(LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;

	case WM_LBUTTONUP:
		// Handle left mouse button release
		MouseButtonUp(LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;

	case WM_RBUTTONDOWN:
		// Handle right mouse button press
		MouseButtonDown(LOWORD(lParam), HIWORD(lParam), FALSE);
		return 0;

	case WM_RBUTTONUP:
		// Handle right mouse button release
		MouseButtonUp(LOWORD(lParam), HIWORD(lParam), FALSE);
		return 0;

	case WM_MOUSEMOVE:
		// Handle mouse movement
		MouseMove(LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_DESTROY:
		// End the game and exit the application
		GameEnd();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWindow, msg, wParam, lParam);
}

void GameEngine::ErrorQuit(LPTSTR szErrorMsg)
{
	MessageBox(GetWindow(), szErrorMsg, TEXT("Critical Error"), MB_OK | MB_ICONERROR);
	PostQuitMessage(0);
}



void GameEngine::AddSprite(Sprite* pSprite)
{
	// Add a sprite to the sprite vector
	if (pSprite != NULL)
	{
		// See if there are sprites already in the sprite vector
		if (m_vSprites.size() > 0)
		{
			// Find a spot in the sprite vector to insert the sprite by its z-order
			vector<Sprite*>::iterator siSprite;
			for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
				if (pSprite->GetZOrder() < (*siSprite)->GetZOrder())
				{
					// Insert the sprite into the sprite vector
					m_vSprites.insert(siSprite, pSprite);
					return;
				}
		}

		// The sprite's z-order is highest, so add it to the end of the vector
		m_vSprites.push_back(pSprite);
	}
}

void GameEngine::DrawSprites(HDC hDC)
{
	// Draw the sprites in the sprite vector
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
		if (!(*siSprite)->IsStateHalt())
			(*siSprite)->Draw(hDC);
}

void GameEngine::UpdateSprites(int** map, int x, int vx)
{
	// Update the sprites in the sprite vector
	RECT          rcOldSpritePos;
	SPRITEACTION  saSpriteAction;
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		(*siSprite)->SetVelocity((*siSprite)->GetVelocity().x, min(50, (*siSprite)->GetVelocity().y));  // artificial fall speed equilibrium between gravity and air resistance 

		if ((*siSprite)->IsStateHalt()) {
			// change stateHalt if it is supposed to be in screen
			if ((*siSprite)->GetAbsX() > x  && (*siSprite)->GetAbsX() < x + 1920/*Ekran genişiliği makrosu*/) {
				(*siSprite)->SetStateHalt(FALSE);
				
				(*siSprite)->SetPosition((*siSprite)->GetPosition().left-((*siSprite)->GetPosition().left %PLATFORM_S), (*siSprite)->GetPosition().top);
				
				
			}
			else {
				continue;
			}
		}
		



		// Save the old sprite position in case we need to restore it
		rcOldSpritePos = (*siSprite)->GetPosition();
		if (!instanceof<PlayerCharacter>((*siSprite))) { // checking if sprite is a AI or not // SimpleAI Class >> BaseAI Class
															//  can't check for character, since all AIs are characters

			((*siSprite))->SetPosition(((*siSprite))->GetPosition().left - vx, ((*siSprite))->GetPosition().top);

		}
		//sprite lara x'in modu eklenmeli
		if (!instanceof<Spell>((*siSprite))) {
		}

		// Update the sprite
		// Update the sprite
		saSpriteAction = (*siSprite)->Update(map, x);
		if (instanceof<FireBurst>(*siSprite) || instanceof<Tile>(*siSprite)) {
			(*siSprite)->SetVelocity((*siSprite)->GetVelocity().x, (*siSprite)->GetVelocity().y - 10); // degravitation 
		}
		// Handle the SA_KILL sprite action
		if (saSpriteAction & SA_KILL)
		{
			//m_vSprites.erase(std::remove(m_vSprites.begin(), m_vSprites.end(), (*siSprite)), m_vSprites.end());
			(*siSprite)->SetHidden(TRUE);
			//siSprite--;


			continue;
		}


		// See if the sprite collided with any others
		if (CheckSpriteCollision(*siSprite)) {
			if (instanceof<Spell>((*siSprite)) || instanceof<FireBurst>((*siSprite)) || instanceof<Tile>((*siSprite)) || instanceof<SimpleAI>((*siSprite))) {

			}
			else if (instanceof<PlayerCharacter>((*siSprite))) {
				
			}
			else {
				(*siSprite)->SetPosition(rcOldSpritePos);

			}






		}
		// Restore the old sprite position




	}
	for (auto sprite : m_vSprites)
	{

		if ((sprite->deathMark && sprite->IsAnimDef()) || sprite->IsHidden()) {
			
			delete sprite;
			m_vSprites.erase(std::remove(m_vSprites.begin(), m_vSprites.end(), sprite), m_vSprites.end());
			
		}
		else {
			if (instanceof<FireBurst>(sprite) && sprite->GetVelocity().x == 0) {
				delete sprite;
				m_vSprites.erase(std::remove(m_vSprites.begin(), m_vSprites.end(), sprite), m_vSprites.end());

				
			}
		}


	}



}


void GameEngine::CleanupSprites()
{
	// Delete and remove the sprites in the sprite vector
	vector<Sprite*>::iterator siSprite;
	for (siSprite = m_vSprites.begin(); siSprite != m_vSprites.end(); siSprite++)
	{
		delete (*siSprite);
		m_vSprites.erase(siSprite);
		siSprite--;
	}
}

Sprite* GameEngine::IsPointInSprite(int x, int y)
{
	// See if the point is in a sprite in the sprite vector
	vector<Sprite*>::reverse_iterator siSprite;
	for (siSprite = m_vSprites.rbegin(); siSprite != m_vSprites.rend(); siSprite++)
		if (!(*siSprite)->IsHidden() && (*siSprite)->IsPointInside(x, y))
			return (*siSprite);

	// The point is not in a sprite
	return NULL;
}




void GameEngine::PlayMIDISong(LPTSTR szMIDIFileName, BOOL bRestart)
{
	//See if the MIDI player needs to be opened
	if (m_uiMIDIPlayerID == 0)
	{
		//Open the MIDI player by specifying the device and filename
		MCI_OPEN_PARMS mciOpenParms;
		mciOpenParms.lpstrDeviceType = "sequencer";
		mciOpenParms.lpstrElementName = szMIDIFileName;
		if (mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
			(DWORD_PTR)&mciOpenParms) == 0)

			//Get the ID for the MIDI player
			m_uiMIDIPlayerID = mciOpenParms.wDeviceID;
		else

			//There was a problem, so just return
			return;
	}

	//Restart the MIDI song, if necessary
	if (bRestart)
	{
		MCI_SEEK_PARMS mciSeekParms;
		if (mciSendCommand(m_uiMIDIPlayerID, MCI_SEEK, MCI_SEEK_TO_START,
			(DWORD_PTR)&mciSeekParms) != 0)

			//There was a problem, so close the MIDI player
			CloseMIDIPlayer();
	}

	//Play the MIDI song
	MCI_PLAY_PARMS mciPlayParms;
	if (mciSendCommand(m_uiMIDIPlayerID, MCI_PLAY, 0,
		(DWORD_PTR)&mciPlayParms) != 0)

		//There was a problem so close the MIDI player
		CloseMIDIPlayer();
}

void GameEngine::PauseMIDISong()
{
	//Pause the currently playing song, if possible
	if (m_uiMIDIPlayerID != 0)
		mciSendCommand(m_uiMIDIPlayerID, MCI_PAUSE, 0, NULL);
}

void GameEngine::CloseMIDIPlayer()
{
	//Close the MIDI player, if possible
	if (m_uiMIDIPlayerID != 0)
	{
		mciSendCommand(m_uiMIDIPlayerID, MCI_CLOSE, 0, NULL);
		m_uiMIDIPlayerID = 0;
	}
}
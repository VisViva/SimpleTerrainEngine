#include "..\\Includes\\CMoveEngine.h"
#include "..\\Includes\\CCamera.h"

#define IDI_ICON_MOVE	102
#define IDR_MAIN_MENU	103
#define ID_FILE_DIRECTXSETTINGS	104
#define ID_FILE_EXIT 105
#define ID_HELP_ABOUT 106

CMoveEngine::CMoveEngine()
{
	m_hWnd = NULL;
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_hIcon = NULL;
	m_hMenu = NULL;
	m_LastFrameRate = 0;
	m_bLostDevice = false;
	m_d3dColor = 0xFFC7CFD6;
	m_bIsMouseIntercepted = true;
	m_enInputMode = IM_GAME;
	if ((GetKeyState(VK_TAB) & 0x0001) = = 0)
	{
		::keybd_event(VK_TAB, 0x45, KEYEVENTF_EXTENDEDKEY, 0);
		::keybd_event(VK_TAB, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
	if ((GetKeyState(VK_OEM_3) & 0x0001) = = 0)
	{
		::keybd_event(VK_OEM_3, 0x45, KEYEVENTF_EXTENDEDKEY, 0);
		::keybd_event(VK_OEM_3, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
	}
}

CMoveEngine::~CMoveEngine()
{
	StopEngine();
}

bool CMoveEngine::InitInstance(HANDLE hInstance, LPCTSTR lpCmdLine, int iCmdShow)
{
	if (!CreateDisplay()) {StopEngine(); return false;}
	if (!BuildObjects()) {StopEngine(); return false;}
	SetupGameState();
	SetupRenderStates();
	return true;
}

bool CMoveEngine::CreateDisplay()
{
	D3DDISPLAYMODE MatchMode;
	CD3DSettingsDlg SettingsDlg;
	CMyD3DInit Initialize;
	LPTSTR WindowTitle = _T("Move Engine");
	USHORT Width = 640;
	USHORT Height = 480;
	RECT rc;
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pD3D)
	{
		MessageBox(m_hWnd, _T("No compatible Direct3D object could be created."), _T("Fatal Error!"), MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		return false;
	}
	if (FAILED(Initialize.Enumerate(m_pD3D)))
	{
		MessageBox(m_hWnd, _T("Device enumeration failed. The application will now exit."), _T("Fatal Error!"), MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		return false;
	}
	MatchMode.Width = 1920;
	MatchMode.Height = 1080;
	MatchMode.Format = D3DFMT_UNKNOWN;
	MatchMode.RefreshRate = 0;
	Initialize.FindBestFullscreenMode(m_D3DSettings, &MatchMode);
	Initialize.FindBestWindowedMode(m_D3DSettings);
	int RetCode = SettingsDlg.ShowDialog(&Initialize, &m_D3DSettings);
	if (RetCode ! = IDOK) return false;
	m_D3DSettings = SettingsDlg.GetD3DSettings();
	if (FAILED(Initialize.CreateDisplay(m_D3DSettings, 0, NULL, StaticWndProc, WindowTitle, Width, Height, this)))
	{
		MessageBox(m_hWnd, _T("Device creation failed. The application will now exit."), _T("Fatal Error!"), MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		return false;
	}
	m_pD3DDevice = Initialize.GetDirect3DDevice();
	m_hWnd = Initialize.GetHWND();
	m_hIcon = LoadIcon((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MOVE));
	m_hMenu = LoadMenu((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAIN_MENU));
	SetClassLong(m_hWnd, GCL_HICON, (long)m_hIcon);
	if (m_D3DSettings.Windowed)
	{
		SetMenu(m_hWnd, m_hMenu);
	}
	::GetClientRect(m_hWnd, &rc);
	m_ulViewX = rc.left;
	m_ulViewY = rc.top;
	m_ulViewWidth = rc.right - rc.left;
	m_ulViewHeight = rc.bottom - rc.top;
	ShowWindow(m_hWnd, SW_SHOW);
	return true;
}

void CMoveEngine::ChangeDevice()
{
	CMyD3DInit Initialize;
	CD3DSettingsDlg SettingsDlg;
	if (FAILED(Initialize.Enumerate(m_pD3D)))
	{
		MessageBox(m_hWnd, _T("Device enumeration failed. The application will now exit."), _T("Fatal Error!"), MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		PostQuitMessage(0);
		return;
	}
	int RetCode = SettingsDlg.ShowDialog(&Initialize, &m_D3DSettings);
	if (RetCode ! = IDOK) return;
	m_D3DSettings = SettingsDlg.GetD3DSettings();
	if (m_pD3DDevice) m_pD3DDevice->Release();
	m_pD3DDevice = NULL;
	if (FAILED(Initialize.CreateDisplay(m_D3DSettings, 0, m_hWnd)))
	{
		MessageBox(m_hWnd, _T("Device creation failed. The application will now exit."), _T("Fatal Error!"), MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		PostQuitMessage(0);
		return;
	}
	m_pD3DDevice = Initialize.GetDirect3DDevice();
	if (m_D3DSettings.Windowed)
	{
		SetMenu(m_hWnd, m_hMenu);
	}
	BuildObjects();
	SetupRenderStates();
}

int CMoveEngine::StartEngine()
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message = = WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage (&msg);
		}
		else
		{
			FrameAdvance();
		}
	}
	return 0;
}

bool CMoveEngine::StopEngine()
{
	ReleaseObjects();
	if (m_pD3DDevice) m_pD3DDevice->Release();
	if (m_pD3D) m_pD3D->Release();
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	if (m_hMenu) DestroyMenu(m_hMenu);
	m_hMenu = NULL;
	SetMenu(m_hWnd, NULL);
	if (m_hWnd) DestroyWindow(m_hWnd);
	m_hWnd = NULL;
	return true;
}

LRESULT CALLBACK CMoveEngine::StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message = = WM_CREATE) SetWindowLong(hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);
	CMoveEngine *Destination = (CMoveEngine*)GetWindowLong(hWnd, GWL_USERDATA);
	if (Destination) return Destination->DisplayWndProc(hWnd, Message, wParam, lParam);
	return DefWindowProc(hWnd, Message, wParam, lParam);
}

LRESULT CMoveEngine::DisplayWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	CMyD3DInit Initialize;
	switch (Message)
	{
		case WM_CREATE:
		break;
		case WM_CLOSE:
		PostQuitMessage(0);
		break;
		case WM_DESTROY:
		PostQuitMessage(0);
		break;
		case WM_SIZE:
		if (wParam = = SIZE_MINIMIZED)
		{
			m_bActive = false;
		}
		else
		{
			m_bActive = true;
			m_ulViewWidth = LOWORD(lParam);
			m_ulViewHeight = HIWORD(lParam);
			if (m_pD3DDevice)
			{
				m_Console.Release();
				if (m_pCamera) m_pCamera->SetViewport(m_ulViewX, m_ulViewY, m_ulViewWidth, m_ulViewHeight, 1.01f, 5000.0f);
				Initialize.ResetDisplay(m_pD3DDevice, m_D3DSettings);
				m_Console.SetUpConsole(m_pD3DDevice, m_ulViewWidth, m_ulViewHeight);
				SetupRenderStates();
			}
		}
		break;
		case WM_SYSKEYDOWN:
		switch (wParam)
		{
			case VK_RETURN:
			m_D3DSettings.Windowed = !m_D3DSettings.Windowed;
			Initialize.ResetDisplay(m_pD3DDevice, m_D3DSettings, m_hWnd);
			m_Console.SetUpConsole(m_pD3DDevice, m_ulViewWidth, m_ulViewHeight);
			SetupRenderStates();
			if (m_D3DSettings.Windowed)
			{
				SetMenu(m_hWnd, m_hMenu);
			}
		}
		break;
		case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
		break;
		case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case ID_FILE_DIRECTXSETTINGS:
			ChangeDevice();
			break;
			case ID_FILE_EXIT:
			SendMessage(m_hWnd, WM_CLOSE, 0, 0);
			return 0;
			case ID_HELP_ABOUT:
			MessageBox(m_hWnd, _T("Move Engine - Alpha Build 0.1\rby Abdulali Qasimov"), _T("About"), MB_OK | MB_ICONINFORMATION);
			break;
		}
		default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return 0;
}

void CMoveEngine::SetupRenderStates()
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	m_ulViewWidth = rc.right - rc.left;
	m_ulViewHeight = rc.bottom - rc.top;
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_Console.m_ConsoleStream << "All necessary render states are set." << "\n";
	m_pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	if (!m_pCamera) return;
	m_pCamera->SetViewport(m_ulViewX, m_ulViewY, m_ulViewWidth, m_ulViewHeight, 1.01f, 5000.0f);
	m_pCamera->UpdateRenderView(m_pD3DDevice);
	m_pCamera->UpdateRenderProj(m_pD3DDevice);
}

void CMoveEngine::SetupGameState()
{
	D3DXMatrixIdentity(&m_mtxIdentity);
	m_bActive = true;
	if (m_bIsMouseIntercepted)
	{
		SetCapture(m_hWnd);
		GetCursorPos(&m_OldCursorPos);
	}
	m_Player.SetCameraMode(CCamera::MODE_FPS);
	m_pCamera = m_Player.GetCamera();
	m_Console.m_ConsoleStream << "Camera mode is set to first person." << "\n";
	m_Player.SetFriction(250.0f);
	m_Player.SetGravity(D3DXVECTOR3(0, -400.0f, 0));
	m_Player.SetMaxVelocityXZ(125.0f);
	m_Player.SetMaxVelocityY(400.0f);
	m_Player.SetCamOffset(D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	m_Player.SetCamLag(0.0f);
	VOLUME_INFO Volume;
	Volume.Min = D3DXVECTOR3(-3, -10, -3);
	Volume.Max = D3DXVECTOR3(3, 10, 3);
	m_Player.SetVolumeInfo(Volume);
	m_Console.m_ConsoleStream << "Player collision volume is set." << "\n";
	m_pCamera->SetFOV(80.0f);
	m_pCamera->SetViewport(m_ulViewX, m_ulViewY, m_ulViewWidth, m_ulViewHeight, 1.01f, 5000.0f);
	m_pCamera->SetVolumeInfo(Volume);
	m_Player.AddPlayerCallback(CTerrain::UpdatePlayer, (LPVOID)&m_Terrain);
	m_Player.AddCameraCallback(CTerrain::UpdateCamera, (LPVOID)&m_Terrain);
	m_Player.SetPosition(D3DXVECTOR3(430.0f, 400.0f, 330.0f));
	m_Player.Rotate(25, 45, 0);
	m_Console.m_ConsoleStream << "All necessary game states are set." << "\n";
}

bool CMoveEngine::BuildObjects()
{
	VERTEXPROCESSING_TYPE vp = m_D3DSettings.GetSettings()->m_VertexProcessingType;
	bool HardwareTnL = true;
	if (vp ! = HARDWARE_VP && vp ! = PURE_HARDWARE_VP)
	{
		HardwareTnL = false;
		m_Console.m_ConsoleStream << "Hardware transform and lighting is off." << "\n";
	}
	else
	{
		m_Console.m_ConsoleStream << "Hardware transform and lighting is on." << "\n";
	}
	ReleaseObjects();
	m_Console.SetUpConsole(m_pD3DDevice, m_ulViewWidth, m_ulViewHeight);
	m_Console.m_ConsoleStream << "Console have been set up." << "\n";
	m_Terrain.SetD3DDevice(m_pD3DDevice, HardwareTnL);
	if (!m_Terrain.LoadHeightMap(_T("Data\\HeightMap.raw"), 257, 257)) return false;
	m_Console.m_ConsoleStream << "Terrain heightmap loaded. [ " << "Data\\HeightMap.raw" << " ]\n";
	CVertex *pVertex = NULL;
	srand(timeGetTime());
	if (m_PlayerMesh.AddVertex(8) < 0) return false;
	pVertex = &m_PlayerMesh.m_pVertex[0];
	*pVertex++ = CVertex(-3, 0, -3, 0xFF000000);
	*pVertex++ = CVertex(-3, 0, 3, 0xFF000000);
	*pVertex++ = CVertex(3, 0, 3, 0xFF000000);
	*pVertex++ = CVertex(3, 0, -3, 0xFF000000);
	*pVertex++ = CVertex(-3, 20, -3, 0xFF000000);
	*pVertex++ = CVertex(-3, 20, 3, 0xFF000000);
	*pVertex++ = CVertex(3, 20, 3, 0xFF000000);
	*pVertex++ = CVertex(3, 20, -3, 0xFF000000);
	if (m_PlayerMesh.AddIndex(16) < 0) return false;
	m_PlayerMesh.m_pIndex[0] = 5;
	m_PlayerMesh.m_pIndex[1] = 6;
	m_PlayerMesh.m_pIndex[2] = 4;
	m_PlayerMesh.m_pIndex[3] = 7;
	m_PlayerMesh.m_pIndex[4] = 0;
	m_PlayerMesh.m_pIndex[5] = 3;
	m_PlayerMesh.m_pIndex[6] = 1;
	m_PlayerMesh.m_pIndex[7] = 2;
	m_PlayerMesh.m_pIndex[8] = 3;
	m_PlayerMesh.m_pIndex[9] = 7;
	m_PlayerMesh.m_pIndex[10] = 2;
	m_PlayerMesh.m_pIndex[11] = 6;
	m_PlayerMesh.m_pIndex[12] = 1;
	m_PlayerMesh.m_pIndex[13] = 5;
	m_PlayerMesh.m_pIndex[14] = 0;
	m_PlayerMesh.m_pIndex[15] = 4;
	if (FAILED(m_PlayerMesh.BuildBuffers(m_pD3DDevice, HardwareTnL, true))) return false;
	m_Console.m_ConsoleStream << "Player mesh is built." << "\n";
	m_Object.m_pMesh = &m_PlayerMesh;
	m_Player.Set3rdPersonObject(&m_Object);
	m_Console.m_ConsoleStream << "Player object is properly linked." << "\n";
	return true;
}

void CMoveEngine::AnimateObjects()
{
}

void CMoveEngine::FrameAdvance()
{
	static TCHAR FrameRate[50];
	static TCHAR TitleBuffer[255];
	m_Timer.Tick();
	if (!m_bActive) return;
	if (m_LastFrameRate ! = m_Timer.GetFrameRate())
	{
		m_LastFrameRate = m_Timer.GetFrameRate(FrameRate);
		_stprintf(TitleBuffer, _T("Move Engine. %s"), FrameRate);
		SetWindowText(m_hWnd, TitleBuffer);
	}
	if (m_bLostDevice)
	{
		HRESULT hRet = m_pD3DDevice->TestCooperativeLevel();
		if (hRet = = D3DERR_DEVICENOTRESET)
		{
			CMyD3DInit Initialize;
			Initialize.ResetDisplay(m_pD3DDevice, m_D3DSettings, m_hWnd);
			m_Console.SetUpConsole(m_pD3DDevice, m_ulViewWidth, m_ulViewHeight);
			SetupRenderStates();
			m_bLostDevice = false;
		}
		else
		{
			return;
		}
	}
	ProcessInput();
	AnimateObjects();
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_d3dColor, 1.0f, 0);
	m_pD3DDevice->BeginScene();
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_mtxIdentity);
	m_Terrain.Render(m_pCamera);
	m_Player.Render(m_pD3DDevice);
	m_Console.RenderConsole();
	m_pD3DDevice->EndScene();
	if (FAILED(m_pD3DDevice->Present(NULL, NULL, NULL, NULL)))
	{
		m_bLostDevice = true;
	}
}

void CMoveEngine::ProcessInput()
{
	static UCHAR pKeyBuffer[256];
	ULONG Direction = 0;
	POINT CursorPos;
	float X = 0.0f, Y = 0.0f;
	if (!GetKeyboardState(pKeyBuffer)) return;
	switch (m_enInputMode)
	{
		case IM_GAME:
		if (pKeyBuffer[0x31] & 0xF0)
		{
			m_Player.SetFriction(250.0f);
			m_Player.SetGravity(D3DXVECTOR3(0, -400.0f, 0));
			m_Player.SetMaxVelocityXZ(125.0f);
			m_Player.SetMaxVelocityY(400.0f);
			m_Player.SetCamOffset(D3DXVECTOR3(0.0f, 10.0f, 0.0f));
			m_Player.SetCamLag(0.0f);
			m_Player.SetCameraMode(CCamera::MODE_FPS);
			m_pCamera = m_Player.GetCamera();
		}
		if (pKeyBuffer[0x32] & 0xF0)
		{
			m_Player.SetFriction(125.0f);
			m_Player.SetGravity(D3DXVECTOR3(0, 0, 0));
			m_Player.SetMaxVelocityXZ(400.0f);
			m_Player.SetMaxVelocityY(400.0f);
			m_Player.SetCamOffset(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_Player.SetCamLag(0.0f);
			m_Player.SetCameraMode(CCamera::MODE_SPACECRAFT);
			m_pCamera = m_Player.GetCamera();
		}
		if (pKeyBuffer[0x33] & 0xF0)
		{
			m_Player.SetFriction(250.0f);
			m_Player.SetGravity(D3DXVECTOR3(0, -400.0f, 0));
			m_Player.SetMaxVelocityXZ(125.0f);
			m_Player.SetMaxVelocityY(400.0f);
			m_Player.SetCamOffset(D3DXVECTOR3(0.0f, 40.0f, -60.0f));
			m_Player.SetCamLag(0.25f);
			m_Player.SetCameraMode(CCamera::MODE_THIRDPERSON);
			m_pCamera = m_Player.GetCamera();
		}
		if (pKeyBuffer[0x57] & 0xF0) Direction | = CPlayer::DIR_FORWARD;
		if (pKeyBuffer[0x53] & 0xF0) Direction | = CPlayer::DIR_BACKWARD;
		if (pKeyBuffer[0x41] & 0xF0) Direction | = CPlayer::DIR_LEFT;
		if (pKeyBuffer[0x44] & 0xF0) Direction | = CPlayer::DIR_RIGHT;
		if (pKeyBuffer[VK_PRIOR] & 0xF0) Direction | = CPlayer::DIR_UP;
		if (pKeyBuffer[VK_NEXT] & 0xF0) Direction | = CPlayer::DIR_DOWN;
		if (GetKeyState(VK_RIGHT) & 0xFF00) m_d3dColor + = 10000;
		if (GetKeyState(VK_LEFT) & 0xFF00) m_d3dColor - = 10000;
		break;
		case IM_CONSOLE:
		if (GetAsyncKeyState(0x60) & 1) m_Console.m_CommandLineStream << "0";
		if (GetAsyncKeyState(0x61) & 1) m_Console.m_CommandLineStream << "1";
		if (GetAsyncKeyState(0x62) & 1) m_Console.m_CommandLineStream << "2";
		if (GetAsyncKeyState(0x63) & 1) m_Console.m_CommandLineStream << "3";
		if (GetAsyncKeyState(0x64) & 1) m_Console.m_CommandLineStream << "4";
		if (GetAsyncKeyState(0x65) & 1) m_Console.m_CommandLineStream << "5";
		if (GetAsyncKeyState(0x66) & 1) m_Console.m_CommandLineStream << "6";
		if (GetAsyncKeyState(0x67) & 1) m_Console.m_CommandLineStream << "7";
		if (GetAsyncKeyState(0x68) & 1) m_Console.m_CommandLineStream << "8";
		if (GetAsyncKeyState(0x69) & 1) m_Console.m_CommandLineStream << "9";
		if (GetAsyncKeyState(0x41) & 1) m_Console.m_CommandLineStream << "a";
		if (GetAsyncKeyState(0x42) & 1) m_Console.m_CommandLineStream << "b";
		if (GetAsyncKeyState(0x43) & 1) m_Console.m_CommandLineStream << "c";
		if (GetAsyncKeyState(0x44) & 1) m_Console.m_CommandLineStream << "d";
		if (GetAsyncKeyState(0x45) & 1) m_Console.m_CommandLineStream << "e";
		if (GetAsyncKeyState(0x46) & 1) m_Console.m_CommandLineStream << "f";
		if (GetAsyncKeyState(0x47) & 1) m_Console.m_CommandLineStream << "g";
		if (GetAsyncKeyState(0x48) & 1) m_Console.m_CommandLineStream << "h";
		if (GetAsyncKeyState(0x49) & 1) m_Console.m_CommandLineStream << "i";
		if (GetAsyncKeyState(0x4A) & 1) m_Console.m_CommandLineStream << "j";
		if (GetAsyncKeyState(0x4B) & 1) m_Console.m_CommandLineStream << "k";
		if (GetAsyncKeyState(0x4C) & 1) m_Console.m_CommandLineStream << "l";
		if (GetAsyncKeyState(0x4D) & 1) m_Console.m_CommandLineStream << "m";
		if (GetAsyncKeyState(0x4E) & 1) m_Console.m_CommandLineStream << "n";
		if (GetAsyncKeyState(0x4F) & 1) m_Console.m_CommandLineStream << "o";
		if (GetAsyncKeyState(0x50) & 1) m_Console.m_CommandLineStream << "p";
		if (GetAsyncKeyState(0x51) & 1) m_Console.m_CommandLineStream << "q";
		if (GetAsyncKeyState(0x52) & 1) m_Console.m_CommandLineStream << "r";
		if (GetAsyncKeyState(0x53) & 1) m_Console.m_CommandLineStream << "s";
		if (GetAsyncKeyState(0x54) & 1) m_Console.m_CommandLineStream << "t";
		if (GetAsyncKeyState(0x55) & 1) m_Console.m_CommandLineStream << "u";
		if (GetAsyncKeyState(0x56) & 1) m_Console.m_CommandLineStream << "v";
		if (GetAsyncKeyState(0x57) & 1) m_Console.m_CommandLineStream << "w";
		if (GetAsyncKeyState(0x58) & 1) m_Console.m_CommandLineStream << "x";
		if (GetAsyncKeyState(0x59) & 1) m_Console.m_CommandLineStream << "y";
		if (GetAsyncKeyState(0x5A) & 1) m_Console.m_CommandLineStream << "z";
		if (GetAsyncKeyState(0xBF) & 1) m_Console.m_CommandLineStream << "/";
		if (GetAsyncKeyState(0xDB) & 1) m_Console.m_CommandLineStream << "[";
		if (GetAsyncKeyState(0xDD) & 1) m_Console.m_CommandLineStream << "]";
		if (GetAsyncKeyState(0xDE) & 1) m_Console.m_CommandLineStream << "'";
		if (GetAsyncKeyState(0xDC) & 1) m_Console.m_CommandLineStream << "\\";
		if (GetAsyncKeyState(0xBE) & 1) m_Console.m_CommandLineStream << ".";
		if (GetAsyncKeyState(0xBC) & 1) m_Console.m_CommandLineStream << ",";
		if (GetAsyncKeyState(0xBA) & 1) m_Console.m_CommandLineStream << ";";
		if (GetAsyncKeyState(0xBD) & 1) m_Console.m_CommandLineStream << "-";
		if (GetAsyncKeyState(0xBB) & 1) m_Console.m_CommandLineStream << "+";
		if (GetAsyncKeyState(0x20) & 1) m_Console.m_CommandLineStream << " ";
		if (GetAsyncKeyState(VK_UP) & 1) m_Console.ScrollUp();
		if (GetAsyncKeyState(VK_DOWN) & 1) m_Console.ScrollDown();
		if (GetAsyncKeyState(VK_BACK) & 1) m_Console.Backspace();
		if (GetAsyncKeyState(VK_RIGHT) & 1) m_Console.RetreiveLastCommand();
		if (GetAsyncKeyState(VK_RETURN) & 1) ReadConsoleCommand();
		break;
	}
	if ((GetKeyState(VK_TAB) & 0x0001) ! = 0)
	{
		if (m_bIsMouseIntercepted)
		{
			SetCapture(m_hWnd);
			GetCursorPos(&m_OldCursorPos);
			m_bIsMouseIntercepted = false;
		}
	}
	else
	{
		if (!m_bIsMouseIntercepted)
		{
			ReleaseCapture();
			m_bIsMouseIntercepted = true;
		}
	}
	if ((GetKeyState(VK_OEM_3) & 0x0001) ! = 0)
	{
		m_Console.HideConsole();
		m_enInputMode = IM_GAME;
	}
	else
	{
		m_Console.ShowConsole();
		m_enInputMode = IM_CONSOLE;
	}
	if (GetCapture() = = m_hWnd)
	{
		SetCursor(NULL);
		GetCursorPos(&CursorPos);
		X = (float)(CursorPos.x - m_OldCursorPos.x) / 3.0f;
		Y = (float)(CursorPos.y - m_OldCursorPos.y) / 3.0f;
		SetCursorPos(m_OldCursorPos.x, m_OldCursorPos.y);
	}
	if (Direction > 0 || X ! = 0.0f || Y ! = 0.0f)
	{
		if (X || Y)
		{
			if (pKeyBuffer[VK_RBUTTON] & 0xF0) m_Player.Rotate(Y, 0.0f, -X);
			else m_Player.Rotate(Y, X, 0.0f);
		}
		if (Direction)
		{
			m_Player.Move(Direction, 500.0f * m_Timer.GetTimeElapsed(), true);
		}
	}
	m_Player.Update(m_Timer.GetTimeElapsed());
	m_pCamera->UpdateRenderView(m_pD3DDevice);
}

void CMoveEngine::ReleaseObjects()
{
	if (m_pD3DDevice)
	{
		m_pD3DDevice->SetStreamSource(0, NULL, 0, 0);
		m_pD3DDevice->SetIndices(NULL);
	}
	m_Terrain.Release();
	m_Console.Release();
}

void CMoveEngine::ReadConsoleCommand()
{
	std::string sCommandToken("");
	std::string sCommandLine(m_Console.m_CommandLineStream.str());
	std::stringstream ssStreamToBeTokenized(sCommandLine);
	ssStreamToBeTokenized >> sCommandToken;
	ssStreamToBeTokenized >> sCommandToken;
	if (sCommandLine = = "> help")
	{
		m_Console.m_ConsoleStream << "----------------------------------------------------------------------------------------------------------------------\n";
		m_Console.m_ConsoleStream << "help - Display command info.\n";
		m_Console.m_ConsoleStream << "clear - Clear console.\n";
		m_Console.m_ConsoleStream << "set renderer point - Render vertices.\n";
		m_Console.m_ConsoleStream << "set rendererwireframe - Render wireframe.\n";
		m_Console.m_ConsoleStream << "set renderer solid - Render filled.\n";
		m_Console.m_ConsoleStream << "set camera fov # - Set field of view.\n";
		m_Console.m_ConsoleStream << "set console color # - Set the color of text.\n";
		m_Console.m_ConsoleStream << "set device - Set device settings.\n";
		m_Console.m_ConsoleStream << "about - Display about info.\n";
		m_Console.m_ConsoleStream << "exit - Exit to windows.\n";
		m_Console.m_ConsoleStream << "----------------------------------------------------------------------------------------------------------------------\n";
	}
	if (sCommandLine = = "> exit")
	{
		PostQuitMessage(0);
	}
	if (sCommandLine = = "> clear")
	{
		m_Console.ClearConsole();
	}
	if (sCommandLine = = "> about")
	{
		m_Console.m_ConsoleStream << "----------------------------------------------------------------------------------------------------------------------\n";
		m_Console.m_ConsoleStream << "   Move Engine Alpha Build 1.0\n";
		m_Console.m_ConsoleStream << "   by Abdulali Qasimov.\n";
		m_Console.m_ConsoleStream << "----------------------------------------------------------------------------------------------------------------------\n";
	}
	if (sCommandToken = = "set")
	{
		ssStreamToBeTokenized >> sCommandToken;
		if (sCommandToken = = "renderer")
		{
			ssStreamToBeTokenized >> sCommandToken;
			if (sCommandToken = = "point")
			{
				m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
				m_Console.m_ConsoleStream << "Render state is set to point rendering.\n";
			}
			if (sCommandToken = = "wireframe")
			{
				m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
				m_Console.m_ConsoleStream << "Render state is set to wireframe rendering.\n";
			}
			if (sCommandToken = = "solid")
			{
				m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
				m_Console.m_ConsoleStream << "Render state is set to solid rendering.\n";
			}
		}
		if (sCommandToken = = "camera")
		{
			ssStreamToBeTokenized >> sCommandToken;
			if (sCommandToken = = "fov")
			{
				float fFov;
				if ((ssStreamToBeTokenized >> std::dec >> fFov).fail())
				{
					m_Console.m_ConsoleStream << "Value type is not acceptable.\n";
				}
				else
				{
					if ((fFov > 150.0f) || (fFov < 5.0f))
					{
						m_Console.m_ConsoleStream << "Value should be within range of 0 and 180 degrees.\n";
					}
					else
					{
						m_pCamera->SetFOV(fFov);
						m_Console.m_ConsoleStream << "Field of view clipping is set to " << std::dec << fFov << ". \n";
					}
				}
			}
		}
		if (sCommandToken = = "console")
		{
			ssStreamToBeTokenized >> sCommandToken;
			if (sCommandToken = = "color")
			{
				D3DCOLOR d3dColor;
				if ((ssStreamToBeTokenized >> std::hex >> d3dColor).fail())
				{
					m_Console.m_ConsoleStream << "Value should be hexademical. Like this: FFFFFFFF.\n";
				}
				else
				{
					if (d3dColor < = 0x00FFFFFF)
					{
						m_Console.m_ConsoleStream << "All color values less than or equal to FFFFFF are completely transparent.\n";
						m_Console.m_ConsoleStream << "The value was forcefully incremented by engine in order to let the text to remain visible.\n";
						d3dColor + = 0xFF000000;
					}
					m_Console.SetConsoleColor(d3dColor);
					m_Console.m_ConsoleStream << "Console is colored to " << std::hex << d3dColor << std::dec << ". \n";
				}
			}
		}
		if (sCommandToken = = "device")
		{
			if (!m_D3DSettings.Windowed)
			{
				m_D3DSettings.Windowed = true;
				CMyD3DInit Initialize;
				Initialize.ResetDisplay(m_pD3DDevice, m_D3DSettings, m_hWnd);
				m_Console.SetUpConsole(m_pD3DDevice, m_ulViewWidth, m_ulViewHeight);
				SetupRenderStates();
				SetMenu(m_hWnd, m_hMenu);
			}
			ChangeDevice();
		}
	}
	m_Console.PresentCommandLine();
}

bool CMyD3DInit::ValidateDisplayMode(const D3DDISPLAYMODE &Mode)
{
	if (Mode.Width < 640 || Mode.Height < 480 || Mode.RefreshRate < 60) return false;
	return true;
}

bool CMyD3DInit::ValidateDevice(const D3DDEVTYPE &Type, const D3DCAPS9 &Caps)
{
	if (!(Caps.RasterCaps & D3DPRASTERCAPS_DITHER)) return false;
	if (!(Caps.ShadeCaps & D3DPSHADECAPS_COLORGOURAUDRGB)) return false;
	if (!(Caps.PrimitiveMiscCaps & D3DPMISCCAPS_CULLCCW)) return false;
	if (!(Caps.ZCmpCaps & D3DPCMPCAPS_LESSEQUAL)) return false;
	return true;
}

bool CMyD3DInit::ValidateVertexProcessingType(const VERTEXPROCESSING_TYPE &Type)
{
	if (Type = = MIXED_VP) return false;
	return true;
}

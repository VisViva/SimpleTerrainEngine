#ifndef CMOVEENGINE_H
#define CMOVEENGINE_H
#include "Main.h"
#include "CTimer.h"
#include "CPlayer.h"
#include "CObject.h"
#include "CConsole.h"
#include "CTerrain.h"
#include "CD3DSettingsDlg.h"

class CCamera;

class CMoveEngine
{
public:
	CMoveEngine();
	virtual ~CMoveEngine();
	enum INPUT_MODE {IM_GAME, IM_CONSOLE};
	LRESULT DisplayWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	bool InitInstance(HANDLE hInstance, LPCTSTR lpCmdLine, int iCmdShow);
	int StartEngine();
	bool StopEngine();
private:
	bool CreateDisplay();
	void SetupRenderStates();
	void FrameAdvance();
	void ProcessInput();
	void ChangeDevice();
	void SetupGameState();
	bool BuildObjects();
	void ReleaseObjects();
	void AnimateObjects();
	void ReadConsoleCommand();
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	HWND m_hWnd;
	HICON m_hIcon;
	HMENU m_hMenu;
	bool m_bLostDevice;
	bool m_bActive;
	bool m_bIsMouseIntercepted;
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	CD3DSettings m_D3DSettings;
	ULONG m_ulViewX;
	ULONG m_ulViewY;
	ULONG m_ulViewWidth;
	ULONG m_ulViewHeight;
	INPUT_MODE m_enInputMode;
	CTimer m_Timer;
	CConsole m_Console;
	D3DCOLOR m_d3dColor;
	CTerrain m_Terrain;
	CPlayer m_Player;
	CCamera *m_pCamera;
	D3DXMATRIX m_mtxIdentity;
	ULONG m_LastFrameRate;
	POINT m_OldCursorPos;
	CObject m_Object;
	CMesh m_PlayerMesh;
};
class CMyD3DInit : public CD3DInitialize
{
private:
	virtual bool ValidateDisplayMode(const D3DDISPLAYMODE& Mode);
	virtual bool ValidateDevice(const D3DDEVTYPE& Type, const D3DCAPS9 &Caps);
	virtual bool ValidateVertexProcessingType(const VERTEXPROCESSING_TYPE &Type);
};
#endif

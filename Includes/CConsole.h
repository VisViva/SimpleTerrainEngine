#ifndef CCONSOLE_H
#define CCONSOLE_H
#include "Main.h"
#include <vector>
#include <string>
#include <sstream>

class CConsole
{
public:
	CConsole();
	virtual ~CConsole();
	void ShowConsole();
	void HideConsole();
	void RenderConsole();
	void SetUpConsole(LPDIRECT3DDEVICE9 a_pD3DDevice, unsigned long a_ulViewWidth, unsigned long a_ulViewHeight);
	void SetConsoleColor(D3DCOLOR a_d3dFontColor);
	void PresentCommandLine();
	void ScrollUp();
	void ScrollDown();
	void ClearConsole();
	void Backspace();
	void RetreiveLastCommand();
	void SetFontSize(short a_sFontSize);
	void Release();
	struct VertexWithColor
	{
		FLOAT x, y, z, RHW;
		DWORD color;
	};
	std::stringstream m_ConsoleStream;
	std::stringstream m_CommandLineStream;

private:
	bool BuildConsole();
	bool m_bVisibilitySwitch;
	bool m_bAutoScroll;
	std::string m_sCachedCommand;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	short m_sTopShift;
	RECT m_rectTextArea;
	RECT m_rectCommandLineArea;
	ID3DXFont *m_d3dFont;
	short m_sFontSize;
	D3DCOLOR m_d3dFontColor;
	unsigned long m_ulTextHeight;
	short m_sNumberOfAllowedLines;
};
#endif

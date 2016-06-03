#include "..\\Includes\\CConsole.h"

CConsole::CConsole()
{
	m_pD3DDevice = NULL;
	m_ulTextHeight = 0;
	m_sNumberOfAllowedLines = 15;
	m_bVisibilitySwitch = false;
	m_bAutoScroll = true;
	SetRect(&m_rectTextArea, 0, 0, 0, 0);
	SetRect(&m_rectCommandLineArea, 0, 0, 0, 0);
	m_d3dFont = NULL;
	m_sFontSize = 0;
	m_d3dFontColor = 0xFFFFFFFF;
	m_sCachedCommand = "";
	m_CommandLineStream << "> ";
}

CConsole::~CConsole()
{
	Release();
	if (m_d3dFont) m_d3dFont = NULL;
}

void CConsole::Release()
{
	if (m_d3dFont) m_d3dFont->OnLostDevice();
	m_pD3DDevice = NULL;
}

bool CConsole::BuildConsole()
{
	D3DXCreateFont(m_pD3DDevice, m_sFontSize, 0, FW_NORMAL, 4, false, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Century Gothic", &m_d3dFont);
	return true;
};

void CConsole::ShowConsole()
{
	m_bVisibilitySwitch = true;
}

void CConsole::HideConsole()
{
	m_bVisibilitySwitch = false;
}

void CConsole::RenderConsole()
{
	if (m_bVisibilitySwitch = = false) return;
	if ((m_ulTextHeight > m_sFontSize * m_sNumberOfAllowedLines) && (m_bAutoScroll = = true))
	{
		m_rectTextArea.top = m_sFontSize * m_sNumberOfAllowedLines - m_ulTextHeight;
	}
	m_ulTextHeight = m_d3dFont->DrawText(NULL, m_ConsoleStream.str().c_str(), -1, &m_rectTextArea, DT_LEFT, m_d3dFontColor);
	m_d3dFont->DrawText(NULL, m_CommandLineStream.str().c_str(), -1, &m_rectCommandLineArea, DT_LEFT, m_d3dFontColor);
}

void CConsole::SetUpConsole(LPDIRECT3DDEVICE9 a_pD3DDevice, unsigned long a_ulViewWidth, unsigned long a_ulViewHeight)
{
	m_pD3DDevice = a_pD3DDevice;
	m_sFontSize = (short)(a_ulViewWidth / 91.4);
	SetRect(&m_rectTextArea, (unsigned long)(a_ulViewWidth / 192), 0, (a_ulViewWidth - (unsigned long)(a_ulViewWidth / 192)), m_sFontSize * m_sNumberOfAllowedLines);
	SetRect(&m_rectCommandLineArea, (unsigned long)(a_ulViewWidth / 192), a_ulViewHeight - (unsigned long)(a_ulViewWidth / 192) - m_sFontSize, a_ulViewWidth - (unsigned long)(a_ulViewWidth / 192), a_ulViewHeight - (unsigned long)(a_ulViewWidth / 192));
	if (!BuildConsole()) MessageBox(NULL, _T("Error!"), _T("In-game console cannot be built!"), MB_OK);
}

void CConsole::PresentCommandLine()
{
	m_bAutoScroll = true;
	m_sCachedCommand = m_CommandLineStream.str();
	m_ConsoleStream << m_CommandLineStream.str() << "\n";
	m_CommandLineStream.str("");
	m_CommandLineStream << "> ";
}

void CConsole::ScrollUp()
{
	if (m_ulTextHeight > (m_sNumberOfAllowedLines+1) * m_sFontSize)
	{
		m_bAutoScroll = false;
		m_rectTextArea.top + = m_sFontSize;
	}
}

void CConsole::ScrollDown()
{
	if (m_ulTextHeight + m_rectTextArea.top >  m_sNumberOfAllowedLines * m_sFontSize)
	{
		m_bAutoScroll = false;
		m_rectTextArea.top - = m_sFontSize;
	}
}

void CConsole::SetConsoleColor(D3DCOLOR a_d3dFontColor)
{
	m_d3dFontColor = a_d3dFontColor;
}

void CConsole::ClearConsole()
{
	m_bAutoScroll = false;
	m_ConsoleStream.str(std::string());
	SetRect(&m_rectTextArea, m_rectTextArea.left, 0, m_rectTextArea.right, m_sFontSize * m_sNumberOfAllowedLines);
	m_ConsoleStream << "Console has been cleared.\n";
}

void CConsole::Backspace()
{
	std::string sCommandLine(m_CommandLineStream.str());
	if (sCommandLine.size () > 2) sCommandLine.resize (sCommandLine.size() - 1);
	m_CommandLineStream.str("");
	m_CommandLineStream << sCommandLine;
}

void CConsole::RetreiveLastCommand()
{
	std::string sCommandLine(m_CommandLineStream.str());
	if (sCommandLine.size() < m_sCachedCommand.size())
	{
		m_CommandLineStream << m_sCachedCommand[sCommandLine.size()];
	}
}

void CConsole::SetFontSize(short a_sFontSize)
{
	m_sFontSize = a_sFontSize;
	if (!BuildConsole()) MessageBox(NULL, _T("Error!"), _T("In-game console cannot be built!"), MB_OK);
}

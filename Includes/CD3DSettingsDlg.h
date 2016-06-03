#ifndef CD3DSETTINGSDLG_H
#define CD3DSETTINGSDLG_H
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include "..\\Includes\\CD3DInitialize.h"

class CD3DSettingsDlg
{
public:
	CD3DSettingsDlg();
	virtual ~CD3DSettingsDlg();
	int ShowDialog (CD3DInitialize *pInitialize, CD3DSettings *pSettings, HWND a_hWndParent = NULL);
	CD3DSettings GetD3DSettings () const {return m_D3DSettings;}
private:
	BOOL SettingsDlgProc(HWND hWnd, unsigned int Message, WPARAM wParam, LPARAM lParam);
	void AdapterChanged();
	void DeviceChanged();
	void WindowedFullscreenChanged();
	void AdapterFormatChanged();
	void ResolutionChanged();
	void RefreshRateChanged();
	void BackBufferFormatChanged();
	void DepthStencilFormatChanged();
	void MultisampleTypeChanged();
	void MultisampleQualityChanged();
	void VertexProcessingChanged();
	void PresentIntervalChanged();
	void Combo_AddItem(unsigned long a_ulComboID, LPVOID a_pData, LPCTSTR a_pStrDesc);
	void Combo_SelectItem(unsigned long a_ulComboID, unsigned long a_ulIndex);
	void Combo_SelectItemData(unsigned long a_ulComboID, LPVOID a_pData);
	void Combo_Clear(unsigned long a_ulComboID);
	bool Combo_FindText(unsigned long a_ulComboID, LPCTSTR a_pStrText);
	unsigned long Combo_GetItemCount(unsigned long a_ulComboID);
	LPVOID Combo_GetSelectedItemData(unsigned long a_ulComboID);
	CD3DSettings::Settings* GetSettings(){return m_D3DSettings.GetSettings();}
	static BOOL CALLBACK StaticDlgProc(HWND hWnd, unsigned int Message, WPARAM wParam, LPARAM lParam);
	HWND m_hWndDlg;
	CD3DInitialize *m_pInitialize;
	CD3DSettings m_D3DSettings;
};
#endif

#include "..\\Includes\\CD3DSettingsDlg.h"

#define IDD_DIALOG_DIRECTX_SETTINGS 101
#define IDI_ICON_MOVE 102
#define IDC_COMBO_ADAPTER 1001
#define IDC_COMBO_DEVICE 1002
#define IDC_COMBO_ADAPTER_FORMAT 1003
#define IDC_COMBO_RESOLUTION 1004
#define IDC_COMBO_REFRESH_RATE 1005
#define IDC_COMBO_BACK_BUFFER_FORMAT 1006
#define IDC_COMBO_DEPTH_STENCIL_BUFFER_FORMAT 1007
#define IDC_COMBO_MULTISAMPLE_TYPE 1008
#define IDC_COMBO_MULTISAMPLE_QUALITY 1009
#define IDC_COMBO_VERTEX_PROCESSING 1010
#define IDC_COMBO_PRESENT_INTERVAL 1011
#define IDC_RADIO_WINDOWED 1012
#define IDC_RADIO_FULLSCREEN 1013

LPCTSTR D3DDevTypeToString rD3DDEVTYPE devType);
LPCTSTR MultisampleTypeToString rD3DMULTISAMPLE_TYPE MultiSampleType);
LPCTSTR VertexProcessingTypeToString rVERTEXPROCESSING_TYPE vpt);
LPCTSTR D3DFormatToString rD3DFORMAT format, bool bWithPrefix = true);
LPCTSTR PresentIntervalToString runsigned int pi);

LPCTSTR D3DDevTypeToString(D3DDEVTYPE devType)
{
	switch (devType)
	{
		case D3DDEVTYPE_HAL: return _T("D3DDEVTYPE_HAL");
		case D3DDEVTYPE_SW: return _T("D3DDEVTYPE_SW");
		case D3DDEVTYPE_REF: return _T("D3DDEVTYPE_REF");
		default: return _T("Unknown D3DDEVTYPE");
	}
}

LPCTSTR MultisampleTypeToString( D3DMULTISAMPLE_TYPE MultiSampleType )
{
	switch (MultiSampleType)
	{
		case D3DMULTISAMPLE_NONE: return _T("D3DMULTISAMPLE_NONE");
		case D3DMULTISAMPLE_NONMASKABLE: return _T("D3DMULTISAMPLE_NONMASKABLE");
		case D3DMULTISAMPLE_2_SAMPLES: return _T("D3DMULTISAMPLE_2_SAMPLES");
		case D3DMULTISAMPLE_3_SAMPLES: return _T("D3DMULTISAMPLE_3_SAMPLES");
		case D3DMULTISAMPLE_4_SAMPLES: return _T("D3DMULTISAMPLE_4_SAMPLES");
		case D3DMULTISAMPLE_5_SAMPLES: return _T("D3DMULTISAMPLE_5_SAMPLES");
		case D3DMULTISAMPLE_6_SAMPLES: return _T("D3DMULTISAMPLE_6_SAMPLES");
		case D3DMULTISAMPLE_7_SAMPLES: return _T("D3DMULTISAMPLE_7_SAMPLES");
		case D3DMULTISAMPLE_8_SAMPLES: return _T("D3DMULTISAMPLE_8_SAMPLES");
		case D3DMULTISAMPLE_9_SAMPLES: return _T("D3DMULTISAMPLE_9_SAMPLES");
		case D3DMULTISAMPLE_10_SAMPLES: return _T("D3DMULTISAMPLE_10_SAMPLES");
		case D3DMULTISAMPLE_11_SAMPLES: return _T("D3DMULTISAMPLE_11_SAMPLES");
		case D3DMULTISAMPLE_12_SAMPLES: return _T("D3DMULTISAMPLE_12_SAMPLES");
		case D3DMULTISAMPLE_13_SAMPLES: return _T("D3DMULTISAMPLE_13_SAMPLES");
		case D3DMULTISAMPLE_14_SAMPLES: return _T("D3DMULTISAMPLE_14_SAMPLES");
		case D3DMULTISAMPLE_15_SAMPLES: return _T("D3DMULTISAMPLE_15_SAMPLES");
		case D3DMULTISAMPLE_16_SAMPLES: return _T("D3DMULTISAMPLE_16_SAMPLES");
		default: return _T("Unknown D3DMULTISAMPLE_TYPE");
	}
}

LPCTSTR VertexProcessingTypeToString(VERTEXPROCESSING_TYPE vpt)
{
	switch (vpt)
	{
		case SOFTWARE_VP: return _T("SOFTWARE_VP");
		case MIXED_VP: return _T("MIXED_VP");
		case HARDWARE_VP: return _T("HARDWARE_VP");
		case PURE_HARDWARE_VP: return _T("PURE_HARDWARE_VP");
		default: return _T("Unknown VERTEXPROCESSING_TYPE");
	}
}

LPCTSTR PresentIntervalToString(unsigned int pi)
{
	switch(pi)
	{
		case D3DPRESENT_INTERVAL_IMMEDIATE: return _T("D3DPRESENT_INTERVAL_IMMEDIATE");
		case D3DPRESENT_INTERVAL_DEFAULT: return _T("D3DPRESENT_INTERVAL_DEFAULT");
		case D3DPRESENT_INTERVAL_ONE: return _T("D3DPRESENT_INTERVAL_ONE");
		case D3DPRESENT_INTERVAL_TWO: return _T("D3DPRESENT_INTERVAL_TWO");
		case D3DPRESENT_INTERVAL_THREE: return _T("D3DPRESENT_INTERVAL_THREE");
		case D3DPRESENT_INTERVAL_FOUR: return _T("D3DPRESENT_INTERVAL_FOUR");
		default: return _T("Unknown Present-Interval");
	}
}

LPCTSTR D3DFormatToString( D3DFORMAT format, bool bWithPrefix )
{
	LPTSTR pstr = NULL;
	switch(format)
	{
		case D3DFMT_UNKNOWN: pstr = _T("D3DFMT_UNKNOWN"); break;
		case D3DFMT_R8G8B8: pstr = _T("D3DFMT_R8G8B8"); break;
		case D3DFMT_A8R8G8B8: pstr = _T("D3DFMT_A8R8G8B8"); break;
		case D3DFMT_X8R8G8B8: pstr = _T("D3DFMT_X8R8G8B8"); break;
		case D3DFMT_R5G6B5: pstr = _T("D3DFMT_R5G6B5"); break;
		case D3DFMT_X1R5G5B5: pstr = _T("D3DFMT_X1R5G5B5"); break;
		case D3DFMT_A1R5G5B5: pstr = _T("D3DFMT_A1R5G5B5"); break;
		case D3DFMT_A4R4G4B4: pstr = _T("D3DFMT_A4R4G4B4"); break;
		case D3DFMT_R3G3B2: pstr = _T("D3DFMT_R3G3B2"); break;
		case D3DFMT_A8: pstr = _T("D3DFMT_A8"); break;
		case D3DFMT_A8R3G3B2: pstr = _T("D3DFMT_A8R3G3B2"); break;
		case D3DFMT_X4R4G4B4: pstr = _T("D3DFMT_X4R4G4B4"); break;
		case D3DFMT_A2B10G10R10: pstr = _T("D3DFMT_A2B10G10R10"); break;
		case D3DFMT_A8B8G8R8: pstr = _T("D3DFMT_A8B8G8R8"); break;
		case D3DFMT_X8B8G8R8: pstr = _T("D3DFMT_X8B8G8R8"); break;
		case D3DFMT_G16R16: pstr = _T("D3DFMT_G16R16"); break;
		case D3DFMT_A2R10G10B10: pstr = _T("D3DFMT_A2R10G10B10"); break;
		case D3DFMT_A16B16G16R16: pstr = _T("D3DFMT_A16B16G16R16"); break;
		case D3DFMT_A8P8: pstr = _T("D3DFMT_A8P8"); break;
		case D3DFMT_P8: pstr = _T("D3DFMT_P8"); break;
		case D3DFMT_L8: pstr = _T("D3DFMT_L8"); break;
		case D3DFMT_A8L8: pstr = _T("D3DFMT_A8L8"); break;
		case D3DFMT_A4L4: pstr = _T("D3DFMT_A4L4"); break;
		case D3DFMT_V8U8: pstr = _T("D3DFMT_V8U8"); break;
		case D3DFMT_L6V5U5: pstr = _T("D3DFMT_L6V5U5"); break;
		case D3DFMT_X8L8V8U8: pstr = _T("D3DFMT_X8L8V8U8"); break;
		case D3DFMT_Q8W8V8U8: pstr = _T("D3DFMT_Q8W8V8U8"); break;
		case D3DFMT_V16U16: pstr = _T("D3DFMT_V16U16"); break;
		case D3DFMT_A2W10V10U10: pstr = _T("D3DFMT_A2W10V10U10"); break;
		case D3DFMT_UYVY: pstr = _T("D3DFMT_UYVY"); break;
		case D3DFMT_YUY2: pstr = _T("D3DFMT_YUY2"); break;
		case D3DFMT_DXT1: pstr = _T("D3DFMT_DXT1"); break;
		case D3DFMT_DXT2: pstr = _T("D3DFMT_DXT2"); break;
		case D3DFMT_DXT3: pstr = _T("D3DFMT_DXT3"); break;
		case D3DFMT_DXT4: pstr = _T("D3DFMT_DXT4"); break;
		case D3DFMT_DXT5: pstr = _T("D3DFMT_DXT5"); break;
		case D3DFMT_D16_LOCKABLE: pstr = _T("D3DFMT_D16_LOCKABLE"); break;
		case D3DFMT_D32: pstr = _T("D3DFMT_D32"); break;
		case D3DFMT_D15S1: pstr = _T("D3DFMT_D15S1"); break;
		case D3DFMT_D24S8: pstr = _T("D3DFMT_D24S8"); break;
		case D3DFMT_D24X8: pstr = _T("D3DFMT_D24X8"); break;
		case D3DFMT_D24X4S4: pstr = _T("D3DFMT_D24X4S4"); break;
		case D3DFMT_D16: pstr = _T("D3DFMT_D16"); break;
		case D3DFMT_L16: pstr = _T("D3DFMT_L16"); break;
		case D3DFMT_VERTEXDATA: pstr = _T("D3DFMT_VERTEXDATA"); break;
		case D3DFMT_INDEX16: pstr = _T("D3DFMT_INDEX16"); break;
		case D3DFMT_INDEX32: pstr = _T("D3DFMT_INDEX32"); break;
		case D3DFMT_Q16W16V16U16: pstr = _T("D3DFMT_Q16W16V16U16"); break;
		case D3DFMT_MULTI2_ARGB8: pstr = _T("D3DFMT_MULTI2_ARGB8"); break;
		case D3DFMT_R16F: pstr = _T("D3DFMT_R16F"); break;
		case D3DFMT_G16R16F: pstr = _T("D3DFMT_G16R16F"); break;
		case D3DFMT_A16B16G16R16F: pstr = _T("D3DFMT_A16B16G16R16F"); break;
		case D3DFMT_R32F: pstr = _T("D3DFMT_R32F"); break;
		case D3DFMT_G32R32F: pstr = _T("D3DFMT_G32R32F"); break;
		case D3DFMT_A32B32G32R32F: pstr = _T("D3DFMT_A32B32G32R32F"); break;
		case D3DFMT_CxV8U8: pstr = _T("D3DFMT_CxV8U8"); break;
		default: pstr = _T("Unknown format"); break;
	}
	if(bWithPrefix || _tcsstr(pstr, _T("D3DFMT_")) = = NULL) return pstr;
	else return pstr + lstrlen(_T("D3DFMT_"));
}

CD3DSettingsDlg::CD3DSettingsDlg()
{
	m_hWndDlg = NULL;
	m_pInitialize = NULL;
}

CD3DSettingsDlg::~CD3DSettingsDlg()
{
}

int CD3DSettingsDlg::ShowDialog(CD3DInitialize *pInitialize, CD3DSettings *pSettings, HWND a_hWndParent)
{
	m_pInitialize = pInitialize;
	if (pSettings)
	{
		m_D3DSettings = *pSettings;
	}
	else
	{
		ZeroMemory(&m_D3DSettings, sizeof(CD3DSettings));
		m_D3DSettings.Windowed = true;
		D3DDISPLAYMODE DisplayMode;
		m_pInitialize->GetDirect3D()->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode);
		GetSettings()->m_ulAdapterOrdinal = D3DADAPTER_DEFAULT;
		GetSettings()->m_d3dDisplayMode = DisplayMode;
	}
	return DialogBoxParam(nullptr, MAKEINTRESOURCE(IDD_DIALOG_DIRECTX_SETTINGS), a_hWndParent, StaticDlgProc, (LPARAM)this);
}

BOOL CALLBACK CD3DSettingsDlg::StaticDlgProc(HWND hWnd, unsigned int Message, WPARAM wParam, LPARAM lParam)
{
	if (Message = = WM_INITDIALOG) SetWindowLong(hWnd, GWL_USERDATA, lParam);
	CD3DSettingsDlg *Destination = (CD3DSettingsDlg*)GetWindowLong(hWnd, GWL_USERDATA);
	if (Destination) return Destination->SettingsDlgProc(hWnd, Message, wParam, lParam);
	return false;
}

BOOL CD3DSettingsDlg::SettingsDlgProc(HWND hWnd, unsigned int Message, WPARAM wParam, LPARAM lParam)
{
	unsigned long i;
	switch (Message)
	{
		case WM_INITDIALOG:
		m_hWndDlg = hWnd;
		for (i = 0; i < m_pInitialize->GetAdapterCount(); i++)
		{
			const CD3DEnumAdapter *pAdapter = m_pInitialize->GetAdapter(i);
			Combo_AddItem(IDC_COMBO_ADAPTER, (LPVOID)pAdapter, pAdapter->m_d3dIdentifier.Description);
			if (pAdapter->m_ulOrdinal = = GetSettings()->m_ulAdapterOrdinal)
			{
				Combo_SelectItemData(IDC_COMBO_ADAPTER, (LPVOID)pAdapter);
			}
		}
		if (Combo_GetSelectedItemData(IDC_COMBO_ADAPTER) = = NULL && Combo_GetItemCount(IDC_COMBO_ADAPTER) > 0)
		{
			Combo_SelectItem(IDC_COMBO_ADAPTER, 0);
		}
		return true;
		case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDOK:
			EndDialog(hWnd, IDOK);
			break;
			case IDCANCEL:
			EndDialog(hWnd, IDCANCEL);
			break;
			case IDC_COMBO_ADAPTER:
			if(CBN_SELCHANGE = = HIWORD(wParam)) AdapterChanged();
			break;
			case IDC_COMBO_DEVICE:
			if(CBN_SELCHANGE = = HIWORD(wParam)) DeviceChanged();
			break;
			case IDC_COMBO_ADAPTER_FORMAT:
			if(CBN_SELCHANGE = = HIWORD(wParam)) AdapterFormatChanged();
			break;
			case IDC_COMBO_RESOLUTION:
			if(CBN_SELCHANGE = = HIWORD(wParam)) ResolutionChanged();
			break;
			case IDC_COMBO_REFRESH_RATE:
			if(CBN_SELCHANGE = = HIWORD(wParam)) RefreshRateChanged();
			break;
			case IDC_COMBO_BACK_BUFFER_FORMAT:
			if(CBN_SELCHANGE = = HIWORD(wParam)) BackBufferFormatChanged();
			break;
			case IDC_COMBO_DEPTH_STENCIL_BUFFER_FORMAT:
			if(CBN_SELCHANGE = = HIWORD(wParam)) DepthStencilFormatChanged();
			break;
			case IDC_COMBO_MULTISAMPLE_TYPE:
			if(CBN_SELCHANGE = = HIWORD(wParam)) MultisampleTypeChanged();
			break;
			case IDC_COMBO_MULTISAMPLE_QUALITY:
			if(CBN_SELCHANGE = = HIWORD(wParam)) MultisampleQualityChanged();
			break;
			case IDC_COMBO_VERTEX_PROCESSING:
			if(CBN_SELCHANGE = = HIWORD(wParam)) VertexProcessingChanged();
			break;
			case IDC_COMBO_PRESENT_INTERVAL:
			if(CBN_SELCHANGE = = HIWORD(wParam)) PresentIntervalChanged();
			break;
			case IDC_RADIO_WINDOWED:
			case IDC_RADIO_FULLSCREEN:
			WindowedFullscreenChanged();
			break;
		}
		break;
		default:
		return false;
	}
	return true;
}

void CD3DSettingsDlg::AdapterChanged()
{
	CD3DEnumAdapter *pAdapter = (CD3DEnumAdapter*)Combo_GetSelectedItemData(IDC_COMBO_ADAPTER);
	if(pAdapter = = NULL) return;
	GetSettings()->m_ulAdapterOrdinal = pAdapter->m_ulOrdinal;
	if (m_D3DSettings.Windowed = = true)
	{
		D3DDISPLAYMODE DisplayMode;
		m_pInitialize->GetDirect3D()->GetAdapterDisplayMode(pAdapter->m_ulOrdinal, &DisplayMode);
		GetSettings()->m_d3dDisplayMode = DisplayMode;
	}
	Combo_Clear(IDC_COMBO_DEVICE);
	for(unsigned long i = 0; i < pAdapter->m_vDevices.size(); i++)
	{
		CD3DEnumDevice *pDevice = pAdapter->m_vDevices[i];
		Combo_AddItem(IDC_COMBO_DEVICE, pDevice, D3DDevTypeToString(pDevice->m_d3dDeviceType));
		if (pDevice->m_d3dDeviceType = = GetSettings()->m_d3dDeviceType)
		{
			Combo_SelectItemData(IDC_COMBO_DEVICE, pDevice);
		}
	}
	if (Combo_GetSelectedItemData(IDC_COMBO_DEVICE) = = NULL && Combo_GetItemCount(IDC_COMBO_DEVICE) > 0)
	{
		Combo_SelectItem(IDC_COMBO_DEVICE, 0);
	}
}

void CD3DSettingsDlg::DeviceChanged()
{
	CD3DEnumDevice *pDevice = (CD3DEnumDevice*)Combo_GetSelectedItemData(IDC_COMBO_DEVICE);
	if(pDevice = = NULL) return;
	GetSettings()->m_d3dDeviceType = pDevice->m_d3dDeviceType;
	bool HasWindowedDeviceOption = false;
	bool HasFullscreenDeviceOption = false;
	for(ULONG i = 0; i < pDevice->m_vOptions.size(); i++)
	{
		CD3DEnumDeviceOptions *pDeviceOptions = (CD3DEnumDeviceOptions*)pDevice->m_vOptions[i];
		if (pDeviceOptions->m_bWindowed) HasWindowedDeviceOption = true; else HasFullscreenDeviceOption = true;
	}
	EnableWindow(GetDlgItem(m_hWndDlg, IDC_RADIO_WINDOWED), HasWindowedDeviceOption);
	EnableWindow(GetDlgItem(m_hWndDlg, IDC_RADIO_FULLSCREEN), HasFullscreenDeviceOption);
	if (m_D3DSettings.Windowed && HasWindowedDeviceOption)
	{
		CheckRadioButton(m_hWndDlg, IDC_RADIO_WINDOWED, IDC_RADIO_FULLSCREEN, IDC_RADIO_WINDOWED);
	}
	else
	{
		CheckRadioButton(m_hWndDlg, IDC_RADIO_WINDOWED, IDC_RADIO_FULLSCREEN, IDC_RADIO_FULLSCREEN);
	}
	WindowedFullscreenChanged();
}

void CD3DSettingsDlg::WindowedFullscreenChanged()
{
	CD3DEnumAdapter *pAdapter = (CD3DEnumAdapter*)Combo_GetSelectedItemData(IDC_COMBO_ADAPTER);
	CD3DEnumDevice *pDevice = (CD3DEnumDevice*)Combo_GetSelectedItemData(IDC_COMBO_DEVICE);
	if(pAdapter = = NULL || pDevice = = NULL) return;
	if (IsDlgButtonChecked(m_hWndDlg, IDC_RADIO_WINDOWED))
	{
		m_D3DSettings.Windowed = true;
		GetSettings()->m_ulAdapterOrdinal = pAdapter->m_ulOrdinal;
		GetSettings()->m_d3dDeviceType = pDevice->m_d3dDeviceType;
		D3DFORMAT Format = GetSettings()->m_d3dDisplayMode.Format;
		Combo_Clear(IDC_COMBO_ADAPTER_FORMAT);
		Combo_AddItem(IDC_COMBO_ADAPTER_FORMAT, (LPVOID)Format, D3DFormatToString(Format));
		Combo_SelectItem(IDC_COMBO_ADAPTER_FORMAT, 0);
		EnableWindow(GetDlgItem(m_hWndDlg, IDC_COMBO_ADAPTER_FORMAT), false);
		unsigned long ulResolutionData, Width, Height;
		TCHAR strResolution[50];
		Width = GetSettings()->m_d3dDisplayMode.Width;
		Height = GetSettings()->m_d3dDisplayMode.Height;
		ulResolutionData = MAKELONG(Width, Height);
		_sntprintf(strResolution, 50, _T("%d x %d"), Width, Height);
		strResolution[49] = 0;
		Combo_Clear(IDC_COMBO_RESOLUTION);
		Combo_AddItem(IDC_COMBO_RESOLUTION, (LPVOID)ulResolutionData, strResolution);
		Combo_SelectItem(IDC_COMBO_RESOLUTION, 0);
		EnableWindow(GetDlgItem(m_hWndDlg, IDC_COMBO_RESOLUTION), false);
		TCHAR strRefreshRate[50];
		unsigned long ulRefreshRate = GetSettings()->m_d3dDisplayMode.RefreshRate;
		_sntprintf(strRefreshRate, 50, _T("%d Hz"), ulRefreshRate);
		strRefreshRate[49] = 0;
		Combo_Clear(IDC_COMBO_REFRESH_RATE);
		Combo_AddItem(IDC_COMBO_REFRESH_RATE, (LPVOID)ulRefreshRate, strRefreshRate);
		Combo_SelectItem(IDC_COMBO_REFRESH_RATE, 0);
		EnableWindow(GetDlgItem(m_hWndDlg, IDC_COMBO_REFRESH_RATE), false);
	}
	else
	{
		m_D3DSettings.Windowed = false;
		GetSettings()->m_ulAdapterOrdinal = pAdapter->m_ulOrdinal;
		GetSettings()->m_d3dDeviceType = pDevice->m_d3dDeviceType;
		Combo_Clear(IDC_COMBO_ADAPTER_FORMAT);
		for (unsigned long i = 0; i < pDevice->m_vOptions.size(); i++)
		{
			CD3DEnumDeviceOptions *pDeviceOptions = pDevice->m_vOptions[i];
			D3DFORMAT AFormat = pDeviceOptions->m_d3dAdapterFormat;
			if (!Combo_FindText(IDC_COMBO_ADAPTER_FORMAT, D3DFormatToString(AFormat)))
			{
				Combo_AddItem(IDC_COMBO_ADAPTER_FORMAT, (LPVOID)AFormat, D3DFormatToString(AFormat));
				if (AFormat = = GetSettings()->m_d3dDisplayMode.Format)
				{
					Combo_SelectItemData(IDC_COMBO_ADAPTER_FORMAT, (LPVOID)AFormat);
				}
			}
		}
		if (Combo_GetSelectedItemData(IDC_COMBO_ADAPTER_FORMAT) = = NULL && Combo_GetItemCount(IDC_COMBO_ADAPTER_FORMAT) > 0)
		{
			Combo_SelectItem(IDC_COMBO_ADAPTER_FORMAT, 0);
		}
		EnableWindow(GetDlgItem(m_hWndDlg, IDC_COMBO_ADAPTER_FORMAT), true);
		EnableWindow(GetDlgItem(m_hWndDlg, IDC_COMBO_RESOLUTION), true);
		EnableWindow(GetDlgItem(m_hWndDlg, IDC_COMBO_REFRESH_RATE), true);
	}
}

void CD3DSettingsDlg::AdapterFormatChanged()
{
	unsigned long i;
	D3DFORMAT AFormat;
	if (m_D3DSettings.Windowed = = false)
	{
		CD3DEnumAdapter * pAdapter = (CD3DEnumAdapter*)Combo_GetSelectedItemData(IDC_COMBO_ADAPTER);
		if (pAdapter = = NULL) return;
		AFormat = (D3DFORMAT)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_ADAPTER_FORMAT));
		GetSettings()->m_d3dDisplayMode.Format = AFormat;
		Combo_Clear(IDC_COMBO_RESOLUTION);
		for (i = 0; i < pAdapter->m_vModes.size(); i++)
		{
			D3DDISPLAYMODE Mode = pAdapter->m_vModes[i];
			if (Mode.Format = = AFormat)
			{
				ULONG ulResolutionData;
				TCHAR strResolution[50];
				ulResolutionData = MAKELONG(Mode.Width, Mode.Height);
				_sntprintf(strResolution, 50, _T("%d x %d"), Mode.Width, Mode.Height);
				strResolution[49] = 0;
				if (!Combo_FindText(IDC_COMBO_RESOLUTION, strResolution))
				{
					Combo_AddItem(IDC_COMBO_RESOLUTION, (LPVOID)ulResolutionData, strResolution);
					if (GetSettings()->m_d3dDisplayMode.Width = = Mode.Width && GetSettings()->m_d3dDisplayMode.Height = = Mode.Height)
					{
						Combo_SelectItemData(IDC_COMBO_RESOLUTION, (LPVOID)ulResolutionData);
					}
				}
			}
		}
		if (Combo_GetSelectedItemData(IDC_COMBO_RESOLUTION) = = NULL && Combo_GetItemCount(IDC_COMBO_RESOLUTION) > 0)
		{
			Combo_SelectItem(IDC_COMBO_RESOLUTION, 0);
		}
	}
	CD3DEnumDevice *pDevice = (CD3DEnumDevice *)Combo_GetSelectedItemData(IDC_COMBO_DEVICE);
	if(pDevice = = NULL) return;
	Combo_Clear(IDC_COMBO_BACK_BUFFER_FORMAT);
	for (i = 0; i < pDevice->m_vOptions.size(); i++)
	{
		CD3DEnumDeviceOptions *pDeviceOptions = pDevice->m_vOptions[i];
		if (pDeviceOptions->m_bWindowed = = m_D3DSettings.Windowed && pDeviceOptions->m_d3dAdapterFormat = = GetSettings()->m_d3dDisplayMode.Format)
		{
			if (!Combo_FindText(IDC_COMBO_BACK_BUFFER_FORMAT, D3DFormatToString(pDeviceOptions->m_d3dBackBufferFormat)))
			{
				Combo_AddItem(IDC_COMBO_BACK_BUFFER_FORMAT, (LPVOID)pDeviceOptions->m_d3dBackBufferFormat, D3DFormatToString(pDeviceOptions->m_d3dBackBufferFormat));
				if (GetSettings()->m_d3dBackBufferFormat = = pDeviceOptions->m_d3dBackBufferFormat)
				{
					Combo_SelectItemData(IDC_COMBO_BACK_BUFFER_FORMAT, (LPVOID)pDeviceOptions->m_d3dBackBufferFormat);
				}
			}
		}
	}
	if ( Combo_GetSelectedItemData(IDC_COMBO_BACK_BUFFER_FORMAT) = = NULL && Combo_GetItemCount(IDC_COMBO_BACK_BUFFER_FORMAT) > 0)
	{
		Combo_SelectItem(IDC_COMBO_BACK_BUFFER_FORMAT, 0);
	}
}

void CD3DSettingsDlg::ResolutionChanged()
{
	unsigned long i;
	if (m_D3DSettings.Windowed) return;
	CD3DEnumAdapter *pAdapter = (CD3DEnumAdapter*)Combo_GetSelectedItemData(IDC_COMBO_ADAPTER);
	if(pAdapter = = NULL) return;
	unsigned long ulResolutionData = (unsigned long)Combo_GetSelectedItemData(IDC_COMBO_RESOLUTION);
	unsigned long Width = LOWORD(ulResolutionData);
	unsigned long Height = HIWORD(ulResolutionData);
	GetSettings()->m_d3dDisplayMode.Width = Width;
	GetSettings()->m_d3dDisplayMode.Height = Height;
	D3DFORMAT AFormat = (D3DFORMAT)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_ADAPTER_FORMAT));
	Combo_Clear(IDC_COMBO_REFRESH_RATE);
	for (i = 0; i < pAdapter->m_vModes.size(); i++)
	{
		D3DDISPLAYMODE Mode = pAdapter->m_vModes[i];
		if (Mode.Format = = AFormat && Mode.Width = = Width && Mode.Height = = Height)
		{
			TCHAR strRefreshRate[50];
			_sntprintf(strRefreshRate, 50, _T("%d Hz"), Mode.RefreshRate);
			strRefreshRate[49] = 0;
			if (!Combo_FindText(IDC_COMBO_REFRESH_RATE, strRefreshRate))
			{
				Combo_AddItem(IDC_COMBO_REFRESH_RATE, (LPVOID)Mode.RefreshRate, strRefreshRate);
				if (GetSettings()->m_d3dDisplayMode.RefreshRate = = Mode.RefreshRate)
				{
					Combo_SelectItemData(IDC_COMBO_REFRESH_RATE, (LPVOID)Mode.RefreshRate);
				}
			}
		}
	}
	if (Combo_GetSelectedItemData(IDC_COMBO_REFRESH_RATE) = = NULL && Combo_GetItemCount(IDC_COMBO_REFRESH_RATE) > 0)
	{
		Combo_SelectItem(IDC_COMBO_REFRESH_RATE, 0);
	}
}

void CD3DSettingsDlg::RefreshRateChanged()
{
	if(m_D3DSettings.Windowed) return;
	GetSettings()->m_d3dDisplayMode.RefreshRate = (unsigned long)Combo_GetSelectedItemData(IDC_COMBO_REFRESH_RATE);
}

void CD3DSettingsDlg::BackBufferFormatChanged()
{
	unsigned long i, j;
	CD3DEnumDevice *pDevice = (CD3DEnumDevice*)Combo_GetSelectedItemData(IDC_COMBO_DEVICE);
	if (!pDevice) return;
	D3DFORMAT AFormat = (D3DFORMAT)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_ADAPTER_FORMAT));
	D3DFORMAT BBFormat = (D3DFORMAT)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_BACK_BUFFER_FORMAT));
	for (i = 0; i < pDevice->m_vOptions.size(); i++)
	{
		CD3DEnumDeviceOptions *pDeviceOptions = pDevice->m_vOptions[i];
		if (pDeviceOptions->m_bWindowed = = m_D3DSettings.Windowed && pDeviceOptions->m_d3dAdapterFormat = = AFormat && pDeviceOptions->m_d3dBackBufferFormat = = BBFormat)
		{
			GetSettings()->m_d3dBackBufferFormat = BBFormat;
			Combo_Clear(IDC_COMBO_DEPTH_STENCIL_BUFFER_FORMAT);
			for (j = 0; j < pDeviceOptions->m_vDepthFormats.size(); j++)
			{
				D3DFORMAT Format = pDeviceOptions->m_vDepthFormats[j];
				Combo_AddItem(IDC_COMBO_DEPTH_STENCIL_BUFFER_FORMAT, (LPVOID)Format, D3DFormatToString(Format));
				if (Format = = GetSettings()->m_d3dDepthStencilFormat)
				{
					Combo_SelectItemData(IDC_COMBO_DEPTH_STENCIL_BUFFER_FORMAT, (LPVOID)Format);
				}
			}
			if (Combo_GetSelectedItemData(IDC_COMBO_DEPTH_STENCIL_BUFFER_FORMAT) = = NULL && Combo_GetItemCount(IDC_COMBO_DEPTH_STENCIL_BUFFER_FORMAT) > 0)
			{
				Combo_SelectItem(IDC_COMBO_DEPTH_STENCIL_BUFFER_FORMAT, 0);
			}
			Combo_Clear(IDC_COMBO_MULTISAMPLE_TYPE);
			for (j = 0; j < pDeviceOptions->m_vMultiSampleTypes.size(); j++)
			{
				D3DMULTISAMPLE_TYPE Type = pDeviceOptions->m_vMultiSampleTypes[j];
				Combo_AddItem(IDC_COMBO_MULTISAMPLE_TYPE, (LPVOID)Type, MultisampleTypeToString(Type));
				if (Type = = GetSettings()->m_d3dMultiSampleType)
				{
					Combo_SelectItemData(IDC_COMBO_MULTISAMPLE_TYPE, (LPVOID)Type);
				}
			}
			if (Combo_GetSelectedItemData(IDC_COMBO_MULTISAMPLE_TYPE) = = NULL && Combo_GetItemCount(IDC_COMBO_MULTISAMPLE_TYPE) > 0)
			{
				Combo_SelectItem(IDC_COMBO_MULTISAMPLE_TYPE, 0);
			}
			Combo_Clear(IDC_COMBO_VERTEX_PROCESSING);
			for (j = 0; j < pDeviceOptions->m_vVertexProcessingTypes.size(); j++)
			{
				VERTEXPROCESSING_TYPE Type = pDeviceOptions->m_vVertexProcessingTypes[j];
				Combo_AddItem(IDC_COMBO_VERTEX_PROCESSING, (LPVOID)Type, VertexProcessingTypeToString(Type));
				if (Type = = GetSettings()->m_VertexProcessingType)
				{
					Combo_SelectItemData(IDC_COMBO_VERTEX_PROCESSING, (LPVOID)Type);
				}
			}
			if (Combo_GetSelectedItemData(IDC_COMBO_VERTEX_PROCESSING) = = NULL && Combo_GetItemCount(IDC_COMBO_VERTEX_PROCESSING) > 0)
			{
				Combo_SelectItem(IDC_COMBO_VERTEX_PROCESSING, 0);
			}
			Combo_Clear(IDC_COMBO_PRESENT_INTERVAL);
			for (j = 0; j < pDeviceOptions->m_vPresentIntervals.size(); j++)
			{
				unsigned long Interval = pDeviceOptions->m_vPresentIntervals[j];
				Combo_AddItem(IDC_COMBO_PRESENT_INTERVAL, (LPVOID)Interval, PresentIntervalToString(Interval));
				if (Interval = = GetSettings()->m_ulPresentInterval)
				{
					Combo_SelectItemData(IDC_COMBO_PRESENT_INTERVAL, (LPVOID)Interval);
				}
			}
			if (Combo_GetSelectedItemData(IDC_COMBO_PRESENT_INTERVAL) = = NULL && Combo_GetItemCount(IDC_COMBO_PRESENT_INTERVAL) > 0)
			{
				Combo_SelectItem(IDC_COMBO_PRESENT_INTERVAL, 0);
			}
			break;
		}
	}
}

void CD3DSettingsDlg::DepthStencilFormatChanged()
{
	D3DFORMAT Format = (D3DFORMAT)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_DEPTH_STENCIL_BUFFER_FORMAT));
	GetSettings()->m_d3dDepthStencilFormat = Format;
}

void CD3DSettingsDlg::MultisampleTypeChanged()
{
	unsigned long i;
	D3DMULTISAMPLE_TYPE MSType;
	MSType = (D3DMULTISAMPLE_TYPE)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_MULTISAMPLE_TYPE));
	GetSettings()->m_d3dMultiSampleType = MSType;
	CD3DEnumDevice *pDevice = (CD3DEnumDevice*)Combo_GetSelectedItemData(IDC_COMBO_DEVICE);
	if (!pDevice) return;
	CD3DEnumDeviceOptions *pDeviceOptions = NULL;
	for (i = 0; i < pDevice->m_vOptions.size(); i++)
	{
		pDeviceOptions = pDevice->m_vOptions[i];
		if (pDeviceOptions->m_bWindowed = = m_D3DSettings.Windowed &&
			pDeviceOptions->m_d3dAdapterFormat = = GetSettings()->m_d3dDisplayMode.Format &&
			pDeviceOptions->m_d3dBackBufferFormat = = GetSettings()->m_d3dBackBufferFormat) break;
			pDeviceOptions = NULL;
		}
		if (!pDeviceOptions) return;
		unsigned long  MaxQuality = 0;
		for (i = 0; i < pDeviceOptions->m_vMultiSampleTypes.size(); i++)
		{
			D3DMULTISAMPLE_TYPE Type = pDeviceOptions->m_vMultiSampleTypes[i];
			if (Type = = MSType)
			{
				MaxQuality = pDeviceOptions->m_vMultiSampleQuality[i];
				break;
			}
		}
		Combo_Clear(IDC_COMBO_MULTISAMPLE_QUALITY);
		for (i = 0; i < MaxQuality; i++)
		{
			TCHAR strQuality[50];
			_sntprintf(strQuality, 50, _T("%d"), i);
			strQuality[49] = 0;
			Combo_AddItem(IDC_COMBO_MULTISAMPLE_QUALITY, (LPVOID)i, strQuality);
			if (i = = GetSettings()->m_ulMultiSampleQuality)
			{
				Combo_SelectItemData(IDC_COMBO_MULTISAMPLE_QUALITY, (LPVOID)i);
			}
		}
		if (Combo_GetSelectedItemData(IDC_COMBO_MULTISAMPLE_QUALITY) = = NULL && Combo_GetItemCount(IDC_COMBO_MULTISAMPLE_QUALITY) > 0)
		{
			Combo_SelectItem(IDC_COMBO_MULTISAMPLE_QUALITY, 0);
		}
	}

	void CD3DSettingsDlg::MultisampleQualityChanged()
	{
		unsigned long MSQuality = (unsigned long)Combo_GetSelectedItemData(IDC_COMBO_MULTISAMPLE_QUALITY);
		GetSettings()->m_ulMultiSampleQuality = MSQuality;
	}

	void CD3DSettingsDlg::VertexProcessingChanged()
	{
		VERTEXPROCESSING_TYPE VPType;
		VPType = (VERTEXPROCESSING_TYPE)((unsigned long)Combo_GetSelectedItemData(IDC_COMBO_VERTEX_PROCESSING));
		GetSettings()->m_VertexProcessingType = VPType;
	}

	void CD3DSettingsDlg::PresentIntervalChanged()
	{
		unsigned long Interval = (ULONG)Combo_GetSelectedItemData(IDC_COMBO_PRESENT_INTERVAL);
		GetSettings()->m_ulPresentInterval = Interval;
	}

	void CD3DSettingsDlg::Combo_AddItem(unsigned long a_ulComboID, LPVOID a_pData, LPCTSTR a_pStrDesc)
	{
		HWND hwndCtrl = GetDlgItem(m_hWndDlg, a_ulComboID);
		DWORD dwItem = ComboBox_AddString(hwndCtrl, a_pStrDesc);
		ComboBox_SetItemData(hwndCtrl, dwItem, a_pData);
	}

	LPVOID CD3DSettingsDlg::Combo_GetSelectedItemData(unsigned long ComboID)
	{
		HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);
		int iItem = ComboBox_GetCurSel(hwndCtrl);
		if (iItem < 0) return NULL;
		return (LPVOID)ComboBox_GetItemData(hwndCtrl, iItem);
	}

	ULONG CD3DSettingsDlg::Combo_GetItemCount(unsigned long ComboID)
	{
		HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);
		return ComboBox_GetCount(hwndCtrl);
	}

	void CD3DSettingsDlg::Combo_SelectItem(unsigned long ComboID, unsigned long Index)
	{
		HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);
		ComboBox_SetCurSel(hwndCtrl, Index);
		PostMessage(m_hWndDlg, WM_COMMAND, MAKEWPARAM(ComboID, CBN_SELCHANGE), (LPARAM)hwndCtrl);
	}

	void CD3DSettingsDlg::Combo_SelectItemData(unsigned long ComboID, LPVOID pData)
	{
		HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);
		unsigned long Count = ComboBox_GetCount(hwndCtrl);
		for (unsigned long i = 0; i < Count; i++)
		{
			if ((LPVOID)ComboBox_GetItemData(hwndCtrl, i) = = pData)
			{
				ComboBox_SetCurSel(hwndCtrl, i);
				PostMessage(m_hWndDlg, WM_COMMAND, MAKEWPARAM(ComboID, CBN_SELCHANGE), (LPARAM)hwndCtrl);
				return;
			}
		}
	}

	void CD3DSettingsDlg::Combo_Clear(unsigned long ComboID)
	{
		HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);
		ComboBox_ResetContent(hwndCtrl);
	}
	
	bool CD3DSettingsDlg::Combo_FindText(unsigned long ComboID, LPCTSTR pStrText)
	{
		TCHAR strItem[255];
		HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);
		unsigned long Count = ComboBox_GetCount(hwndCtrl);
		for (unsigned long i = 0; i < Count; i++)
		{
			if(ComboBox_GetLBTextLen(hwndCtrl, i ) > = 255) continue;
			ComboBox_GetLBText(hwndCtrl, i, strItem);
			if(lstrcmp(strItem, pStrText) = = 0) return true;
		}
		return false;
	}

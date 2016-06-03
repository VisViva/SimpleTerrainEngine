#include "..\\Includes\\CD3DInitialize.h"

const unsigned long ValidAdapterFormatCount = 3;
const unsigned long BackBufferFormatCount = 11;
const unsigned long DeviceTypeCount = 3;
const unsigned long MultiSampleTypeCount = 17;
const unsigned long DepthStencilFormatCount = 6;
const unsigned long PresentIntervalCount = 6;
const D3DFORMAT ValidAdapterFormats[3] = {D3DFMT_X8R8G8B8, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5};
const D3DDEVTYPE DeviceTypes[3] = {D3DDEVTYPE_HAL, D3DDEVTYPE_SW, D3DDEVTYPE_REF};
const D3DFORMAT BackBufferFormats[11] = {D3DFMT_R8G8B8, D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8, D3DFMT_R5G6B5, D3DFMT_A1R5G5B5, D3DFMT_X1R5G5B5, D3DFMT_R3G3B2, D3DFMT_A8R3G3B2, D3DFMT_X4R4G4B4, D3DFMT_A4R4G4B4, D3DFMT_A2B10G10R10};
const D3DMULTISAMPLE_TYPE MultiSampleTypes[17] = {D3DMULTISAMPLE_NONE, D3DMULTISAMPLE_NONMASKABLE, D3DMULTISAMPLE_2_SAMPLES, D3DMULTISAMPLE_3_SAMPLES, D3DMULTISAMPLE_4_SAMPLES, D3DMULTISAMPLE_5_SAMPLES, D3DMULTISAMPLE_6_SAMPLES, D3DMULTISAMPLE_7_SAMPLES, D3DMULTISAMPLE_8_SAMPLES, D3DMULTISAMPLE_9_SAMPLES, D3DMULTISAMPLE_10_SAMPLES, D3DMULTISAMPLE_11_SAMPLES, D3DMULTISAMPLE_12_SAMPLES, D3DMULTISAMPLE_13_SAMPLES, D3DMULTISAMPLE_14_SAMPLES, D3DMULTISAMPLE_15_SAMPLES, D3DMULTISAMPLE_16_SAMPLES};
const D3DFORMAT DepthStencilFormats[6] = {D3DFMT_D32, D3DFMT_D24X4S4, D3DFMT_D24X8, D3DFMT_D24S8, D3DFMT_D16, D3DFMT_D15S1};
const unsigned long PresentIntervals[6] = {D3DPRESENT_INTERVAL_IMMEDIATE, D3DPRESENT_INTERVAL_DEFAULT, D3DPRESENT_INTERVAL_ONE, D3DPRESENT_INTERVAL_TWO, D3DPRESENT_INTERVAL_THREE, D3DPRESENT_INTERVAL_FOUR};

CD3DEnumAdapter::~CD3DEnumAdapter()
{
	for (unsigned long i = 0; i < m_vDevices.size(); i++)
	{
		if ( m_vDevices[i] ) delete m_vDevices[i];
	}
	m_vModes.clear();
	m_vDevices.clear();
}

CD3DEnumDevice::~CD3DEnumDevice()
{
	for (unsigned long i = 0; i < m_vOptions.size(); i++)
	{
		if (m_vOptions[i]) delete m_vOptions[i];
	}
	m_vOptions.clear();
}

CD3DEnumDeviceOptions::~CD3DEnumDeviceOptions()
{
	m_vDepthFormats.clear();
	m_vMultiSampleTypes.clear();
	m_vPresentIntervals.clear();
	m_vVertexProcessingTypes.clear();
	m_vDepthFormats.clear();
}

CD3DInitialize::CD3DInitialize()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
}

CD3DInitialize::~CD3DInitialize()
{
	if (m_pD3D) m_pD3D->Release();
	m_pD3D = NULL;
	if (m_pD3DDevice) m_pD3DDevice->Release();
	m_pD3DDevice = NULL;
	for (unsigned long i = 0; i < m_vpAdapters.size(); i++)
	{
		if (m_vpAdapters[i]) delete m_vpAdapters[i];
	}
	m_vpAdapters.clear();
}

HRESULT CD3DInitialize::Enumerate(LPDIRECT3D9 pD3D)
{
	HRESULT hRet;
	m_pD3D = pD3D;
	if (!m_pD3D) return E_FAIL;
	m_pD3D->AddRef();
	if (FAILED(hRet = EnumerateAdapters())) return hRet;
	return S_OK;
}

HRESULT CD3DInitialize::CreateDisplay(CD3DSettings& D3DSettings, unsigned long Flags, HWND hWnd, WNDPROC pWndProc, LPCTSTR Title, unsigned long Width, unsigned long Height, LPVOID lParam )
{
	unsigned long CreateFlags = 0;
	CD3DSettings::Settings *pSettings = D3DSettings.GetSettings();
	if (!hWnd)
	{
		WNDCLASS			wc;
		wc.style = CS_BYTEALIGNCLIENT | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = pWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = (HINSTANCE)GetModuleHandle(NULL);
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = Title;
		RegisterClass(&wc);
		unsigned long ulLeft = (::GetSystemMetrics(SM_CXSCREEN) - Width) / 2;
		unsigned long ulTop = (::GetSystemMetrics(SM_CYSCREEN) - Height) / 2;
		unsigned long ulStyle = WS_OVERLAPPEDWINDOW;
		if (!D3DSettings.Windowed)
		{
			ulLeft = 0;
			ulTop = 0;
			Width = pSettings->m_d3dDisplayMode.Width;
			Height = pSettings->m_d3dDisplayMode.Height;
			ulStyle = WS_VISIBLE | WS_POPUP;
		}
		m_hWnd = CreateWindow(Title, Title, ulStyle, ulLeft, ulTop, Width, Height, NULL, NULL, wc.hInstance, lParam);
		if (!m_hWnd) return E_FAIL;
	}
	else
	{
		m_hWnd = hWnd;
		if (!D3DSettings.Windowed)
		{
			SetMenu(m_hWnd, NULL);
			SetWindowLong(m_hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
			SetWindowPos(m_hWnd, NULL, 0, 0, pSettings->m_d3dDisplayMode.Width, pSettings->m_d3dDisplayMode.Height, SWP_NOZORDER);
		}
		else
		{
			RECT rc;
			GetWindowRect(hWnd, &rc);
			SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(hWnd, HWND_NOTOPMOST, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), SWP_NOACTIVATE | SWP_SHOWWINDOW);
		}
	}
	D3DPRESENT_PARAMETERS d3dpp = BuildPresentParameters(D3DSettings);
	if (pSettings->m_VertexProcessingType = = PURE_HARDWARE_VP) CreateFlags | = D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else if (pSettings->m_VertexProcessingType = = HARDWARE_VP) CreateFlags | = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else if (pSettings->m_VertexProcessingType = = MIXED_VP) CreateFlags | = D3DCREATE_MIXED_VERTEXPROCESSING;
	else if (pSettings->m_VertexProcessingType = = SOFTWARE_VP) CreateFlags | = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	m_pD3DDevice = NULL;
	HRESULT hRet = m_pD3D->CreateDevice(pSettings->m_ulAdapterOrdinal, pSettings->m_d3dDeviceType, m_hWnd, CreateFlags, &d3dpp, &m_pD3DDevice);
	if (FAILED(hRet))
	{
		if (m_pD3DDevice) m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
		return hRet;
	}
	return S_OK;
}

HRESULT CD3DInitialize::ResetDisplay(LPDIRECT3DDEVICE9 pD3DDevice, CD3DSettings& D3DSettings, HWND hWnd)
{
	D3DPRESENT_PARAMETERS d3dpp = BuildPresentParameters(D3DSettings);
	CD3DSettings::Settings *pSettings = D3DSettings.GetSettings();
	if (hWnd)
	{
		if (!D3DSettings.Windowed)
		{
			SetMenu(hWnd, NULL);
			SetWindowLong(hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
			SetWindowPos(hWnd, NULL, 0, 0, pSettings->m_d3dDisplayMode.Width, pSettings->m_d3dDisplayMode.Height, SWP_NOZORDER);
		}
		else
		{
			SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(hWnd, HWND_NOTOPMOST, -640, -480, 640, 480, SWP_NOACTIVATE | SWP_SHOWWINDOW);
			SetWindowPos(hWnd, HWND_NOTOPMOST, (::GetSystemMetrics(SM_CXSCREEN) - 640) / 2, (::GetSystemMetrics(SM_CYSCREEN) - 480) / 2, 640, 480, SWP_NOACTIVATE | SWP_SHOWWINDOW);
		}
	}
	HRESULT hRet = pD3DDevice->Reset(&d3dpp);
	if (FAILED(hRet)) return hRet;
	return S_OK;
}

HRESULT CD3DInitialize::EnumerateAdapters()
{
	HRESULT hRet;
	unsigned long nAdapterCount = m_pD3D->GetAdapterCount();
	for (unsigned long i = 0; i < nAdapterCount; i++)
	{
		CD3DEnumAdapter * pAdapter = new CD3DEnumAdapter;
		if (!pAdapter) return E_OUTOFMEMORY;
		pAdapter->m_ulOrdinal = i;
		m_pD3D->GetAdapterIdentifier(i, 0, &pAdapter->m_d3dIdentifier);
		if (FAILED(hRet = EnumerateDisplayModes(pAdapter)) || FAILED(hRet = EnumerateDevices(pAdapter)))
		{
			delete pAdapter;
			if (hRet = = E_ABORT) continue; else return hRet;
		}
		try {m_vpAdapters.push_back(pAdapter);} catch (...)
		{
			delete pAdapter;
			return E_OUTOFMEMORY;
		}
	}
	return S_OK;
}

HRESULT CD3DInitialize::EnumerateDisplayModes(CD3DEnumAdapter *pAdapter)
{
	HRESULT hRet;
	ULONG i, j;
	D3DDISPLAYMODE Mode;
	for (i = 0; i < ValidAdapterFormatCount; i++)
	{
		ULONG nModeCount = m_pD3D->GetAdapterModeCount(pAdapter->m_ulOrdinal, ValidAdapterFormats[i]);
		if (nModeCount = = 0) continue;
		for (j = 0; j < nModeCount; j++)
		{
			hRet = m_pD3D->EnumAdapterModes(pAdapter->m_ulOrdinal, ValidAdapterFormats[i], j, &Mode);
			if (FAILED(hRet)) return hRet;
			if (!ValidateDisplayMode(Mode)) continue;
			try {pAdapter->m_vModes.push_back(Mode);} catch (...)
			{
				return E_OUTOFMEMORY;
			}
		}
	}
	return (pAdapter->m_vModes.size() = = 0) ? E_ABORT : S_OK;
}

HRESULT CD3DInitialize::EnumerateDevices(CD3DEnumAdapter *pAdapter)
{
	unsigned long i;
	HRESULT hRet;
	D3DCAPS9 Caps;
	for (i = 0; i < DeviceTypeCount; i++)
	{
		if (FAILED(m_pD3D->GetDeviceCaps(pAdapter->m_ulOrdinal, DeviceTypes[i], &Caps))) continue;
		if (!ValidateDevice(DeviceTypes[i], Caps)) continue;
		CD3DEnumDevice *pDevice = new CD3DEnumDevice;
		if (!pDevice) return E_OUTOFMEMORY;
		pDevice->m_d3dDeviceType = DeviceTypes[i];
		pDevice->m_d3dCaps = Caps;
		if (FAILED(hRet = EnumerateDeviceOptions(pDevice, pAdapter)))
		{
			delete pDevice;
			if (hRet = = E_ABORT) continue; else return hRet;
		}
		try { pAdapter->m_vDevices.push_back(pDevice);} catch (...)
		{
			delete pDevice;
			return E_OUTOFMEMORY;
		}
	}
	return (pAdapter->m_vDevices.size() = = 0) ? E_ABORT : S_OK;
}

HRESULT CD3DInitialize::EnumerateDeviceOptions(CD3DEnumDevice *pDevice, CD3DEnumAdapter *pAdapter)
{
	HRESULT hRet;
	unsigned long	i, j, k;
	bool Windowed;
	D3DFORMAT AdapterFormats[ValidAdapterFormatCount];
	unsigned long	AdapterFormatCount = 0;
	D3DFORMAT AdapterFormat, BackBufferFormat;
	for (i = 0; i < pAdapter->m_vModes.size(); i++)
	{
		for (j = 0; j < AdapterFormatCount; j++) if (pAdapter->m_vModes[i].Format = = AdapterFormats[j]) break;
		if (j = = AdapterFormatCount) AdapterFormats[AdapterFormatCount++] = pAdapter->m_vModes[i].Format;
	}
	for (i = 0; i < AdapterFormatCount; i++)
	{
		AdapterFormat = AdapterFormats[i];
		for (j = 0; j < BackBufferFormatCount; j++)
		{
			BackBufferFormat = BackBufferFormats[j];
			for (k = 0; k < 2; k++)
			{
				if (k = = 0) Windowed = false; else Windowed = true;
				if (FAILED(m_pD3D->CheckDeviceType(pAdapter->m_ulOrdinal, pDevice->m_d3dDeviceType, AdapterFormat, BackBufferFormat, Windowed))) continue;
				CD3DEnumDeviceOptions * pDeviceOptions = new CD3DEnumDeviceOptions;
				if (!pDeviceOptions) return E_OUTOFMEMORY;
				pDeviceOptions->m_ulAdapterOrdinal = pAdapter->m_ulOrdinal;
				pDeviceOptions->m_d3dDeviceType = pDevice->m_d3dDeviceType;
				pDeviceOptions->m_d3dAdapterFormat = AdapterFormat;
				pDeviceOptions->m_d3dBackBufferFormat = BackBufferFormat;
				pDeviceOptions->m_d3dCaps = pDevice->m_d3dCaps;
				pDeviceOptions->m_bWindowed = Windowed;
				if (!ValidateDeviceOptions(BackBufferFormat, Windowed))
				{
					delete pDeviceOptions;
					continue;
				}
				if (FAILED(hRet = EnumerateDepthStencilFormats(pDeviceOptions)) ||
				FAILED(hRet = EnumerateMultiSampleTypes(pDeviceOptions)) ||
				FAILED(hRet = EnumerateVertexProcessingTypes(pDeviceOptions)) ||
				FAILED(hRet = EnumeratePresentIntervals(pDeviceOptions)))
				{
					delete pDeviceOptions;
					if (hRet = = E_ABORT) continue; else return hRet;
				}
				try {pDevice->m_vOptions.push_back(pDeviceOptions);} catch (...)
				{
					delete pDeviceOptions;
					return E_OUTOFMEMORY;
				}
			}
		}
	}
	return (pDevice->m_vOptions.size() = = 0) ? E_ABORT : S_OK;
}

HRESULT CD3DInitialize::EnumerateDepthStencilFormats(CD3DEnumDeviceOptions *pDeviceOptions)
{
	unsigned long i;
	try
	{
		for (i = 0; i < DepthStencilFormatCount; i++)
		{
			if (SUCCEEDED(m_pD3D->CheckDeviceFormat(pDeviceOptions->m_ulAdapterOrdinal, pDeviceOptions->m_d3dDeviceType, pDeviceOptions->m_d3dAdapterFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, DepthStencilFormats[i])))
			{
				if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(pDeviceOptions->m_ulAdapterOrdinal, pDeviceOptions->m_d3dDeviceType, pDeviceOptions->m_d3dAdapterFormat, pDeviceOptions->m_d3dBackBufferFormat, DepthStencilFormats[i])))
				{
					if (ValidateDepthStencilFormat(DepthStencilFormats[i]))
					{
						pDeviceOptions->m_vDepthFormats.push_back(DepthStencilFormats[i]);
					}
				}
			}
		}
	}
	catch (...){return E_OUTOFMEMORY;}
	return (pDeviceOptions->m_vDepthFormats.size() = = 0) ? E_ABORT : S_OK;
}

HRESULT CD3DInitialize::EnumerateMultiSampleTypes(CD3DEnumDeviceOptions *pDeviceOptions)
{
	unsigned long i, Quality;
	try
	{
		for (i = 0; i < MultiSampleTypeCount; i++)
		{
			if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(pDeviceOptions->m_ulAdapterOrdinal, pDeviceOptions->m_d3dDeviceType, pDeviceOptions->m_d3dBackBufferFormat, pDeviceOptions->m_bWindowed, MultiSampleTypes[i], &Quality)))
			{
				if (ValidateMultiSampleType(MultiSampleTypes[i]))
				{
					pDeviceOptions->m_vMultiSampleTypes.push_back(MultiSampleTypes[i]);
					pDeviceOptions->m_vMultiSampleQuality.push_back(Quality);
				}
			}
		}
	}
	catch (...) {return E_OUTOFMEMORY;}
	return (pDeviceOptions->m_vMultiSampleTypes.size() = = 0) ? E_ABORT : S_OK;
}

HRESULT CD3DInitialize::EnumerateVertexProcessingTypes(CD3DEnumDeviceOptions *pDeviceOptions)
{
	try
	{
		if (pDeviceOptions->m_d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		{
			if (pDeviceOptions->m_d3dCaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
			{
				if (ValidateVertexProcessingType(PURE_HARDWARE_VP)) pDeviceOptions->m_vVertexProcessingTypes.push_back(PURE_HARDWARE_VP);
			}
			if (ValidateVertexProcessingType(HARDWARE_VP)) pDeviceOptions->m_vVertexProcessingTypes.push_back(HARDWARE_VP);
			if (ValidateVertexProcessingType(MIXED_VP)) pDeviceOptions->m_vVertexProcessingTypes.push_back(MIXED_VP);
		}
		if (ValidateVertexProcessingType(SOFTWARE_VP)) pDeviceOptions->m_vVertexProcessingTypes.push_back(SOFTWARE_VP);
	}
	catch (...) {return E_OUTOFMEMORY;}
	return (pDeviceOptions->m_vVertexProcessingTypes.size() = = 0) ? E_ABORT : S_OK;
}

HRESULT CD3DInitialize::EnumeratePresentIntervals(CD3DEnumDeviceOptions *pDeviceOptions)
{
	unsigned long i, Interval;
	try
	{
		for (i = 0; i < PresentIntervalCount; i++)
		{
			Interval = PresentIntervals[i];
			if (pDeviceOptions->m_bWindowed)
			{
				if (Interval = = D3DPRESENT_INTERVAL_TWO || Interval = = D3DPRESENT_INTERVAL_THREE || Interval = = D3DPRESENT_INTERVAL_FOUR) continue;
			}
			if (Interval = = D3DPRESENT_INTERVAL_DEFAULT)
			{
				pDeviceOptions->m_vPresentIntervals.push_back(Interval);
				continue;
			}
			if (pDeviceOptions->m_d3dCaps.PresentationIntervals & Interval)
			{
				if (ValidatePresentInterval(Interval)) pDeviceOptions->m_vPresentIntervals.push_back(Interval);
			}
		}
	}
	catch (...) {return E_OUTOFMEMORY;}
	return (pDeviceOptions->m_vPresentIntervals.size() = = 0) ? E_ABORT : S_OK;
}

D3DPRESENT_PARAMETERS CD3DInitialize::BuildPresentParameters(CD3DSettings& D3DSettings, unsigned long Flags)
{
	D3DPRESENT_PARAMETERS d3dpp;
	CD3DSettings::Settings *pSettings = D3DSettings.GetSettings();
	ZeroMemory (&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = pSettings->m_d3dBackBufferFormat;
	d3dpp.Windowed = D3DSettings.Windowed;
	d3dpp.MultiSampleType = pSettings->m_d3dMultiSampleType;
	d3dpp.MultiSampleQuality = pSettings->m_ulMultiSampleQuality;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = pSettings->m_d3dDepthStencilFormat;
	d3dpp.PresentationInterval = pSettings->m_ulPresentInterval;
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL | Flags;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	if (!d3dpp.Windowed)
	{
		d3dpp.FullScreen_RefreshRateInHz = pSettings->m_d3dDisplayMode.RefreshRate;
		d3dpp.BackBufferWidth = pSettings->m_d3dDisplayMode.Width;
		d3dpp.BackBufferHeight = pSettings->m_d3dDisplayMode.Height;
	}
	return d3dpp;
}

bool CD3DInitialize::FindBestWindowedMode(CD3DSettings &D3DSettings, bool bRequireHAL, bool bRequireREF)
{
	ULONG					i, j, k;
	D3DDISPLAYMODE			DisplayMode;
	CD3DEnumAdapter			*pBestAdapter = NULL;
	CD3DEnumDevice			*pBestDevice = NULL;
	CD3DEnumDeviceOptions	*pBestOptions = NULL;
	CD3DSettings::Settings	*pSettings = NULL;
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode);
	for(i = 0; i < GetAdapterCount(); i++)
	{
		CD3DEnumAdapter *pAdapter = m_vpAdapters[i];
		for(j = 0; j < pAdapter->m_vDevices.size(); j++)
		{
			CD3DEnumDevice * pDevice = pAdapter->m_vDevices[j];
			if (bRequireHAL && pDevice->m_d3dDeviceType ! = D3DDEVTYPE_HAL) continue;
			if (bRequireREF && pDevice->m_d3dDeviceType ! = D3DDEVTYPE_REF) continue;
			for (k = 0; k < pDevice->m_vOptions.size(); k++)
			{
				CD3DEnumDeviceOptions *pOptions = pDevice->m_vOptions[k];
				bool MatchedBB = (pOptions->m_d3dBackBufferFormat = = pOptions->m_d3dAdapterFormat );
				if (!pOptions->m_bWindowed) continue;
				if ( pOptions->m_d3dAdapterFormat ! = DisplayMode.Format) continue;
				if(pBestOptions = = NULL || (pOptions->m_d3dDeviceType = = D3DDEVTYPE_HAL && MatchedBB) || (pBestOptions->m_d3dDeviceType ! = D3DDEVTYPE_HAL && pOptions->m_d3dDeviceType = = D3DDEVTYPE_HAL))
				{
					pBestAdapter = pAdapter;
					pBestDevice = pDevice;
					pBestOptions = pOptions;
					if (pOptions->m_d3dDeviceType = = D3DDEVTYPE_HAL && MatchedBB)
					{
						goto EndWindowedDeviceOptionSearch;
					}
				}
			}
		}
	}
	EndWindowedDeviceOptionSearch:
	if (pBestOptions = = NULL) return false;
	D3DSettings.Windowed = true;
	pSettings = D3DSettings.GetSettings();
	pSettings->m_ulAdapterOrdinal = pBestOptions->m_ulAdapterOrdinal;
	pSettings->m_d3dDisplayMode = DisplayMode;
	pSettings->m_d3dDeviceType = pBestOptions->m_d3dDeviceType;
	pSettings->m_d3dBackBufferFormat = pBestOptions->m_d3dBackBufferFormat;
	pSettings->m_d3dDepthStencilFormat = pBestOptions->m_vDepthFormats[0];
	pSettings->m_d3dMultiSampleType = pBestOptions->m_vMultiSampleTypes[0];
	pSettings->m_ulMultiSampleQuality = 0;
	pSettings->m_VertexProcessingType = pBestOptions->m_vVertexProcessingTypes[0];
	pSettings->m_ulPresentInterval = pBestOptions->m_vPresentIntervals[0];
	return true;
}

bool CD3DInitialize::FindBestFullscreenMode(CD3DSettings & D3DSettings, D3DDISPLAYMODE * pMatchMode, bool bRequireHAL, bool bRequireREF )
{
	unsigned long	i, j, k;
	D3DDISPLAYMODE AdapterDisplayMode;
	D3DDISPLAYMODE BestAdapterDisplayMode;
	D3DDISPLAYMODE BestDisplayMode;
	CD3DEnumAdapter *pBestAdapter = NULL;
	CD3DEnumDevice *pBestDevice = NULL;
	CD3DEnumDeviceOptions	*pBestOptions = NULL;
	CD3DSettings::Settings *pSettings = NULL;
	BestAdapterDisplayMode.Width = 0;
	BestAdapterDisplayMode.Height = 0;
	BestAdapterDisplayMode.Format = D3DFMT_UNKNOWN;
	BestAdapterDisplayMode.RefreshRate = 0;
	for(i = 0; i < GetAdapterCount(); i++)
	{
		CD3DEnumAdapter * pAdapter = m_vpAdapters[i];
		m_pD3D->GetAdapterDisplayMode(pAdapter->m_ulOrdinal, &AdapterDisplayMode);
		if (pMatchMode)
		{
			if (pMatchMode->Width  ! = 0) AdapterDisplayMode.Width = pMatchMode->Width;
			if (pMatchMode->Height ! = 0) AdapterDisplayMode.Height = pMatchMode->Height;
			if (pMatchMode->Format ! = D3DFMT_UNKNOWN) AdapterDisplayMode.Format = pMatchMode->Format;
			if (pMatchMode->RefreshRate ! = 0) AdapterDisplayMode.RefreshRate = pMatchMode->RefreshRate;
		}
		for(j = 0; j < pAdapter->m_vDevices.size(); j++)
		{
			CD3DEnumDevice * pDevice = pAdapter->m_vDevices[j];
			if (bRequireHAL && pDevice->m_d3dDeviceType ! = D3DDEVTYPE_HAL) continue;
			if (bRequireREF && pDevice->m_d3dDeviceType ! = D3DDEVTYPE_REF) continue;
			for (k = 0; k < pDevice->m_vOptions.size(); k++)
			{
				CD3DEnumDeviceOptions *pOptions = pDevice->m_vOptions[k];
				bool MatchedBB = (pOptions->m_d3dBackBufferFormat = = pOptions->m_d3dAdapterFormat);
				bool MatchedDesktop = (pOptions->m_d3dAdapterFormat = = AdapterDisplayMode.Format);
				if (pOptions->m_bWindowed) continue;
				if (pBestOptions = = NULL || (pBestOptions->m_d3dDeviceType ! = D3DDEVTYPE_HAL && pDevice->m_d3dDeviceType = = D3DDEVTYPE_HAL) || (pOptions->m_d3dDeviceType = = D3DDEVTYPE_HAL && pBestOptions->m_d3dAdapterFormat ! = AdapterDisplayMode.Format && MatchedDesktop) || (pOptions->m_d3dDeviceType = = D3DDEVTYPE_HAL && MatchedDesktop && MatchedBB))
				{
					BestAdapterDisplayMode = AdapterDisplayMode;
					pBestAdapter = pAdapter;
					pBestDevice = pDevice;
					pBestOptions = pOptions;
					if (pOptions->m_d3dDeviceType = = D3DDEVTYPE_HAL && MatchedDesktop && MatchedBB)
					{
						goto EndFullscreenDeviceOptionSearch;
					}
				}
			}
		}
	}
	EndFullscreenDeviceOptionSearch:
	if (pBestOptions = = NULL) return false;
	BestDisplayMode.Width = 0;
	BestDisplayMode.Height = 0;
	BestDisplayMode.Format = D3DFMT_UNKNOWN;
	BestDisplayMode.RefreshRate = 0;
	for(i = 0; i < pBestAdapter->m_vModes.size(); i++)
	{
		D3DDISPLAYMODE Mode = pBestAdapter->m_vModes[i];
		if(Mode.Format ! = pBestOptions->m_d3dAdapterFormat) continue;
		if(Mode.Width = = BestAdapterDisplayMode.Width && Mode.Height = = BestAdapterDisplayMode.Height && Mode.RefreshRate = = BestAdapterDisplayMode.RefreshRate)
		{
			BestDisplayMode = Mode;
			break;
		}
		else if(Mode.Width = = BestAdapterDisplayMode.Width && Mode.Height = = BestAdapterDisplayMode.Height && Mode.RefreshRate > BestDisplayMode.RefreshRate)
		{
			BestDisplayMode = Mode;
		}
		else if(Mode.Width = = BestAdapterDisplayMode.Width)
		{
			BestDisplayMode = Mode;
		}
		else if(BestDisplayMode.Width = = 0)
		{
			BestDisplayMode = Mode;
		}
	}
	D3DSettings.Windowed = false;
	pSettings = D3DSettings.GetSettings();
	pSettings->m_ulAdapterOrdinal = pBestOptions->m_ulAdapterOrdinal;
	pSettings->m_d3dDisplayMode = BestDisplayMode;
	pSettings->m_d3dDeviceType = pBestOptions->m_d3dDeviceType;
	pSettings->m_d3dBackBufferFormat = pBestOptions->m_d3dBackBufferFormat;
	pSettings->m_d3dDepthStencilFormat = pBestOptions->m_vDepthFormats[0];
	pSettings->m_d3dMultiSampleType = pBestOptions->m_vMultiSampleTypes[0];
	pSettings->m_ulMultiSampleQuality = 0;
	pSettings->m_VertexProcessingType = pBestOptions->m_vVertexProcessingTypes[0];
	pSettings->m_ulPresentInterval = pBestOptions->m_vPresentIntervals[0];
	return true;
}

LPDIRECT3DDEVICE9 CD3DInitialize::GetDirect3DDevice()
{
	if (!m_pD3DDevice) return NULL;
	m_pD3DDevice->AddRef();
	return m_pD3DDevice;
}

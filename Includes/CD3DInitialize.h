#ifndef CD3DINITIALIZE_H
#define CD3DINITIALIZE_H
#include <d3dx9.h>
#include <vector>

enum VERTEXPROCESSING_TYPE
{
	SOFTWARE_VP = 1,
	MIXED_VP = 2,
	HARDWARE_VP = 3,
	PURE_HARDWARE_VP = 4
};

class CD3DEnumDeviceOptions;
class CD3DEnumDevice;
class CD3DEnumAdapter;

typedef std::vector<D3DMULTISAMPLE_TYPE> VectorMSType;
typedef std::vector<D3DFORMAT> VectorFormat;
typedef std::vector<ULONG> VectorULONG;
typedef std::vector<VERTEXPROCESSING_TYPE> VectorVPType;
typedef std::vector<D3DDISPLAYMODE> VectorDisplayMode;
typedef std::vector<CD3DEnumDeviceOptions*> VectorDeviceOptions;
typedef std::vector<CD3DEnumDevice*> VectorDevice;
typedef std::vector<CD3DEnumAdapter*> VectorAdapter;

class CD3DEnumDeviceOptions
{
public:
	~CD3DEnumDeviceOptions();
	bool			m_bWindowed;
	unsigned long	m_ulAdapterOrdinal;
	D3DDEVTYPE		m_d3dDeviceType;
	D3DCAPS9		m_d3dCaps;
	D3DFORMAT		m_d3dAdapterFormat;
	D3DFORMAT		m_d3dBackBufferFormat;
	VectorMSType	m_vMultiSampleTypes;
	VectorULONG		m_vMultiSampleQuality;
	VectorFormat	m_vDepthFormats;
	VectorVPType	m_vVertexProcessingTypes;
	VectorULONG		m_vPresentIntervals;
};
class CD3DEnumDevice
{
public:
	~CD3DEnumDevice();
	D3DDEVTYPE				m_d3dDeviceType;
	D3DCAPS9				m_d3dCaps;
	VectorDeviceOptions		m_vOptions;
};
class CD3DEnumAdapter
{
public:
	~CD3DEnumAdapter();
	unsigned long			m_ulOrdinal;
	D3DADAPTER_IDENTIFIER9	m_d3dIdentifier;
	VectorDisplayMode		m_vModes;
	VectorDevice			m_vDevices;
};
class CD3DSettings
{
public:
	struct Settings
	{
		unsigned long			m_ulAdapterOrdinal;
		D3DDISPLAYMODE			m_d3dDisplayMode;
		D3DDEVTYPE				m_d3dDeviceType;
		D3DFORMAT				m_d3dBackBufferFormat;
		D3DFORMAT				m_d3dDepthStencilFormat;
		D3DMULTISAMPLE_TYPE		m_d3dMultiSampleType;
		unsigned long			m_ulMultiSampleQuality;
		VERTEXPROCESSING_TYPE	m_VertexProcessingType;
		unsigned long			m_ulPresentInterval;
	};
	bool		Windowed;
	Settings	Windowed_Settings;
	Settings	Fullscreen_Settings;
	Settings* GetSettings(){return (Windowed) ? &Windowed_Settings : &Fullscreen_Settings;}
};
class CD3DInitialize
{
public:
	CD3DInitialize();
	virtual ~CD3DInitialize();
	HRESULT Enumerate(LPDIRECT3D9 pD3D);
	HRESULT CreateDisplay(CD3DSettings &D3DSettings, unsigned long Flags = 0, HWND hWnd = NULL, WNDPROC pWndProc = NULL, LPCTSTR Title = NULL, unsigned long Width = CW_USEDEFAULT, unsigned long Height = CW_USEDEFAULT, LPVOID lParam = NULL);
	HRESULT ResetDisplay(LPDIRECT3DDEVICE9 pD3DDevice, CD3DSettings &D3DSettings, HWND hWnd = NULL);
	bool FindBestWindowedMode (CD3DSettings & D3DSettings, bool bRequireHAL = false, bool bRequireREF = false);
	bool FindBestFullscreenMode (CD3DSettings & D3DSettings, D3DDISPLAYMODE *pMatchMode = NULL, bool bRequireHAL = false, bool bRequireREF = false);
	D3DPRESENT_PARAMETERS BuildPresentParameters (CD3DSettings& D3DSettings, ULONG Flags = 0);
	ULONG GetAdapterCount() const {return m_vpAdapters.size();}
	HWND GetHWND() {return m_hWnd;}
	LPDIRECT3DDEVICE9 GetDirect3DDevice();
	const CD3DEnumAdapter *GetAdapter(ULONG Index){return (Index > GetAdapterCount()) ? NULL : m_vpAdapters[Index];}
	const LPDIRECT3D9 GetDirect3D(){return m_pD3D;}
private:
	HRESULT EnumerateAdapters				();
	HRESULT EnumerateDisplayModes			(CD3DEnumAdapter *pAdapter);
	HRESULT EnumerateDevices				(CD3DEnumAdapter *pAdapter);
	HRESULT EnumerateDeviceOptions			(CD3DEnumDevice  *pDevice, CD3DEnumAdapter *pAdapter);
	HRESULT EnumerateDepthStencilFormats	(CD3DEnumDeviceOptions *pDeviceOptions);
	HRESULT EnumerateMultiSampleTypes		(CD3DEnumDeviceOptions *pDeviceOptions);
	HRESULT EnumerateVertexProcessingTypes	(CD3DEnumDeviceOptions *pDeviceOptions);
	HRESULT EnumeratePresentIntervals		(CD3DEnumDeviceOptions *pDeviceOptions);
	virtual bool ValidateDisplayMode			(const D3DDISPLAYMODE& Mode){return true;}
	virtual bool ValidateDevice					(const D3DDEVTYPE &Type, const D3DCAPS9 &Caps){return true;}
	virtual bool ValidateDeviceOptions			(const D3DFORMAT &BackBufferFormat, bool IsWindowed){return true;}
	virtual bool ValidateDepthStencilFormat		(const D3DFORMAT &DepthStencilFormat){return true;}
	virtual bool ValidateMultiSampleType		(const D3DMULTISAMPLE_TYPE &Type){return true;}
	virtual bool ValidateVertexProcessingType	(const VERTEXPROCESSING_TYPE &Type){return true;}
	virtual bool ValidatePresentInterval		(const unsigned long &Interval){return true;}
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3DDevice;
	HWND				m_hWnd;
	VectorAdapter		m_vpAdapters;
};
#endif

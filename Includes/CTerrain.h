#ifndef CTERRAIN_H
#define CTERRAIN_H
#include "Main.h"

class CMesh;
class CPlayer;
class CCamera;

class CTerrain
{
public:
	CTerrain();
	virtual ~CTerrain();
	void SetD3DDevice(LPDIRECT3DDEVICE9 pD3DDevice, bool HardwareTnL);
	bool LoadHeightMap(LPCTSTR FileName, ULONG Width, ULONG Height);
	float GetHeight(float x, float z, bool ReverseQuad = false);
	void Render(CCamera *pCamera);
	void Release();
	static void UpdatePlayer(LPVOID pContext, CPlayer *pPlayer, float TimeScale);
	static void UpdateCamera(LPVOID pContext, CCamera *pCamera, float TimeScale);
private:
	D3DXVECTOR3 m_vecScale;
	UCHAR *m_pHeightMap;
	unsigned long m_nHeightMapWidth;
	unsigned long m_nHeightMapHeight;
	CMesh **m_pMesh;
	unsigned long m_nMeshCount;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	bool m_bHardwareTnL;
	unsigned long m_nPrimitiveCount;
	long AddMesh(unsigned long Count = 1);
	bool BuildMeshes();
	D3DXVECTOR3 GetHeightMapNormal(unsigned long x, unsigned long z);
};
#endif

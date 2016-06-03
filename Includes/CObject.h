#ifndef COBJECT_H
#define COBJECT_H
#include "Main.h"

class CVertex
{
public:
	CVertex() {x = 0.0f; y = 0.0f; z = 0.0f; Diffuse = 0xFF000000;}
	CVertex(float fX, float fY, float fZ, ULONG ulDiffuse = 0xFF000000) {x = fX; y = fY; z = fZ; Diffuse = ulDiffuse;}
	float x;
	float y;
	float z;
	unsigned long Diffuse;
};
class CMesh
{
public:
	CMesh();
	CMesh(unsigned long VertexCount, unsigned long IndexCount);
	virtual ~CMesh();
	long AddVertex (ULONG Count = 1);
	long AddIndex (ULONG Count = 1);
	HRESULT BuildBuffers (LPDIRECT3DDEVICE9 pD3DDevice, bool HardwareTnL, bool ReleaseOriginals = true);
	unsigned long m_nVertexCount;
	CVertex *m_pVertex;
	unsigned long m_nIndexCount;
	unsigned short *m_pIndex;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
	D3DXVECTOR3 m_BoundsMin;
	D3DXVECTOR3 m_BoundsMax;
};
class CObject
{
public:
	CObject();
	CObject(CMesh *pMesh);
	D3DXMATRIX m_mtxWorld;
	CMesh *m_pMesh;
};
#endif

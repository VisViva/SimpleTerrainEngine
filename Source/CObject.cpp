#include "..\\Includes\\CObject.h"

CObject::CObject()
{
	m_pMesh = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
}

CObject::CObject(CMesh *pMesh)
{
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pMesh = pMesh;
}

CMesh::CMesh()
{
	m_pVertex = NULL;
	m_pIndex = NULL;
	m_nVertexCount = 0;
	m_nIndexCount = 0;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
}

CMesh::CMesh(unsigned long VertexCount, unsigned long IndexCount)
{
	m_pVertex = NULL;
	m_pIndex = NULL;
	m_nVertexCount = 0;
	m_nIndexCount = 0;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	if (VertexCount > 0) AddVertex(VertexCount);
	if (IndexCount > 0) AddIndex(IndexCount);
}

CMesh::~CMesh()
{
	if (m_pVertex) delete []m_pVertex;
	if (m_pIndex) delete []m_pIndex;
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pIndexBuffer) m_pIndexBuffer->Release();
	m_pVertex = NULL;
	m_pIndex = NULL;
	m_nVertexCount = 0;
	m_nIndexCount = 0;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
}

long CMesh::AddVertex(unsigned long Count)
{
	CVertex *pVertexBuffer = NULL;
	if (!(pVertexBuffer = new CVertex[m_nVertexCount + Count])) return -1;
	if (m_pVertex)
	{
		memcpy(pVertexBuffer, m_pVertex, m_nVertexCount *sizeof(CVertex));
		delete []m_pVertex;
	}
	m_pVertex = pVertexBuffer;
	m_nVertexCount + = Count;
	return m_nVertexCount - Count;
}

long CMesh::AddIndex(unsigned long Count)
{
	USHORT *pIndexBuffer = NULL;
	if (!(pIndexBuffer = new USHORT[m_nIndexCount + Count])) return -1;
	if (m_pIndex)
	{
		memcpy(pIndexBuffer, m_pIndex, m_nIndexCount * sizeof(USHORT));
		delete []m_pIndex;
	}
	m_pIndex = pIndexBuffer;
	m_nIndexCount + = Count;
	return m_nIndexCount - Count;
}

HRESULT CMesh::BuildBuffers(LPDIRECT3DDEVICE9 pD3DDevice, bool HardwareTnL, bool ReleaseOriginals)
{
	HRESULT hRet = S_OK;
	CVertex *pVertex = NULL;
	USHORT *pIndex = NULL;
	unsigned long ulUsage = D3DUSAGE_WRITEONLY;
	if (!HardwareTnL) ulUsage | = D3DUSAGE_SOFTWAREPROCESSING;
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pIndexBuffer) m_pIndexBuffer->Release();
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	hRet = pD3DDevice->CreateVertexBuffer(sizeof(CVertex) *m_nVertexCount, ulUsage, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
	if (FAILED(hRet)) return hRet;
	hRet = m_pVertexBuffer->Lock(0, sizeof(CVertex) * m_nVertexCount, (void**)&pVertex, 0);
	if (FAILED(hRet)) return hRet;
	memcpy(pVertex, m_pVertex, sizeof(CVertex) *m_nVertexCount);
	m_pVertexBuffer->Unlock();
	hRet = pD3DDevice->CreateIndexBuffer(sizeof(USHORT) *m_nIndexCount, ulUsage, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuffer, NULL);
	if (FAILED(hRet)) return hRet;
	hRet = m_pIndexBuffer->Lock(0, sizeof(USHORT) *m_nIndexCount, (void**)&pIndex, 0);
	if (FAILED(hRet)) return hRet;
	memcpy(pIndex, m_pIndex, sizeof(USHORT) *m_nIndexCount);
	m_pIndexBuffer->Unlock();
	m_BoundsMin = D3DXVECTOR3(999999.0f, 999999.0f, 999999.0f);
	m_BoundsMax = D3DXVECTOR3(-999999.0f, -999999.0f, -999999.0f);
	for (unsigned long i = 0; i < m_nVertexCount; ++i)
	{
		D3DXVECTOR3 *Pos = (D3DXVECTOR3*)&m_pVertex[i];
		if (Pos->x < m_BoundsMin.x) m_BoundsMin.x = Pos->x;
		if (Pos->y < m_BoundsMin.y) m_BoundsMin.y = Pos->y;
		if (Pos->z < m_BoundsMin.z) m_BoundsMin.z = Pos->z;
		if (Pos->x > m_BoundsMax.x) m_BoundsMax.x = Pos->x;
		if (Pos->y > m_BoundsMax.y) m_BoundsMax.y = Pos->y;
		if (Pos->z > m_BoundsMax.z) m_BoundsMax.z = Pos->z;
	}
	if (ReleaseOriginals)
	{
		if (m_pVertex) delete []m_pVertex;
		if (m_pIndex) delete []m_pIndex;
		m_pVertex = NULL;
		m_pIndex = NULL;
		m_nVertexCount = 0;
		m_nIndexCount = 0;
	}
	return S_OK;
}

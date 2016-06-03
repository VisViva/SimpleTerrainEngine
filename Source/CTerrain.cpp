#include "..\\Includes\\CTerrain.h"
#include "..\\Includes\\CObject.h"
#include "..\\Includes\\CPlayer.h"
#include "..\\Includes\\CCamera.h"

namespace
{
	const USHORT BlockWidth = 17;
	const USHORT BlockHeight = 17;
	const USHORT QuadsWide = BlockWidth - 1;
	const USHORT QuadsHigh = BlockHeight - 1;
};

CTerrain::CTerrain()
{
	m_pD3DDevice = NULL;
	m_pHeightMap = NULL;
	m_nHeightMapWidth = 0;
	m_nHeightMapHeight = 0;
	m_pMesh = NULL;
	m_nMeshCount = 0;
	m_vecScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

CTerrain::~CTerrain()
{
	Release();
}

void CTerrain::Release()
{
	if (m_pHeightMap) delete[]m_pHeightMap;
	if (m_pMesh)
	{
		for (ULONG i = 0; i < m_nMeshCount; i++)
		{
			if (m_pMesh[i]) delete m_pMesh[i];
		}
		delete []m_pMesh;
	}
	m_pHeightMap = NULL;
	m_nHeightMapWidth = 0;
	m_nHeightMapHeight = 0;
	m_pMesh = NULL;
	m_nMeshCount = 0;
	m_pD3DDevice = NULL;
}

bool CTerrain::LoadHeightMap(LPCTSTR FileName, ULONG Width, ULONG Height)
{
	FILE *pFile = NULL;
	if (m_pMesh) return false;
	if (!m_pD3DDevice) return false;
	m_nHeightMapWidth = Width;
	m_nHeightMapHeight = Height;
	m_vecScale.x = 4.0f * (512 / (m_nHeightMapWidth - 1));
	m_vecScale.y = 2.0f;
	m_vecScale.z = 4.0f * (512 / (m_nHeightMapHeight - 1));
	m_pHeightMap = new UCHAR[Width * Height];
	if (!m_pHeightMap) return false;
	pFile = _tfopen(FileName, _T("rb"));
	if (!pFile) return false;
	fread(m_pHeightMap, Width * Height, 1, pFile);
	fclose(pFile);
	if (AddMesh(((Width - 1) / QuadsWide) * ((Height - 1) / QuadsHigh)) < 0) return false;
	return BuildMeshes();
}

D3DXVECTOR3 CTerrain::GetHeightMapNormal(ULONG x, ULONG z)
{
	D3DXVECTOR3 Normal, Edge1, Edge2;
	ULONG HMIndex, HMAddX, HMAddZ;
	float y1, y2, y3;
	if (x < 0.0f || z < 0.0f || x > = m_nHeightMapWidth || z > = m_nHeightMapHeight) return D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	HMIndex = x + z * m_nHeightMapWidth;
	if (x < (m_nHeightMapWidth - 1)) HMAddX = 1; else HMAddX = -1;
	if (z < (m_nHeightMapHeight - 1)) HMAddZ = m_nHeightMapWidth; else HMAddZ = -(signed)m_nHeightMapWidth;
	y1 = (float)m_pHeightMap[HMIndex] * m_vecScale.y;
	y2 = (float)m_pHeightMap[HMIndex + HMAddX] * m_vecScale.y;
	y3 = (float)m_pHeightMap[HMIndex + HMAddZ] * m_vecScale.y;
	Edge1 = D3DXVECTOR3(0.0f, y3 - y1, m_vecScale.z);
	Edge2 = D3DXVECTOR3(m_vecScale.x, y2 - y1, 0.0f);
	D3DXVec3Cross(&Normal, &Edge1, &Edge2);
	D3DXVec3Normalize(&Normal, &Normal);
	return Normal;
}

float CTerrain::GetHeight(float x, float z, bool ReverseQuad)
{
	float fTopLeft, fTopRight, fBottomLeft, fBottomRight;
	x = x / m_vecScale.x;
	z = z / m_vecScale.z;
	if (x < 0.0f || z < 0.0f || x > = m_nHeightMapWidth || z > = m_nHeightMapHeight) return 0.0f;
	int ix = (int)x;
	int iz = (int)z;
	float fPercentX = x - ((float)ix);
	float fPercentZ = z - ((float)iz);
	if (ReverseQuad)
	{
		fTopLeft = (float)m_pHeightMap[ix + iz * m_nHeightMapWidth] * m_vecScale.y;
		fBottomRight = (float)m_pHeightMap[(ix + 1) + (iz + 1) * m_nHeightMapWidth] * m_vecScale.y;
		if (fPercentX < fPercentZ)
		{
			fBottomLeft = (float)m_pHeightMap[ix + (iz + 1) * m_nHeightMapWidth] * m_vecScale.y;
			fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
		}
		else
		{
			fTopRight = (float)m_pHeightMap[(ix + 1) + iz * m_nHeightMapWidth] * m_vecScale.y;
			fBottomLeft = fTopLeft + (fBottomRight - fTopRight);
		}
	}
	else
	{
		fTopRight = (float)m_pHeightMap[(ix + 1) + iz * m_nHeightMapWidth] * m_vecScale.y;
		fBottomLeft = (float)m_pHeightMap[ix + (iz + 1) * m_nHeightMapWidth] * m_vecScale.y;
		if (fPercentX < (1.0f - fPercentZ))
		{
			fTopLeft = (float)m_pHeightMap[ix + iz * m_nHeightMapWidth] * m_vecScale.y;
			fBottomRight = fBottomLeft + (fTopRight - fTopLeft);
		}
		else
		{
			fBottomRight = (float)m_pHeightMap[(ix + 1) + (iz + 1) * m_nHeightMapWidth] * m_vecScale.y;
			fTopLeft = fTopRight + (fBottomLeft - fBottomRight);
		}
	}
	float fTopHeight = fTopLeft + ((fTopRight - fTopLeft) * fPercentX);
	float fBottomHeight = fBottomLeft + ((fBottomRight - fBottomLeft) * fPercentX);
	return fTopHeight + ((fBottomHeight - fTopHeight) * fPercentZ);
}

bool CTerrain::BuildMeshes()
{
	long x, z, vx, vz, Counter, StartX, StartZ;
	long BlocksWide = (m_nHeightMapWidth  - 1) / QuadsWide;
	long BlocksHigh = (m_nHeightMapHeight - 1) / QuadsHigh;
	D3DXVECTOR3 VertexPos, LightDir = D3DXVECTOR3(0.650945f, -0.390567f, 0.650945f);
	ULONG IndexCount = ((BlockWidth * 2) * QuadsHigh) + (QuadsHigh - 1);
	m_nPrimitiveCount = ((QuadsWide * QuadsHigh) * 2) + ((QuadsHigh - 1) * 3);
	for (z = 0; z < BlocksHigh; z++)
	{
		for (x = 0; x < BlocksWide; x++)
		{
			CMesh *pMesh = m_pMesh[x + z * BlocksWide];
			if (pMesh->AddVertex(BlockWidth * BlockHeight) < 0) return false;
			if (pMesh->AddIndex(IndexCount) < 0) return false;
			Counter = 0;
			StartX = x * (BlockWidth - 1);
			StartZ = z * (BlockHeight - 1);
			for (vz = StartZ; vz < StartZ + BlockHeight; vz++)
			{
				for (vx = StartX; vx < StartX + BlockWidth; vx++)
				{
					pMesh->m_pVertex[Counter].x = (float)vx * m_vecScale.x;
					pMesh->m_pVertex[Counter].y = (float)m_pHeightMap[vx + vz * m_nHeightMapWidth] * m_vecScale.y;
					pMesh->m_pVertex[Counter].z = (float)vz * m_vecScale.z;
					Counter++;
				}
			}
			Counter = 0;
			for (vz = StartZ; vz < StartZ + BlockHeight; vz++)
			{
				for (vx = StartX; vx < StartX + BlockWidth; vx++)
				{
					VertexPos = (D3DXVECTOR3&)pMesh->m_pVertex[Counter];
					float fRed = 1.0f, fGreen = 0.8f, fBlue = 0.6f, fScale = 0.25f;
					fScale = D3DXVec3Dot(&GetHeightMapNormal(vx, vz), &(-LightDir));
					fScale + = D3DXVec3Dot(&GetHeightMapNormal(vx + 1, vz), &(-LightDir));
					fScale + = D3DXVec3Dot(&GetHeightMapNormal(vx + 1, vz + 1), &(-LightDir));
					fScale + = D3DXVec3Dot(&GetHeightMapNormal(vx, vz + 1), &(-LightDir));
					fScale / = 4.0f;
					fScale + = 0.05f;
					if (fScale > 1.0f) fScale = 1.0f;
					if (fScale < 0.25f) fScale = 0.25f;
					pMesh->m_pVertex[Counter].Diffuse = D3DCOLOR_COLORVALUE(fRed * fScale, fGreen * fScale, fBlue * fScale, 1.0f);
					Counter++;
				}
			}
			Counter = 0;
			for (vz = 0; vz < BlockHeight - 1; vz++)
			{
				if ((vz % 2) = = 0)
				{
					for (vx = 0; vx < BlockWidth; vx++)
					{
						if (vx = = 0 && vz > 0) pMesh->m_pIndex[Counter++] = (USHORT)(vx + vz * BlockWidth);
						pMesh->m_pIndex[Counter++] = (USHORT)(vx + vz * BlockWidth);
						pMesh->m_pIndex[Counter++] = (USHORT)((vx + vz * BlockWidth) + BlockWidth);
					}
				}
				else
				{
					for (vx = BlockWidth - 1; vx > = 0; vx--)
					{
						if (vx = = (BlockWidth - 1)) pMesh->m_pIndex[Counter++] = (USHORT)(vx + vz * BlockWidth);
						pMesh->m_pIndex[Counter++] = (USHORT)(vx + vz * BlockWidth);
						pMesh->m_pIndex[Counter++] = (USHORT)((vx + vz * BlockWidth) + BlockWidth);
					}
				}
			}
			if (FAILED(pMesh->BuildBuffers(m_pD3DDevice, m_bHardwareTnL))) return false;
		}
	}
	return true;
}

void CTerrain::Render(CCamera *pCamera)
{
	if(!m_pD3DDevice) return;
	m_pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	for (ULONG i = 0; i < m_nMeshCount; i++)
	{
		if (pCamera && (!pCamera->BoundsInFrustum(m_pMesh[i]->m_BoundsMin, m_pMesh[i]->m_BoundsMax))) continue;
		m_pD3DDevice->SetStreamSource(0, m_pMesh[i]->m_pVertexBuffer, 0, sizeof(CVertex));
		m_pD3DDevice->SetIndices(m_pMesh[i]->m_pIndexBuffer);
		m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, BlockWidth * BlockHeight, 0, m_nPrimitiveCount);
	}
}

long CTerrain::AddMesh(ULONG Count)
{
	CMesh **pMeshBuffer = NULL;
	if (!(pMeshBuffer = new CMesh*[m_nMeshCount + Count])) return -1;
	ZeroMemory(&pMeshBuffer[m_nMeshCount], Count * sizeof(CMesh*));
	if (m_pMesh)
	{
		memcpy(pMeshBuffer, m_pMesh, m_nMeshCount * sizeof(CMesh*));
		delete []m_pMesh;
	}
	m_pMesh = pMeshBuffer;
	for (UINT i = 0; i < Count; i++)
	{
		if (!(m_pMesh[m_nMeshCount] = new CMesh())) return -1;
		m_nMeshCount++;
	}
	return m_nMeshCount - Count;
}

void CTerrain::SetD3DDevice(LPDIRECT3DDEVICE9 pD3DDevice, bool HardwareTnL)
{
	if (!pD3DDevice) return;
	m_pD3DDevice = pD3DDevice;
	m_pD3DDevice->AddRef();
	m_bHardwareTnL = HardwareTnL;
}

void CTerrain::UpdatePlayer(LPVOID pContext, CPlayer *pPlayer, float TimeScale)
{
	if (!pContext || !pPlayer) return;
	VOLUME_INFO Volume = pPlayer->GetVolumeInfo();
	D3DXVECTOR3 Position = pPlayer->GetPosition();
	D3DXVECTOR3 Velocity = pPlayer->GetVelocity();
	bool ReverseQuad = false;
	int PosZ = (int)(Position.z / ((CTerrain*)pContext)->m_vecScale.z);
	if ( (PosZ % 2) ! = 0 ) ReverseQuad = true;
	float fHeight = ((CTerrain*)pContext)->GetHeight( Position.x, Position.z, ReverseQuad ) - Volume.Min.y;
	if (Position.y < fHeight)
	{
		Velocity.y = 0;
		Position.y = fHeight;
		pPlayer->SetVelocity(Velocity);
		pPlayer->SetPosition(Position);
	}
}

void CTerrain::UpdateCamera(LPVOID pContext, CCamera *pCamera, float TimeScale)
{
	if (!pContext || !pCamera) return;
	if (pCamera->GetCameraMode() ! = CCamera::MODE_THIRDPERSON) return;
	VOLUME_INFO Volume = pCamera->GetVolumeInfo();
	D3DXVECTOR3 Position = pCamera->GetPosition();
	bool ReverseQuad = false;
	ULONG PosZ = (ULONG)(Position.z / ((CTerrain*)pContext)->m_vecScale.z);
	if ((PosZ % 2) ! = 0) ReverseQuad = true; else ReverseQuad = false;
	float fHeight = ((CTerrain*)pContext)->GetHeight(Position.x, Position.z, ReverseQuad) - Volume.Min.y;
	if (Position.y < fHeight)
	{
		Position.y = fHeight;
		pCamera->SetPosition(Position);
	}
	CPlayer *pPlayer = pCamera->GetPlayer();
	if (!pPlayer) return;
	((CCam3rdPerson*)pCamera)->SetLookAt(pPlayer->GetPosition());
}

#include "..\\Includes\\CPlayer.h"
#include "..\\Includes\\CCamera.h"
#include "..\\Includes\\CObject.h"

CPlayer::CPlayer()
{
	m_pCamera = NULL;
	m_p3rdPersonObject = NULL;
	m_CameraMode = 0;
	m_nUpdatePlayerCount = 0;
	m_nUpdateCameraCount = 0;
	m_vecPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecCamOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fCameraLag = 0.0f;
	m_vecVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecGravity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 125.0f;
	m_fMaxVelocityY = 125.0f;
	m_fFriction = 250.0f;
	m_Volume.Min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Volume.Max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

CPlayer::~CPlayer()
{
	if (m_pCamera) delete m_pCamera;
	m_pCamera = NULL;
	m_p3rdPersonObject = NULL;
}

void CPlayer::Move(ULONG Direction, float Distance, bool Velocity)
{
	D3DXVECTOR3 vecShift = D3DXVECTOR3(0, 0, 0);
	if (Direction & DIR_FORWARD) vecShift + = m_vecLook * Distance;
	if (Direction & DIR_BACKWARD) vecShift - = m_vecLook * Distance;
	if (Direction & DIR_RIGHT) vecShift + = m_vecRight * Distance;
	if (Direction & DIR_LEFT) vecShift - = m_vecRight * Distance;
	if (Direction & DIR_UP) vecShift + = m_vecUp * Distance;
	if (Direction & DIR_DOWN) vecShift - = m_vecUp * Distance;
	if (Direction) Move(vecShift, Velocity);
}

void CPlayer::Move(const D3DXVECTOR3& vecShift, bool Velocity)
{
	if (Velocity)
	{
		m_vecVelocity + = vecShift;
	}
	else
	{
		m_vecPos + = vecShift;
		m_pCamera->Move(vecShift);
	}
}

void CPlayer::SetCamOffset(const D3DXVECTOR3& Offset)
{
	m_vecCamOffset = Offset;
	if (!m_pCamera) return;
	m_pCamera->SetPosition(m_vecPos + Offset);
}

void CPlayer::Update(float TimeScale)
{
	ULONG i;
	m_vecVelocity + = m_vecGravity * TimeScale;
	float Length = sqrtf(m_vecVelocity.x * m_vecVelocity.x + m_vecVelocity.z * m_vecVelocity.z);
	if (Length > m_fMaxVelocityXZ)
	{
		m_vecVelocity.x * = (m_fMaxVelocityXZ / Length);
		m_vecVelocity.z * = (m_fMaxVelocityXZ / Length);
	}
	Length = sqrtf(m_vecVelocity.y * m_vecVelocity.y);
	if (Length > m_fMaxVelocityY)
	{
		m_vecVelocity.y * = (m_fMaxVelocityY / Length);
	}
	Move(m_vecVelocity * TimeScale, false);
	for (i =0; i < m_nUpdatePlayerCount; i++)
	{
		UPDATEPLAYER UpdatePlayer = (UPDATEPLAYER)m_pUpdatePlayer[i].pFunction;
		UpdatePlayer(m_pUpdatePlayer[i].pContext, this, TimeScale);
	}
	m_pCamera->Update(TimeScale, m_fCameraLag);
	for (i =0; i < m_nUpdateCameraCount; i++)
	{
		UPDATECAMERA UpdateCamera = (UPDATECAMERA)m_pUpdateCamera[i].pFunction;
		UpdateCamera(m_pUpdateCamera[i].pContext, m_pCamera, TimeScale);
	}
	D3DXVECTOR3 vecDec = -m_vecVelocity;
	D3DXVec3Normalize(&vecDec, &vecDec);
	Length = D3DXVec3Length(&m_vecVelocity);
	float Dec = (m_fFriction * TimeScale);
	if (Dec > Length) Dec = Length;
	m_vecVelocity + = vecDec * Dec;
}

bool CPlayer::SetCameraMode(ULONG Mode)
{
	CCamera *pNewCamera = NULL;
	if (m_pCamera && m_CameraMode = = Mode) return true;
	switch (Mode)
	{
		case CCamera::MODE_FPS:
		if (!(pNewCamera = new CCam1stPerson(m_pCamera))) return false;
		break;
		case CCamera::MODE_THIRDPERSON:
		if (!(pNewCamera = new CCam3rdPerson(m_pCamera))) return false;
		break;
		case CCamera::MODE_SPACECRAFT:
		if (!(pNewCamera = new CCamSpaceCraft(m_pCamera))) return false;
		break;
	}
	if (!pNewCamera) return false;
	if (m_CameraMode = = CCamera::MODE_SPACECRAFT)
	{
		m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_vecRight.y = 0.0f;
		m_vecLook.y = 0.0f;
		D3DXVec3Normalize(&m_vecRight, &m_vecRight);
		D3DXVec3Normalize(&m_vecLook, &m_vecLook);
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		m_fYaw = D3DXToDegree(acosf(D3DXVec3Dot(&D3DXVECTOR3(0.0f, 0.0f, 1.0f), &m_vecLook)));
		if (m_vecLook.x < 0.0f) m_fYaw = -m_fYaw;
	}
	else if (m_pCamera && Mode = = CCamera::MODE_SPACECRAFT)
	{
		m_vecRight = m_pCamera->GetRight();
		m_vecLook = m_pCamera->GetLook();
		m_vecUp = m_pCamera->GetUp();
	}
	m_CameraMode = Mode;
	pNewCamera->AttachToPlayer(this);
	if (m_pCamera) delete m_pCamera;
	m_pCamera = pNewCamera;
	return true;
}

void CPlayer::Rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	if (!m_pCamera) return;
	if (m_CameraMode = = CCamera::MODE_FPS || m_CameraMode = = CCamera::MODE_THIRDPERSON)
	{
		if (x)
		{
			m_fPitch + = x;
			if (m_fPitch > 89.0f) {x - = (m_fPitch - 89.0f); m_fPitch = 89.0f;}
			if (m_fPitch < -89.0f) {x - = (m_fPitch + 89.0f); m_fPitch = -89.0f;}
		}
		if (y)
		{
			m_fYaw + = y;
			if (m_fYaw > 360.0f) m_fYaw - = 360.0f;
			if (m_fYaw < 0.0f) m_fYaw + = 360.0f;
		}
		if (z)
		{
			m_fRoll + = z;
			if (m_fRoll > 20.0f) {z - = (m_fRoll - 20.0f); m_fRoll = 20.0f;}
			if (m_fRoll < -20.0f) {z - = (m_fRoll + 20.0f); m_fRoll = -20.0f;}
		}
		m_pCamera->Rotate(x, y, z);
		if (y)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_vecUp, D3DXToRadian(y));
			D3DXVec3TransformNormal(&m_vecLook, &m_vecLook, &mtxRotate);
			D3DXVec3TransformNormal(&m_vecRight, &m_vecRight, &mtxRotate);
		}
	}
	else if (m_CameraMode = = CCamera::MODE_SPACECRAFT)
	{
		m_pCamera->Rotate(x, y, z);
		if (x ! = 0)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_vecRight, D3DXToRadian(x));
			D3DXVec3TransformNormal(&m_vecLook, &m_vecLook, &mtxRotate);
			D3DXVec3TransformNormal(&m_vecUp, &m_vecUp, &mtxRotate);
		}
		if ( y ! = 0 )
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_vecUp, D3DXToRadian(y));
			D3DXVec3TransformNormal(&m_vecLook, &m_vecLook, &mtxRotate);
			D3DXVec3TransformNormal(&m_vecRight, &m_vecRight, &mtxRotate);
		}
		if (z ! = 0)
		{
			D3DXMatrixRotationAxis(&mtxRotate, &m_vecLook, D3DXToRadian(z));
			D3DXVec3TransformNormal(&m_vecUp, &m_vecUp, &mtxRotate);
			D3DXVec3TransformNormal(&m_vecRight, &m_vecRight, &mtxRotate);
		}
	}
	D3DXVec3Normalize(&m_vecLook, &m_vecLook);
	D3DXVec3Cross(&m_vecRight, &m_vecUp, &m_vecLook);
	D3DXVec3Normalize(&m_vecRight, &m_vecRight);
	D3DXVec3Cross(&m_vecUp, &m_vecLook, &m_vecRight);
	D3DXVec3Normalize(&m_vecUp, &m_vecUp);
}

void CPlayer::SetVolumeInfo(const VOLUME_INFO& Volume)
{
	m_Volume = Volume;
}

const VOLUME_INFO& CPlayer::GetVolumeInfo() const
{
	return m_Volume;
}

void CPlayer::AddPlayerCallback(UPDATEPLAYER pFunc, LPVOID pContext)
{
	m_pUpdatePlayer[m_nUpdatePlayerCount].pFunction = (LPVOID)pFunc;
	m_pUpdatePlayer[m_nUpdatePlayerCount].pContext = pContext;
	m_nUpdatePlayerCount++;
}

void CPlayer::AddCameraCallback(UPDATECAMERA pFunc, LPVOID pContext)
{
	m_pUpdateCamera[m_nUpdateCameraCount].pFunction = (LPVOID)pFunc;
	m_pUpdateCamera[m_nUpdateCameraCount].pContext = pContext;
	m_nUpdateCameraCount++;
}

void CPlayer::RemovePlayerCallback(UPDATEPLAYER pFunc, LPVOID pContext)
{
	int i;
	for (i = 0; i < m_nUpdatePlayerCount; i++)
	{
		if (m_pUpdatePlayer[i].pFunction = = pFunc && m_pUpdatePlayer[i].pContext = = pContext) break;
	}
	if (i = = m_nUpdatePlayerCount) return;
	if (i < (m_nUpdatePlayerCount - 1))
	{
		memmove(&m_pUpdatePlayer[i], &m_pUpdatePlayer[i + 1], (m_nUpdatePlayerCount - 2) * sizeof(CALLBACK_FUNC));
	}
	m_nUpdatePlayerCount--;
}

void CPlayer::RemoveCameraCallback(UPDATECAMERA pFunc, LPVOID pContext)
{
	int i;
	for (i = 0; i < m_nUpdateCameraCount; i++)
	{
		if (m_pUpdateCamera[i].pFunction = = pFunc && m_pUpdateCamera[i].pContext = = pContext) break;
	}
	if (i = = m_nUpdateCameraCount) return;
	if (i < (m_nUpdateCameraCount - 1))
	{
		memmove(&m_pUpdateCamera[i], &m_pUpdateCamera[i + 1], (m_nUpdateCameraCount - 2) * sizeof(CALLBACK_FUNC));
	}
	m_nUpdateCameraCount--;
}

void CPlayer::Render(LPDIRECT3DDEVICE9 pDevice)
{
	CObject *pObject = NULL;
	if (!pDevice) return;
	if (m_pCamera)
	{
		if (m_CameraMode = = CCamera::MODE_THIRDPERSON) pObject = m_p3rdPersonObject;
	}
	else
	{
		pObject = m_p3rdPersonObject;
	}
	if (!pObject) return;
	D3DXMATRIX *pMatrix = &pObject->m_mtxWorld;
	pMatrix->_11 = m_vecRight.x; pMatrix->_21 = m_vecUp.x; pMatrix->_31 = m_vecLook.x;
	pMatrix->_12 = m_vecRight.y; pMatrix->_22 = m_vecUp.y; pMatrix->_32 = m_vecLook.y;
	pMatrix->_13 = m_vecRight.z; pMatrix->_23 = m_vecUp.z; pMatrix->_33 = m_vecLook.z;
	pMatrix->_41 = m_vecPos.x;
	pMatrix->_42 = m_vecPos.y - 10.0f;
	pMatrix->_43 = m_vecPos.z;
	CMesh *pMesh = pObject->m_pMesh;
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	pDevice->SetTransform(D3DTS_WORLD, &pObject->m_mtxWorld);
	pDevice->SetStreamSource(0, pMesh->m_pVertexBuffer, 0, sizeof(CVertex));
	pDevice->SetIndices(pMesh->m_pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 8, 0, 14);
}

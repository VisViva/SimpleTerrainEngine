#include "..\\Includes\\CCamera.h"
#include "..\\Includes\\CPlayer.h"

CCamera::CCamera()
{
	m_pPlayer = NULL;
	m_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fFOV = 60.0f;
	m_fNearClip = 1.0f;
	m_fFarClip = 100.0f;
	m_Viewport.X = 0;
	m_Viewport.Y = 0;
	m_Viewport.Width = 640;
	m_Viewport.Height = 480;
	m_Viewport.MinZ = 0.0f;
	m_Viewport.MaxZ = 1.0f;
	m_bViewDirty = true;
	m_bProjDirty = true;
	m_bFrustumDirty = true;
	D3DXMatrixIdentity(&m_mtxView);
	D3DXMatrixIdentity(&m_mtxProj);
}

CCamera::CCamera(const CCamera *pCamera)
{
	m_pPlayer = NULL;
	m_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fFOV = 60.0f;
	m_fNearClip = 1.0f;
	m_fFarClip = 100.0f;
	m_Viewport.X = 0;
	m_Viewport.Y = 0;
	m_Viewport.Width = 640;
	m_Viewport.Height = 480;
	m_Viewport.MinZ = 0.0f;
	m_Viewport.MaxZ = 1.0f;
	m_bViewDirty = true;
	m_bProjDirty = true;
	m_bFrustumDirty = true;
	D3DXMatrixIdentity(&m_mtxView);
	D3DXMatrixIdentity(&m_mtxProj);
}

CCamera::~CCamera()
{
}

void CCamera::AttachToPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}

void CCamera::DetachFromPlayer()
{
	m_pPlayer = NULL;
}

void CCamera::SetViewport(long Left, long Top, long Width, long Height, float NearClip, float FarClip, LPDIRECT3DDEVICE9 pDevice)
{
	m_Viewport.X = Left;
	m_Viewport.Y = Top;
	m_Viewport.Width = Width;
	m_Viewport.Height = Height;
	m_Viewport.MinZ = 0.0f;
	m_Viewport.MaxZ = 1.0f;
	m_fNearClip = NearClip;
	m_fFarClip = FarClip;
	m_bProjDirty = true;
	m_bFrustumDirty = true;
	if (pDevice) pDevice->SetViewport(&m_Viewport);
}

const D3DXMATRIX& CCamera::GetProjMatrix()
{
	if (m_bProjDirty)
	{
		float fAspect = (float)m_Viewport.Width / (float)m_Viewport.Height;
		D3DXMatrixPerspectiveFovLH(&m_mtxProj, D3DXToRadian(m_fFOV), fAspect, m_fNearClip, m_fFarClip);
		m_bProjDirty = false;
	}
	return m_mtxProj;
}

const D3DXMATRIX& CCamera::GetViewMatrix()
{
	if (m_bViewDirty)
	{
		D3DXVec3Normalize(&m_vecLook, &m_vecLook);
		D3DXVec3Cross(&m_vecRight, &m_vecUp, &m_vecLook);
		D3DXVec3Normalize(&m_vecRight, &m_vecRight);
		D3DXVec3Cross(&m_vecUp, &m_vecLook, &m_vecRight);
		D3DXVec3Normalize(&m_vecUp, &m_vecUp);
		m_mtxView._11 = m_vecRight.x; m_mtxView._12 = m_vecUp.x; m_mtxView._13 = m_vecLook.x;
		m_mtxView._21 = m_vecRight.y; m_mtxView._22 = m_vecUp.y; m_mtxView._23 = m_vecLook.y;
		m_mtxView._31 = m_vecRight.z; m_mtxView._32 = m_vecUp.z; m_mtxView._33 = m_vecLook.z;
		m_mtxView._41 =- D3DXVec3Dot(&m_vecPos, &m_vecRight);
		m_mtxView._42 =- D3DXVec3Dot(&m_vecPos, &m_vecUp);
		m_mtxView._43 =- D3DXVec3Dot(&m_vecPos, &m_vecLook);
		m_bViewDirty = false;
	}
	return m_mtxView;
}

void CCamera::UpdateRenderView(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if (!pD3DDevice) return;
	pD3DDevice->SetTransform(D3DTS_VIEW, &GetViewMatrix());
}

void CCamera::UpdateRenderProj(LPDIRECT3DDEVICE9 pD3DDevice)
{
	if (!pD3DDevice) return;
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &GetProjMatrix());
}

void CCamera::CalcFrustumPlanes()
{
	if (!m_bFrustumDirty) return;
	D3DXMATRIX m = GetViewMatrix() * GetProjMatrix();
	m_Frustum[0].a = -(m._14 + m._11);
	m_Frustum[0].b = -(m._24 + m._21);
	m_Frustum[0].c = -(m._34 + m._31);
	m_Frustum[0].d = -(m._44 + m._41);
	m_Frustum[1].a = -(m._14 - m._11);
	m_Frustum[1].b = -(m._24 - m._21);
	m_Frustum[1].c = -(m._34 - m._31);
	m_Frustum[1].d = -(m._44 - m._41);
	m_Frustum[2].a = -(m._14 - m._12);
	m_Frustum[2].b = -(m._24 - m._22);
	m_Frustum[2].c = -(m._34 - m._32);
	m_Frustum[2].d = -(m._44 - m._42);
	m_Frustum[3].a = -(m._14 + m._12);
	m_Frustum[3].b = -(m._24 + m._22);
	m_Frustum[3].c = -(m._34 + m._32);
	m_Frustum[3].d = -(m._44 + m._42);
	m_Frustum[4].a = -(m._13);
	m_Frustum[4].b = -(m._23);
	m_Frustum[4].c = -(m._33);
	m_Frustum[4].d = -(m._43);
	m_Frustum[5].a = -(m._14 - m._13);
	m_Frustum[5].b = -(m._24 - m._23);
	m_Frustum[5].c = -(m._34 - m._33);
	m_Frustum[5].d = -(m._44 - m._43);
	for (unsigned long i = 0; i < 6; i++) D3DXPlaneNormalize(&m_Frustum[i], &m_Frustum[i]);
	m_bFrustumDirty = false;
}

bool CCamera::BoundsInFrustum(const D3DXVECTOR3 &Min, const D3DXVECTOR3 &Max)
{
	CalcFrustumPlanes();
	unsigned long i;
	D3DXVECTOR3 NearPoint, FarPoint, Normal;
	for (i = 0; i < 6; i++)
	{
		Normal = D3DXVECTOR3(m_Frustum[i].a, m_Frustum[i].b, m_Frustum[i].c);
		if (Normal.x > 0.0f)
		{
			if (Normal.y > 0.0f)
			{
				if (Normal.z > 0.0f)
				{
					NearPoint.x = Min.x; NearPoint.y = Min.y; NearPoint.z = Min.z;
				}
				else
				{
					NearPoint.x = Min.x; NearPoint.y = Min.y; NearPoint.z = Max.z;
				}
			}
			else
			{
				if (Normal.z > 0.0f)
				{
					NearPoint.x = Min.x; NearPoint.y = Max.y; NearPoint.z = Min.z;
				}
				else
				{
					NearPoint.x = Min.x; NearPoint.y = Max.y; NearPoint.z = Max.z;
				}
			}
		}
		else
		{
			if (Normal.y > 0.0f)
			{
				if (Normal.z > 0.0f)
				{
					NearPoint.x = Max.x; NearPoint.y = Min.y; NearPoint.z = Min.z;
				}
				else
				{
					NearPoint.x = Max.x; NearPoint.y = Min.y; NearPoint.z = Max.z;
				}
			}
			else
			{
				if (Normal.z > 0.0f)
				{
					NearPoint.x = Max.x; NearPoint.y = Max.y; NearPoint.z = Min.z;
				}
				else
				{
					NearPoint.x = Max.x; NearPoint.y = Max.y; NearPoint.z = Max.z;
				}
			}
		}
		if (D3DXVec3Dot(&Normal, &NearPoint) + m_Frustum[i].d > 0.0f) return false;
	}
	return true;
}

void CCamera::SetVolumeInfo(const VOLUME_INFO& Volume)
{
	m_Volume = Volume;
}

const VOLUME_INFO& CCamera::GetVolumeInfo() const
{
	return m_Volume;
}

CCam1stPerson::CCam1stPerson(const CCamera *pCamera)
{
	SetCameraDetails(pCamera);
}

void CCam1stPerson::SetCameraDetails(const CCamera *pCamera)
{
	if (!pCamera) return;
	m_vecPos = pCamera->GetPosition();
	m_vecRight = pCamera->GetRight();
	m_vecLook = pCamera->GetLook();
	m_vecUp = pCamera->GetUp();
	m_fFOV = pCamera->GetFOV();
	m_fNearClip = pCamera->GetNearClip();
	m_fFarClip = pCamera->GetFarClip();
	m_Viewport = pCamera->GetViewport();
	m_Volume = pCamera->GetVolumeInfo();
	if (pCamera->GetCameraMode() = = MODE_SPACECRAFT)
	{
		m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_vecRight.y = 0.0f;
		m_vecLook.y = 0.0f;
		D3DXVec3Normalize(&m_vecRight, &m_vecRight);
		D3DXVec3Normalize(&m_vecLook, &m_vecLook);
	}
	m_bViewDirty = true;
	m_bProjDirty = true;
}

void CCam1stPerson::Rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	if (!m_pPlayer) return;
	if (x ! = 0)
	{
		D3DXMatrixRotationAxis( &mtxRotate, &m_vecRight, D3DXToRadian( x ) );
		D3DXVec3TransformNormal( &m_vecLook, &m_vecLook, &mtxRotate );
		D3DXVec3TransformNormal( &m_vecUp, &m_vecUp, &mtxRotate );
		D3DXVec3TransformNormal( &m_vecRight, &m_vecRight, &mtxRotate );
	}
	if (y ! = 0)
	{
		D3DXMatrixRotationAxis(&mtxRotate, &m_pPlayer->GetUp(), D3DXToRadian(y));
		D3DXVec3TransformNormal(&m_vecLook, &m_vecLook, &mtxRotate);
		D3DXVec3TransformNormal(&m_vecUp, &m_vecUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_vecRight, &m_vecRight, &mtxRotate);
	}
	if (z ! = 0)
	{
		D3DXMatrixRotationAxis(&mtxRotate, &m_pPlayer->GetLook(), D3DXToRadian(z));
		m_vecPos - = m_pPlayer->GetPosition();
		D3DXVec3TransformCoord(&m_vecPos, &m_vecPos, &mtxRotate);
		m_vecPos + = m_pPlayer->GetPosition();
		D3DXVec3TransformNormal(&m_vecLook, &m_vecLook, &mtxRotate);
		D3DXVec3TransformNormal(&m_vecUp, &m_vecUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_vecRight, &m_vecRight, &mtxRotate);
	}
	m_bViewDirty = true;
	m_bFrustumDirty = true;
}

CCam3rdPerson::CCam3rdPerson(const CCamera *pCamera)
{
	SetCameraDetails(pCamera);
}

void CCam3rdPerson::SetCameraDetails(const CCamera *pCamera)
{
	if (!pCamera) return;
	m_vecPos = pCamera->GetPosition();
	m_vecRight = pCamera->GetRight();
	m_vecLook = pCamera->GetLook();
	m_vecUp = pCamera->GetUp();
	m_fFOV = pCamera->GetFOV();
	m_fNearClip = pCamera->GetNearClip();
	m_fFarClip = pCamera->GetFarClip();
	m_Viewport = pCamera->GetViewport();
	m_Volume = pCamera->GetVolumeInfo();
	if (pCamera->GetCameraMode() = = MODE_SPACECRAFT)
	{
		m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_vecRight.y = 0.0f;
		m_vecLook.y = 0.0f;
		D3DXVec3Normalize(&m_vecRight, &m_vecRight);
		D3DXVec3Normalize(&m_vecLook, &m_vecLook);
	}
	m_bViewDirty = true;
	m_bProjDirty = true;
	m_bFrustumDirty = true;
}

void CCam3rdPerson::Update(float TimeScale, float Lag)
{
	D3DXMATRIX  mtxRotate;
	D3DXVECTOR3 vecOffset, vecPosition, vecDir;
	if (!m_pPlayer) return;
	float fTimeScale = 1.0f, Length = 0.0f;
	if (Lag ! = 0.0f) fTimeScale = TimeScale * (1.0f / Lag);
	D3DXMatrixIdentity(&mtxRotate);
	D3DXVECTOR3 vecRight = m_pPlayer->GetRight(), vecUp = m_pPlayer->GetUp(), vecLook = m_pPlayer->GetLook();
	mtxRotate._11 = vecRight.x; mtxRotate._21 = vecUp.x; mtxRotate._31 = vecLook.x;
	mtxRotate._12 = vecRight.y; mtxRotate._22 = vecUp.y; mtxRotate._32 = vecLook.y;
	mtxRotate._13 = vecRight.z; mtxRotate._23 = vecUp.z; mtxRotate._33 = vecLook.z;
	D3DXVec3TransformCoord(&vecOffset, &m_pPlayer->GetCamOffset(), &mtxRotate);
	vecPosition = m_pPlayer->GetPosition() + vecOffset;
	vecDir = vecPosition - m_vecPos;
	Length = D3DXVec3Length(&vecDir);
	D3DXVec3Normalize(&vecDir, &vecDir);
	float Distance = Length * fTimeScale;
	if (Distance > Length) Distance = Length;
	if (Length < 0.01f) Distance = Length;
	if (Distance > 0)
	{
		m_vecPos + = vecDir *Distance;
		SetLookAt(m_pPlayer->GetPosition());
		m_bViewDirty = true;
		m_bFrustumDirty = true;
	}
}

void CCam3rdPerson::SetLookAt(const D3DXVECTOR3& vecLookAt)
{
	D3DXMATRIX Matrix;
	D3DXMatrixLookAtLH(&Matrix, &m_vecPos, &vecLookAt, &m_pPlayer->GetUp());
	m_vecRight = D3DXVECTOR3(Matrix._11, Matrix._21, Matrix._31);
	m_vecUp = D3DXVECTOR3(Matrix._12, Matrix._22, Matrix._32);
	m_vecLook = D3DXVECTOR3(Matrix._13, Matrix._23, Matrix._33);
	m_bViewDirty = true;
	m_bFrustumDirty = true;
}

CCamSpaceCraft::CCamSpaceCraft(const CCamera *pCamera)
{
	SetCameraDetails(pCamera);
}

void CCamSpaceCraft::SetCameraDetails(const CCamera *pCamera)
{
	if (!pCamera) return;
	m_vecPos = pCamera->GetPosition();
	m_vecRight = pCamera->GetRight();
	m_vecLook = pCamera->GetLook();
	m_vecUp = pCamera->GetUp();
	m_fFOV = pCamera->GetFOV();
	m_fNearClip = pCamera->GetNearClip();
	m_fFarClip = pCamera->GetFarClip();
	m_Viewport = pCamera->GetViewport();
	m_Volume = pCamera->GetVolumeInfo();
	m_bViewDirty = true;
	m_bProjDirty = true;
	m_bFrustumDirty = true;
}

void CCamSpaceCraft::Rotate(float x, float y, float z)
{
	D3DXMATRIX mtxRotate;
	if (!m_pPlayer) return;
	if (x ! = 0)
	{
		D3DXMatrixRotationAxis(&mtxRotate, &m_pPlayer->GetRight(), D3DXToRadian(x));
		D3DXVec3TransformNormal(&m_vecLook, &m_vecLook, &mtxRotate);
		D3DXVec3TransformNormal(&m_vecUp, &m_vecUp, &mtxRotate);
		m_vecPos - = m_pPlayer->GetPosition();
		D3DXVec3TransformCoord(&m_vecPos, &m_vecPos, &mtxRotate);
		m_vecPos + = m_pPlayer->GetPosition();
	}
	if (y ! = 0)
	{
		D3DXMatrixRotationAxis(&mtxRotate, &m_pPlayer->GetUp(), D3DXToRadian(y));
		D3DXVec3TransformNormal(&m_vecLook, &m_vecLook, &mtxRotate);
		D3DXVec3TransformNormal(&m_vecRight, &m_vecRight, &mtxRotate);
		m_vecPos - = m_pPlayer->GetPosition();
		D3DXVec3TransformCoord(&m_vecPos, &m_vecPos, &mtxRotate);
		m_vecPos + = m_pPlayer->GetPosition();
	}
	if (z ! = 0)
	{
		D3DXMatrixRotationAxis(&mtxRotate, &m_pPlayer->GetLook(), D3DXToRadian(z));
		D3DXVec3TransformNormal(&m_vecUp, &m_vecUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_vecRight, &m_vecRight, &mtxRotate);
		m_vecPos - = m_pPlayer->GetPosition();
		D3DXVec3TransformCoord(&m_vecPos, &m_vecPos, &mtxRotate);
		m_vecPos + = m_pPlayer->GetPosition();
	}
	m_bViewDirty = true;
	m_bFrustumDirty = true;
}

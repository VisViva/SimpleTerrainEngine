#ifndef CCAMERA_H
#define CCAMERA_H
#include "Main.h"
#include "CPlayer.h"

class CCamera
{
public:
	enum CAMERA_MODE {
		MODE_FPS = 1,
		MODE_THIRDPERSON = 2,
		MODE_SPACECRAFT = 3,
		MODE_FORCE_32BIT = 0x7FFFFFFF
	};
	CCamera();
	CCamera(const CCamera *pCamera);
	virtual ~CCamera();
	void SetFOV(float FOV) {m_fFOV = FOV; m_bProjDirty = true;}
	void SetViewport(long Left, long Top, long Width, long Height, float NearClip, float FarClip, LPDIRECT3DDEVICE9 pDevice = NULL);
	void UpdateRenderView(LPDIRECT3DDEVICE9 pD3DDevice);
	void UpdateRenderProj(LPDIRECT3DDEVICE9 pD3DDevice);
	const D3DXMATRIX& GetProjMatrix();
	float GetFOV() const {return m_fFOV;}
	float GetNearClip() const {return m_fNearClip;}
	float GetFarClip() const {return m_fFarClip;}
	const D3DVIEWPORT9& GetViewport() const {return m_Viewport;}
	CPlayer* GetPlayer() const {return m_pPlayer;}
	const D3DXVECTOR3& GetPosition() const {return m_vecPos;}
	const D3DXVECTOR3& GetLook() const {return m_vecLook;}
	const D3DXVECTOR3& GetUp() const {return m_vecUp;}
	const D3DXVECTOR3& GetRight() const {return m_vecRight;}
	const D3DXMATRIX& GetViewMatrix();
	void SetVolumeInfo(const VOLUME_INFO &Volume);
	const VOLUME_INFO& GetVolumeInfo() const;
	bool BoundsInFrustum(const D3DXVECTOR3 &Min, const D3DXVECTOR3 &Max);
	virtual void AttachToPlayer(CPlayer *pPlayer);
	virtual void DetachFromPlayer();
	virtual void SetPosition(const D3DXVECTOR3& Position) {m_vecPos = Position; m_bViewDirty = true; m_bFrustumDirty = true;}
	virtual void Move(const D3DXVECTOR3& vecShift) {m_vecPos + = vecShift; m_bViewDirty = true; m_bFrustumDirty = true;}
	virtual void Rotate(float x, float y, float z) {}
	virtual void Update(float TimeScale, float Lag) {}
	virtual void SetCameraDetails(const CCamera *pCamera) {}
	virtual CAMERA_MODE GetCameraMode() const = 0;
protected:
	void CalcFrustumPlanes();
	CPlayer *m_pPlayer;
	VOLUME_INFO m_Volume;
	D3DXMATRIX m_mtxView;
	D3DXMATRIX m_mtxProj;
	D3DXPLANE m_Frustum[6];
	bool m_bViewDirty;
	bool m_bProjDirty;
	bool m_bFrustumDirty;
	float m_fFOV;
	float m_fNearClip;
	float m_fFarClip;
	D3DVIEWPORT9 m_Viewport;
	D3DXVECTOR3 m_vecPos;
	D3DXVECTOR3 m_vecUp;
	D3DXVECTOR3 m_vecLook;
	D3DXVECTOR3 m_vecRight;
};
class CCam1stPerson : public CCamera
{
public:
	CCam1stPerson();
	CCam1stPerson(const CCamera *pCamera);
	CAMERA_MODE GetCameraMode() const {return MODE_FPS;}
	void Rotate(float x, float y, float z);
	void SetCameraDetails(const CCamera *pCamera);
};
class CCam3rdPerson : public CCamera
{
public:
	CCam3rdPerson();
	CCam3rdPerson(const CCamera *pCamera);
	CAMERA_MODE GetCameraMode() const {return MODE_THIRDPERSON;}
	void Move(const D3DXVECTOR3& vecShift) {};
	void Rotate(float x, float y, float z) {};
	void Update(float TimeScale, float Lag);
	void SetCameraDetails(const CCamera *pCamera);
	void SetLookAt(const D3DXVECTOR3& vecLookAt);
};
class CCamSpaceCraft : public CCamera
{
public:
	CCamSpaceCraft();
	CCamSpaceCraft(const CCamera *pCamera);
	CAMERA_MODE GetCameraMode() const {return MODE_SPACECRAFT;}
	void Rotate (float x, float y, float z);
	void SetCameraDetails (const CCamera *pCamera);
};
#endif

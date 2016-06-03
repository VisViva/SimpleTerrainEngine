#ifndef CPLAYER_H
#define CPLAYER_H
#include "Main.h"

typedef struct _VOLUME_INFO
{
	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;
} VOLUME_INFO;

typedef struct _CALLBACK_FUNC
{
	LPVOID pFunction;
	LPVOID pContext;
} CALLBACK_FUNC;

class CObject;
class CCamera;
class CPlayer;

typedef void (*UPDATEPLAYER)(LPVOID pContext, CPlayer * pPlayer, float TimeScale);
typedef void (*UPDATECAMERA)(LPVOID pContext, CCamera * pCamera, float TimeScale);

class CPlayer
{
public:
	enum DIRECTION {
		DIR_FORWARD = 1,
		DIR_BACKWARD = 2,
		DIR_LEFT = 4,
		DIR_RIGHT = 8,
		DIR_UP = 16,
		DIR_DOWN = 32,
		DIR_FORCE_32BIT = 0x7FFFFFFF
	};
	CPlayer();
	virtual ~CPlayer();
	bool SetCameraMode (ULONG Mode);
	void Update (float TimeScale);
	void AddPlayerCallback(UPDATEPLAYER pFunc, LPVOID pContext);
	void AddCameraCallback(UPDATECAMERA pFunc, LPVOID pContext);
	void RemovePlayerCallback(UPDATEPLAYER pFunc, LPVOID pContext);
	void RemoveCameraCallback(UPDATECAMERA pFunc, LPVOID pContext);
	void Set3rdPersonObject(CObject *pObject) {m_p3rdPersonObject = pObject;}
	void SetFriction(float Friction) {m_fFriction = Friction;}
	void SetGravity(const D3DXVECTOR3& Gravity) {m_vecGravity = Gravity;}
	void SetMaxVelocityXZ(float MaxVelocity) {m_fMaxVelocityXZ = MaxVelocity;}
	void SetMaxVelocityY(float MaxVelocity) {m_fMaxVelocityY = MaxVelocity;}
	void SetVelocity(const D3DXVECTOR3& Velocity) {m_vecVelocity = Velocity;}
	void SetCamLag(float CamLag) {m_fCameraLag = CamLag;}
	void SetCamOffset(const D3DXVECTOR3& Offset);
	void SetVolumeInfo(const VOLUME_INFO& Volume);
	const VOLUME_INFO& GetVolumeInfo() const;
	CCamera* GetCamera() const {return m_pCamera;}
	const D3DXVECTOR3& GetVelocity() const {return m_vecVelocity;}
	const D3DXVECTOR3& GetCamOffset() const {return m_vecCamOffset;}
	const D3DXVECTOR3& GetPosition() const {return m_vecPos;}
	const D3DXVECTOR3& GetLook() const {return m_vecLook;}
	const D3DXVECTOR3& GetUp() const {return m_vecUp;}
	const D3DXVECTOR3& GetRight() const {return m_vecRight;}
	float GetYaw() const {return m_fYaw;}
	float GetPitch() const {return m_fPitch;}
	float GetRoll() const {return m_fRoll;}
	void SetPosition(const D3DXVECTOR3& Position) {Move(Position - m_vecPos, false);}
	void Move(ULONG Direction, float Distance, bool Velocity = false);
	void Move(const D3DXVECTOR3& vecShift, bool Velocity = false);
	void Rotate(float x, float y, float z);
	void Render(LPDIRECT3DDEVICE9 pDevice);
private:
	CCamera* m_pCamera;
	CObject* m_p3rdPersonObject;
	VOLUME_INFO m_Volume;
	ULONG m_CameraMode;
	D3DXVECTOR3 m_vecPos;
	D3DXVECTOR3 m_vecUp;
	D3DXVECTOR3 m_vecRight;
	D3DXVECTOR3 m_vecLook;
	D3DXVECTOR3 m_vecCamOffset;
	float m_fPitch;
	float m_fRoll;
	float m_fYaw;
	D3DXVECTOR3 m_vecVelocity;
	D3DXVECTOR3 m_vecGravity;
	float m_fMaxVelocityXZ;
	float m_fMaxVelocityY;
	float m_fFriction;
	float m_fCameraLag;
	CALLBACK_FUNC m_pUpdatePlayer[255];
	CALLBACK_FUNC m_pUpdateCamera[255];
	USHORT m_nUpdatePlayerCount;
	USHORT m_nUpdateCameraCount;
};
#endif

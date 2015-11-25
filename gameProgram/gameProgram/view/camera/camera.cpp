//*****************************************************************************
//
// カメラ [camera.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "camera.h"

#include "..\..\debugproc\debugproc.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const float _aspect			= ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
const float _shadowWidth	= 256;
const float _shadowHeight	= 255;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インライン定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
inline float _radian(float angle)
{
	return (3.14159265359f / 180.0f * angle);
}

inline float _distance(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	return sqrt((end.x-start.x)*(end.x-start.x) + (end.y-start.y)*(end.y-start.y) + (end.z-start.z)*(end.z-start.z));
}

//=============================================================================
// コンストラクタ
//=============================================================================
Camera::Camera(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_device	= device;
	m_debugproc	= nullptr;

	D3DXMatrixIdentity(&m_view);
	D3DXMatrixIdentity(&m_projection);

	m_eye		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_at		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_distance	= _distance(m_at, m_eye);

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecF = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecR = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
Camera::~Camera(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Camera::Create(Camera** outPointer, LPDIRECT3DDEVICE9 device, D3DXVECTOR3 at, D3DXVECTOR3 eye)
{
	Camera* pointer = new Camera(device);
	if(!pointer->Initialize(at, eye))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Camera::Initialize(D3DXVECTOR3 at, D3DXVECTOR3 eye)
{
	//----------------------------
	// 位置と距離
	//----------------------------
	m_eye		= eye;
	m_at		= at;
	m_distance	= _distance(m_at, m_eye);

	//----------------------------
	// 回転
	//----------------------------
	m_rot.y = atan2(m_eye.x, -m_eye.z);
	m_rot.x = atan2(m_eye.y, (float)hypot(m_eye.z, m_eye.x));

	if(m_rot.y < -D3DX_PI || m_rot.y > D3DX_PI)
	{
		m_rot.y = m_rot.y + (-(m_rot.y / m_rot.y) * D3DX_PI * 2.0f);
	}

	if(m_rot.x < -D3DX_PI || m_rot.x > D3DX_PI)
	{
		m_rot.x = m_rot.x + (-(m_rot.x / m_rot.x) * D3DX_PI * 2.0f);
	}

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Camera::Finalize(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void Camera::Update(void)
{
}

//=============================================================================
// カメラの設定（ビューとプロジェクション）
//=============================================================================
void Camera::SetCamera(void)
{
	//----------------------------
	// ビューマトリックス
	//----------------------------
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_view);
	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_view, &m_eye, &m_at, &m_vecU);
	// ビューマトリックスの設定
	m_device->SetTransform(D3DTS_VIEW, &m_view);

	//----------------------------
	// プロジェクションマトリックス
	//----------------------------
	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_projection);
	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_projection,
								_radian(45),	// 視野角
								_aspect,		// アスペクト比
								10.0f,			// near値
								10000.0f);		// far値
	// プロジェクションマトリックスの設定
	//m_device->SetTransform(D3DTS_PROJECTION, &m_projection);

/*#ifdef _DEBUG
	// カメラ情報表示
	m_debugproc->PrintDebugProc("***カメラ情報**********\nPos x:%f y:%f z:%f\nAt  x:%f y:%f z:%f\nRot x:%f y:%f z:%f\n",
								m_eye.x, m_eye.y, m_eye.z,
								m_at.x,  m_at.y,  m_at.z,
								m_rot.x, m_rot.y, m_rot.z);
#endif*/
}

//=============================================================================
// 影カメラの設定（ビューとプロジェクション）
//=============================================================================
void Camera::SetShadowCamera(D3DXVECTOR3 targetPos)
{
	D3DXMATRIX	 view,projection;
	D3DVIEWPORT9 viewport;
	D3DXVECTOR3	 at	 = targetPos;
	D3DXVECTOR3	 eye = at + D3DXVECTOR3(1.0f, 3.0f, -2.0f);
	D3DXVECTOR3	 up	 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//----------------------------
	// ビューマトリックス
	//----------------------------
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&view);
	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&view, &eye, &at, &up);
	// ビューマトリックスの設定
	//m_device->SetTransform(D3DTS_VIEW, &view);

	//----------------------------
	// プロジェクションマトリックス
	//----------------------------
	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&projection);
		// プロジェクションマトリックスの作成
		D3DXMatrixOrthoLH(&projection,
							100.0f,	// width
							100.0f,	// height
							-50.0f,	// near値
							50.0f);	// far値
	// プロジェクションマトリックスの設定
	//m_device->SetTransform(D3DTS_PROJECTION, &projection);

	//----------------------------
	// ビューポート
	//----------------------------
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width  = (DWORD)_shadowWidth;
	viewport.Height = (DWORD)_shadowHeight;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	m_device->SetViewport(&viewport);
}

// EOF

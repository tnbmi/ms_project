//*****************************************************************************
//
// �J���� [camera.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "camera.h"

#include "..\..\debugproc\debugproc.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const float _aspect			= ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
const float _shadowWidth	= 256;
const float _shadowHeight	= 255;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C�����C����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
inline float _radian(float angle)
{
	return (PAI / 180.0f * angle);
}

inline float _distance(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	return sqrt((end.x-start.x)*(end.x-start.x) + (end.y-start.y)*(end.y-start.y) + (end.z-start.z)*(end.z-start.z));
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Camera::Camera(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// �����o�[������
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
// �f�X�g���N�^
//=============================================================================
Camera::~Camera(void)
{
}

//=============================================================================
// ����
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
// ������
//=============================================================================
bool Camera::Initialize(D3DXVECTOR3 at, D3DXVECTOR3 eye)
{
	//----------------------------
	// �ʒu�Ƌ���
	//----------------------------
	m_eye		= eye;
	m_at		= at;
	m_distance	= _distance(m_at, m_eye);

	//----------------------------
	// ��]
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
// �I��
//=============================================================================
void Camera::Finalize(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void Camera::Update(void)
{
}

//=============================================================================
// �J�����̐ݒ�i�r���[�ƃv���W�F�N�V�����j
//=============================================================================
void Camera::SetCamera(void)
{
	//----------------------------
	// �r���[�}�g���b�N�X
	//----------------------------
	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_view);
	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_view, &m_eye, &m_at, &m_vecU);
	// �r���[�}�g���b�N�X�̐ݒ�
	m_device->SetTransform(D3DTS_VIEW, &m_view);

	//----------------------------
	// �v���W�F�N�V�����}�g���b�N�X
	//----------------------------
	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_projection);
	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_projection,
								_radian(45),	// ����p
								_aspect,		// �A�X�y�N�g��
								10.0f,			// near�l
								10000.0f);		// far�l
	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	//m_device->SetTransform(D3DTS_PROJECTION, &m_projection);

/*#ifdef _DEBUG
	// �J�������\��
	m_debugproc->PrintDebugProc("***�J�������**********\nPos x:%f y:%f z:%f\nAt  x:%f y:%f z:%f\nRot x:%f y:%f z:%f\n",
								m_eye.x, m_eye.y, m_eye.z,
								m_at.x,  m_at.y,  m_at.z,
								m_rot.x, m_rot.y, m_rot.z);
#endif*/
}

//=============================================================================
// �e�J�����̐ݒ�i�r���[�ƃv���W�F�N�V�����j
//=============================================================================
void Camera::SetShadowCamera(D3DXVECTOR3 targetPos)
{
	D3DXMATRIX	 view,projection;
	D3DVIEWPORT9 viewport;
	D3DXVECTOR3	 at	 = targetPos;
	D3DXVECTOR3	 eye = at + D3DXVECTOR3(1.0f, 3.0f, -2.0f);
	D3DXVECTOR3	 up	 = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//----------------------------
	// �r���[�}�g���b�N�X
	//----------------------------
	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&view);
	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&view, &eye, &at, &up);
	// �r���[�}�g���b�N�X�̐ݒ�
	//m_device->SetTransform(D3DTS_VIEW, &view);

	//----------------------------
	// �v���W�F�N�V�����}�g���b�N�X
	//----------------------------
	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&projection);
		// �v���W�F�N�V�����}�g���b�N�X�̍쐬
		D3DXMatrixOrthoLH(&projection,
							100.0f,	// width
							100.0f,	// height
							-50.0f,	// near�l
							50.0f);	// far�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	//m_device->SetTransform(D3DTS_PROJECTION, &projection);
		/*if(cnt == Shader::PAT_2D)
		{
			// 2D�p�v���W�F�N�V����
			//D3DXMATRIX	proj2D(2/SCREEN_WIDTH,	0.0f,			 0.0f, 0.0f,
			//					0.0f,			2/SCREEN_HEIGHT, 0.0f, 0.0f,
			//					0.0f,			0.0f,			 1.0f, 0.0f,
			//					0.0f,			0.0f,			 0.0f, 1.0f);
			D3DXMATRIX	proj2D(1.0f,	0.0f, 0.0f, 0.0f,
								0.0f,	1.0f, 0.0f, 0.0f,
								0.0f,	0.0f, 1.0f, 0.0f,
								0.0f,	0.0f, 0.0f, 1.0f);
			vsc->SetMatrix(m_device, "gProj", &proj2D);
		}*/

	//----------------------------
	// �r���[�|�[�g
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

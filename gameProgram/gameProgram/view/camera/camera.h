//*****************************************************************************
//
// �J���� [camera.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_CAMERA_H_
#define MY_CAMERA_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Debugproc;

class Camera
{
public:
	Camera(void);
	virtual ~Camera(void);

	static bool Create(Camera** outPointer, LPDIRECT3DDEVICE9 device, D3DXVECTOR3 at, D3DXVECTOR3 eye);
	bool Initialize(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 at, D3DXVECTOR3 eye);
	void Finalize(void);
	void Update(void);

	void SetCamera(void);
	void SetShadowCamera(D3DXVECTOR3 targetPos);

	void debugproc(Debugproc* debugproc) {m_debugproc = debugproc;}

	void		eye(D3DXVECTOR3 eye) {m_eye = eye;}
	void		eye(float x, float y, float z) {m_eye = D3DXVECTOR3(x, y, z);}
	void		eye_x(float x) {m_eye.x = x;}
	void		eye_y(float y) {m_eye.y = y;}
	void		eye_z(float z) {m_eye.z = z;}
	D3DXVECTOR3	eye(void) {return m_eye;}

	void		at(D3DXVECTOR3 at) {m_at = at;}
	void		at(float x, float y, float z) {m_at = D3DXVECTOR3(x, y, z);}
	void		at_x(float x) {m_at.x = x;}
	void		at_y(float y) {m_at.y = y;}
	void		at_z(float z) {m_at.z = z;}
	D3DXVECTOR3	at(void) {return m_at;}

	void		rot(D3DXVECTOR3 rot) {m_rot = rot;}
	void		rot(float x, float y, float z) {m_rot = D3DXVECTOR3(x, y, z);}
	void		rot_x(float x) {m_rot.x = x;}
	void		rot_y(float y) {m_rot.y = y;}
	void		rot_z(float z) {m_rot.z = z;}
	D3DXVECTOR3	rot(void) {return m_rot;}

	void	distance(float distance) {m_distance = distance;}
	float	distance(void) {return m_distance;}

	D3DXMATRIX view(void) {return m_view;}
	D3DXMATRIX viewProjection(void) {return (m_view * m_projection);}

private:
	LPDIRECT3DDEVICE9	m_device;
	Debugproc*			m_debugproc;

	D3DXMATRIX	 m_view;		// �r���[�}�g���b�N�X
	D3DXMATRIX	 m_projection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DVIEWPORT9 m_viewport;	// �r���[�|�[�g

	D3DXVECTOR3	m_eye;		// ���_
	D3DXVECTOR3	m_at;		// �����_
	D3DXVECTOR3	m_rot;		// ����(��]�p)
	float		m_distance;	// ����(���_���璍���_�܂�)

	D3DXVECTOR3	m_vecU;	// ������x�N�g��
	D3DXVECTOR3	m_vecF;	// �O�����x�N�g��
	D3DXVECTOR3	m_vecR;	// �E�����x�N�g��
};

//=============================================================================
#endif

// EOF

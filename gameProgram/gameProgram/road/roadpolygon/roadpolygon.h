//*****************************************************************************
//
// �X���b�h�p�|���S�� [roadpolygon.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef ROAD_POLYGON_H_
#define ROAD_POLYGON_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class RoadPolygon
{
public:
	typedef enum
	{
		POLYGON_CRITERIA_LEFT = 0,
		POLYGON_CRITERIA_CENTER,
		POLYGON_CRITERIA_RIGHT
	}POLYGON_CRITERIA;
	RoadPolygon(void);
	virtual ~RoadPolygon(void);

	static bool Create(RoadPolygon** outPointer, LPDIRECT3DDEVICE9 device , LPDIRECT3DTEXTURE9 textue , D3DXVECTOR3 polygonSize , POLYGON_CRITERIA crit );
	bool Initialize(LPDIRECT3DDEVICE9 device , LPDIRECT3DTEXTURE9 texture , D3DXVECTOR3 polygonSize , POLYGON_CRITERIA crit );
	void Finalize(void);
	void Update(void);
	void Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc);
	D3DXVECTOR3 GetPosition( void ){return m_pos;}
	void SetPosition( float x, float y , float z){m_pos.x = x , m_pos.y = y , m_pos.z = z;}
	void SetPosition( D3DXVECTOR3 pos ){ m_pos = pos; }

	D3DXVECTOR3 GetRot( void ){ return m_rot; }
	void SetRot( D3DXVECTOR3 rot ){ m_rot = rot; }
	void SetRot( float x, float y , float z ){ m_rot.x = x , m_rot.y = y , m_rot.z = z; }
	D3DXCOLOR GetColor( void ){ return m_diffuse; }
	void SetColor( D3DCOLOR diff){m_diffuse = diff;}
	void SetColor( float r , float g , float b , float a ){m_diffuse = D3DXCOLOR( r,g,b,a );}

private:
	struct VERTEX
	{
		D3DXVECTOR3	position;
		D3DCOLOR	color;
		D3DXVECTOR2	uv;
	};
	VERTEX m_vtx[4];

	LPDIRECT3DDEVICE9	m_device;		// Device�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DTEXTURE9	m_tex;			//�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_scl;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_pos;
	D3DXCOLOR m_diffuse;
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	POLYGON_CRITERIA m_crit;

	void SetVertexPolygon( POLYGON_CRITERIA crit );
};

//=============================================================================
#endif

// EOF

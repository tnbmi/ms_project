//*****************************************************************************
//
// �C���X�^���V���O�r���{�[�h�N���X [instancingBillboard.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef INSTANCING_BILLBOARD_H_
#define INSTANCING_BILLBOARD_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\objectBase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class InstancingBillboard : public ObjectBase
{
public:
	InstancingBillboard(LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority, OBJECT_TYPE type );
	virtual ~InstancingBillboard(void);

	static bool Create(InstancingBillboard** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority  , OBJECT_TYPE type );
	static bool Create(InstancingBillboard** outPointer, LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority  , OBJECT_TYPE type,const int polygonMax,const char *texFilePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2&oneSize );
	bool Initialize(void);
	bool Initialize( const int polygonMax,const char *texFilePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize );
	void Finalize(void);
	void Update(void);
	void Draw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp);

	//�C���X�^���V���O���Ɏg���\����
	typedef struct POLYGONDATA
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 scl;
		D3DXCOLOR col;
		D3DXVECTOR2 uvOffset;
		float camRange;//�J�����Ƃ̋����\�[�g�Ɏg��
		bool isDelete;
	};

	//���Z�����Ƃ�
	typedef struct OPTION
	{
		bool is_additiveSynthesis;//���Z�����t���O
		bool is_zsort;
		bool is_zwrite;

		OPTION(){}
		OPTION( bool isAdd,bool isZSort,bool isZWrite){ is_additiveSynthesis = isAdd;is_zsort = isZSort;is_zwrite = isZWrite; }
	};

		void SetOption( const OPTION opt ){ m_option = opt; }

		//�z�񂩂�C���X�^���V���O�p�����|�C���^�ő݂��o��
		//PolygonInfo��M�邱�ƂŃ|���S����`�悳���
		POLYGONDATA *LentInsPolygonData();

		//�e�N�X�`���ă��[�h
		void LoadTexture( const char *FilePath );

		//Uv���Đݒ肷��@�o�b�t�@��Lock����̂Œ���
		void SetVtxBuffTexInfo( const D3DXVECTOR2 &texSize,const D3DXVECTOR2 oneSize );


		void SetCameraPos( const D3DXVECTOR3 camPos ){ m_cameraPos = camPos; }
		int GetPolygonMax(){ return m_polygonMax; }

private:

	//�V�F�[�_�[ ����������
	IDirect3DVertexShader9 *m_vertex_shader;
	IDirect3DPixelShader9  *m_pixel_shader;
	LPD3DXCONSTANTTABLE m_vs_constant_table;
	LPD3DXCONSTANTTABLE m_ps_constant_table;
	LPDIRECT3DVERTEXDECLARATION9 m_decl;


	//�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;//�|���p
	LPDIRECT3DVERTEXBUFFER9 m_instancingBuff;//�C���X�^���V���O�p
	LPDIRECT3DINDEXBUFFER9 m_idxBuff;//�C���X�^���V���O�ɕK�{�Ȃ̂Ŏd���Ȃ�
		

	int m_polygonMax;//�C���X�^���V���O�ŕ\���ł���ő吔
	int m_usingPolygonSum;//���ݕ\�����Ă���|���S����
	int m_vtxSize;//���_�`���̃T�C�Y

	//�e�N�X�`��
	D3DXVECTOR2 m_texSize;//�e�N�X�`�����̂̃T�C�Y
	D3DXVECTOR2 m_oneSize;//�e�N�X�`���V�[�g�̐؂���T�C�Y

	typedef struct VTXBILL
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;
	};

	//�C���X�^���V���O�o�b�t�@�̌`��(�J���[��D3DXVECTOR4�Ȃ̂͂Ȃ���D3DXCOLOR���ƃ_������������j
	typedef struct INSTANCINGDATA
	{
		D3DXMATRIX mtx;
		D3DXVECTOR2 uv_offset;
		D3DXVECTOR4 col;
	};

	
	//�C���X�^���V���O�p���z��
	POLYGONDATA **m_insArray;
	int m_cursol;//�z��̑ݏo�J�[�\���@�݂��o�����ʒu��ۑ����Ă��̎����猟��������Α����͌����R�X�g���팸�ł���͂�

	//Option
	OPTION m_option;

	//�J�����ʒu
	D3DXVECTOR3 m_cameraPos;

	//�C���X�^���V���O�p�o�b�t�@�ɕύX��K�p����
	void ApplyInstancingBuff();

	//�e�Ή��V�F�[�_�[�̓ǂݍ���
	void LoadShaderBill();

	//�e���_�o�b�t�@�̍쐬
	bool CreateVtxBuffBill();

	//�C���X�^���V���O�o�b�t�@�쐬
	bool CreateInstancingBuff();

	//�C���f�b�N�X�o�b�t�@�̍쐬
	//�ꌩ����Ȃ��悤�Ɍ����邪�A�C���X�^���V���O��DrawIndexedPrimitive�̂ݑΉ��Ȃ̂ł���[�Ȃ�
	bool CreateIdxBuff();

	//���Z�����Ȃǂ̃I�v�V������K������
	void ApplyOption();

};

//=============================================================================
#endif

// EOF
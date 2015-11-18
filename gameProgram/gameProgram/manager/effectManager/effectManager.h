//*****************************************************************************
//
// �G�t�F�N�g�}�l�[�W�� [effectManager.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef EFFECTMANAGER_H_
#define EFFECTMANAGER_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\..\objectBase\instancingBillboard\instancingBillboard.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class EffectManager
{
public:
	EffectManager(InstancingBillboard *insBill);
	virtual ~EffectManager(void);

	static bool Create(EffectManager** outPointer,InstancingBillboard *insBill );
	bool Initialize(void);
	void Finalize(void);
	void Update(void);

	//�⊮�`��
	typedef enum COMPLETIONMODE
	{
		COMP_MODE_LINE,
		COMP_MODE_EASEIN,
		COMP_MODE_EASEOUT,
		COMP_MODE_CUBE,
		kCompMax

	}COMPLETIONMODE;

	
	typedef struct KEY
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 scl;
		D3DXVECTOR4 col;
	};

	typedef struct ANIMATION
	{
		int animSum;
		D3DXVECTOR2 startUvOffset;
	};

	//�G�t�F�N�g�\����
	typedef struct PARTICLE
	{
		KEY key[2];//�L�[�t���[�����ɂ��悤���Ǝv�������ǂ߂�ǂ������̂�2�_�Ԃ̕⊮�ł�����
		int compFrame;
		ANIMATION anim;//�A�j���[�V��������Ȃ����
		int animFrame;

		int compErase;
		float animErase;

		float lifeSpan;//���̂Ƃ���⊮�t���[���Ǝ����͓���
		COMPLETIONMODE mode;

		InstancingBillboard::POLYGONDATA *polygon;//�`��p
		bool isDelete;
	};

	//������G�t�F�N�g�t�@�C���̃f�[�^�ǂݍ��ݐ�
	struct EFFECTDATABASE
	{
		PARTICLE *particleArray;
		int particleSum;
	};


	//�G�t�F�N�g�̒ǉ��@�󂫂Ȃ���Ζ��������
	//CompFrame �⊮�t���[����
	//Life�@���@���݂͕⊮�t���[�����Ɠ����ɂ��Ă����Ă�������
	//AnimFrame ���A�j���[�V�����t���[����
	//Offset UvOffset�ʒu
	//CompMode �⊮���[�h
	void AddParticle( const D3DXVECTOR3 &posSt,const D3DXVECTOR3 &sclSt,const D3DXVECTOR4 &colSt,
					const D3DXVECTOR3 &posEd,const D3DXVECTOR3 &sclEd,const D3DXVECTOR4 &colEd,
					const int compFrame,const int life,const int animSum,const int animFrame,const int offsetX,const int offsetY,
					const COMPLETIONMODE compMode );

	void SetDevice( LPDIRECT3DDEVICE9 Device ){ m_device = Device; }

	void LoadTexture( const char *filePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize );
	void SetTexInfo( const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize );
	void SetCamPos( const D3DXVECTOR3 &camPos );

	//�G�t�F�N�g�f�[�^��ǂݍ���Ńf�[�^�Ƃ��ĕۑ�����
	void LoadEffectData( const char *filePath );

	//�ǂݍ��񂾃f�[�^����G�t�F�N�g���Ăяo���@Idx�̓��[�h��(0����j
	//�����̈ʒu�͏o���ʒu
	void AddEffectFromDataBase( const int idx,const D3DXVECTOR3 pos );

	//���Z�������̃I�v�V����
	void SetOption( InstancingBillboard::OPTION option );

	void SetViewMtx( const D3DXMATRIX &viewMtx );

private:

	LPDIRECT3DDEVICE9 m_device;

	InstancingBillboard *m_insBill;
	PARTICLE *m_particleArray;
	int m_particleMax;
	int m_cursol;

	//�⊮
	D3DXVECTOR3 Lerp( const D3DXVECTOR3 &st,const D3DXVECTOR3 &ed,float min,float max,float elapsed,float (*func)(float) );
	D3DXVECTOR4 Lerp( const D3DXVECTOR4 &st,const D3DXVECTOR4 &ed,float min,float max,float elapsed,float (*func)(float) );

	//�G�t�F�N�g�f�[�^�x�[�X
	const static int kEffectDataBaseMax = 10;
	int m_databaseCursol;//�f�[�^������̃J�[�\��
	EFFECTDATABASE m_effectDatabase[ kEffectDataBaseMax ];
};

//=============================================================================
#endif

// EOF
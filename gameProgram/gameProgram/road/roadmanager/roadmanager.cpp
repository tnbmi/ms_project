//*****************************************************************************
//
// �\�[�X�̃e���v���[�g [roadmanager.cpp]
// Author : KAZUMA OOIGAWA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "roadmanager.h"
#include "..\roadpolygon\roadpolygon.h"
#include "..\..\import\road\roadImport.h"
#include "..\..\shader\shader.h"
#include "..\..\view\camera\camera.h"
#include "..\..\phase\phase.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �萔��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const float _changeColorSpeed = 0.2f;
const int _checkMax = 4;

const D3DXVECTOR3 _at	= D3DXVECTOR3(0.0f, 100.0f, 0.0f);
const D3DXVECTOR3 _eye	= D3DXVECTOR3(0.0f, 150.0f, -3000.0f);

// ���萔
	const D3DXVECTOR3 _husumaSize = D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT , 0.0f );
	const float _husumaSpeed = 5.0f;
	// ��Ԓ萔
	const D3DXVECTOR3 _husumaCloseLeftInitPos = D3DXVECTOR3( SCREEN_WIDTH * 0.5f - _husumaSize.x * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f);
		const D3DXVECTOR3 _husumaCloseRightInitPos = D3DXVECTOR3( SCREEN_WIDTH * 0.5f + _husumaSize.x * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f);
	// �J��Ԓ萔	   
		const D3DXVECTOR3 _husumaOpenLeftInitPos = D3DXVECTOR3( 0.0f - _husumaSize.x * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f);
		const D3DXVECTOR3 _husumaOpenRightInitPos = D3DXVECTOR3( SCREEN_WIDTH + _husumaSize.x * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f);
// �A�C�R���萔
const D3DXVECTOR3 _iconSize = D3DXVECTOR3( 144.0f , 144.0f , 0.0f );
const D3DXVECTOR3 _iconInitPos = D3DXVECTOR3( SCREEN_WIDTH - _iconSize.x  , SCREEN_HEIGHT - _iconSize.y , 0.0f );
const float _iconRotSpeed = 0.01f;
// �e�L�X�g�萔
const D3DXVECTOR3 _textSize = D3DXVECTOR3( 432.0f , 144.0f , 0.0f );
const D3DXVECTOR3 _textInitPos = D3DXVECTOR3( SCREEN_WIDTH - _iconSize.x - _textSize.x , SCREEN_HEIGHT - _textSize.y , 0.0f );
const float _textMoveLimit = _textInitPos.y + 10.0f;
const float _textSpeed = 0.1f;
const int _textSheetMax = 5;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �ÓI�ϐ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
bool RoadManager::m_closeFlag;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
RoadManager::RoadManager(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_textSpeed = _textSpeed;
	m_state = ROAD_STATE_NONE;
	m_shader = nullptr;
	m_closeFlag = false;

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
RoadManager::~RoadManager(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool RoadManager::Create(RoadManager** outPointer , LPDIRECT3DDEVICE9 device )
{
	RoadManager* pointer = new RoadManager();
	if(!pointer->Initialize( device ))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool RoadManager::Initialize(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// �R�����g
	//----------------------------
		// 3D���_�錾
	D3DVERTEXELEMENT9 velement3D[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},		// ���W
		{0, 12, D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},		// �@��
		{0, 24, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,		0},		// �F
		{0, 28, D3DDECLTYPE_FLOAT2,		D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},		// �e�N�X�`��
		D3DDECL_END(),
	};

	device->CreateVertexDeclaration(&velement3D[0], &m_decl3D);



	if(!Shader::Create( &m_shader , device ) )
		return false;
	if(!RoadImport::Create( &m_import , device ))
		return false;

	if(!RoadPolygon::Create(&m_fadeLeft , device , m_import->texture(RoadImport::FADE_LEFT) , _husumaSize , RoadPolygon::POLYGON_CRITERIA_RIGHT))
		return false;
	if(!RoadPolygon::Create(&m_fadeRight , device , m_import->texture(RoadImport::FADE_RIGHT) , _husumaSize ,RoadPolygon::POLYGON_CRITERIA_LEFT))
		return false;
	if(!RoadPolygon::Create(&m_icon , device , m_import->texture(RoadImport::ICON) , _iconSize , RoadPolygon::POLYGON_CRITERIA_CENTER))
		return false;
	if(!RoadPolygon::Create(&m_text , device , m_import->texture(RoadImport::TEXT) , _textSize , RoadPolygon::POLYGON_CRITERIA_CENTER))
		return false;
	// �ʒu������
	InitializeClosePos();

	m_device = device;
	return true;
}

//=============================================================================
// �I��
//=============================================================================
void RoadManager::Finalize(void)
{

	if( m_decl3D != NULL )
	{
		m_decl3D->Release();
		m_decl3D = NULL;
	}

	m_fadeLeft->Finalize();
	m_fadeRight->Finalize();
	m_icon->Finalize();
	m_text->Finalize();
	m_import->Finalize();
	delete m_import;
	m_shader->Finalize();
	delete m_shader;
	delete this;
}

//=============================================================================
// �X�V
//=============================================================================
void RoadManager::Update(void)
{
	bool endFlag = false;
	switch( m_state )
	{
	case ROAD_STATE_CLOSE:
		Close();
		break;
	case ROAD_STATE_OPEN:
		Open();
		break;
	case ROAD_STATE_NONE:
		break;
	}
}

//=============================================================================
// �`��
//=============================================================================
void RoadManager::Draw(void)
{

	if( m_state != ROAD_STATE_NONE )
	{
	m_device->Clear(0, NULL,
					(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
					D3DCOLOR_RGBA(64, 64, 128, 256), 1.0f, 0);

		LPD3DXCONSTANTTABLE vsc =m_shader->vsc(Shader::PAT_2D);
		LPD3DXCONSTANTTABLE psc =m_shader->psc(Shader::PAT_2D);
		// �V�F�[�_�[���Z�b�g
		m_shader->SetShader(&vsc, &psc, Shader::PAT_2D);

		//----------------------------
		// Direct3D�ɂ��`��
		//----------------------------
		if(SUCCEEDED(m_device->BeginScene()))	//�J�n
		{
			
			// 2D���_�ݒ�
			m_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

			m_fadeLeft->Draw(vsc,psc);
			m_fadeRight->Draw(vsc,psc);
			m_icon->Draw(vsc,psc);
			m_text->Draw(vsc,psc);
			m_device->EndScene();	// �I��
		}
		m_device->Present(nullptr, nullptr, nullptr, nullptr);

	}
}
void RoadManager::Draw(Phase* phase)
{

	if( m_state != ROAD_STATE_NONE )
	{

		//----------------------------
		// Direct3D�ɂ��`��
		//----------------------------
		if(SUCCEEDED(m_device->BeginScene()))	//�J�n
		{
			m_device->Clear(0, NULL,
							(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
							D3DCOLOR_RGBA(64, 64, 128, 256), 1.0f, 0);

			m_device->SetVertexDeclaration(m_decl3D);
			phase->Draw();

			LPD3DXCONSTANTTABLE vsc =m_shader->vsc(Shader::PAT_2D);
			LPD3DXCONSTANTTABLE psc =m_shader->psc(Shader::PAT_2D);
			// �V�F�[�_�[���Z�b�g
			m_shader->SetShader(&vsc, &psc, Shader::PAT_2D);

			//----------------------------
			// Direct3D�ɂ��`��
			//----------------------------
			// 2D���_�ݒ�
			m_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

			m_fadeLeft->Draw(vsc,psc);
			m_fadeRight->Draw(vsc,psc);
			m_icon->Draw(vsc,psc);
			m_text->Draw(vsc,psc);
			m_device->EndScene();	// �I��
		}
		m_device->Present(nullptr, nullptr, nullptr, nullptr);

	}
}
//=============================================================================
// �|���S���ʒu������
//=============================================================================
void RoadManager::InitializeOpenPos( void )
{
	m_closeFlag = false;
	m_fadeLeft->SetPosition( _husumaOpenLeftInitPos );
	m_fadeRight->SetPosition( _husumaOpenRightInitPos );
	m_icon->SetPosition( _iconInitPos );
	m_icon->SetRot(0.0f,0.0f,0.0f );
	m_icon->SetColor( 1.0f , 1.0f , 1.0f , 1.0f );
	m_text->SetPosition( _textInitPos );
	m_text->SetColor( 1.0f , 1.0f , 1.0f , 1.0f );
}
//=============================================================================
// �|���S���ʒu������
//=============================================================================
void RoadManager::InitializeClosePos( void )
{
	m_closeFlag = true;
	m_fadeLeft->SetPosition( _husumaCloseLeftInitPos );
	m_fadeRight->SetPosition( _husumaCloseRightInitPos );
	m_icon->SetPosition( _iconInitPos );
	m_icon->SetRot( D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) );
	m_icon->SetColor( 1.0f , 1.0f , 1.0f , 1.0f );
	m_text->SetPosition( _textInitPos );
	m_text->SetColor( 1.0f , 1.0f , 1.0f , 1.0f );
	m_text->SetUvY( 0.2f , 1 );
}
//=============================================================================
// ���[�f�B���O��ʉ��J����
//=============================================================================
void RoadManager::Open( void )
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXCOLOR diff;
	int checkNum = 0;//�N���[�Y�ɑJ�ڂł��邩�̃`�F�b�N����
	// �����X�V

	pos = m_fadeLeft->GetPosition();
	if( pos.x > ( 0.0f - _husumaSize.x * 0.5f ) )
	{
		pos.x-=_husumaSpeed;
	}
	else
	{
		checkNum++;
	}
	m_fadeLeft->SetPosition( pos );

	m_fadeLeft->Update();

	// ���E�X�V

	pos = m_fadeRight->GetPosition();
	if( pos.x < ( SCREEN_WIDTH + _husumaSize.x * 0.5f ) )
	{
		pos.x+=_husumaSpeed;
	}
	else
	{
		checkNum++;
	}
	m_fadeRight->SetPosition( pos );

	m_fadeRight->Update();

	// �A�C�R���X�V

	rot = m_icon->GetRot();
	rot.z+=_iconRotSpeed;
	m_icon->SetRot(rot);

	diff = m_icon->GetColor();
	if( diff.a > 0.0f )
	{
		diff.a -=_changeColorSpeed;
		m_icon->SetColor( diff );
	}
	else
	{
		checkNum++;
	}

	m_icon->Update();

	//�e�L�X�g�X�V

	float sheetNum = m_text->GetSheetNum();
	if( sheetNum > _textSheetMax )
	{
		sheetNum = 0;
	}
	else
	{
		sheetNum+=0.01f;
	}
	if( (int)sheetNum % 1 == 0.0f )
	{
		m_text->SetUvX( 0.2f , sheetNum );
	}
	m_text->SetSheetNum( sheetNum );
	m_text->SetPosition( pos );
	diff = m_text->GetColor();
	if( diff.a > 0.0f )
	{
		diff.a -=_changeColorSpeed;
		m_text->SetColor(diff);
	}
	else
	{
		checkNum++;
	}
	m_text->Update();
	if( checkNum == _checkMax )
	{
		m_state = ROAD_STATE_NONE;
		InitializeOpenPos();
	}
}
//=============================================================================
// ���[�f�B���O��ʉ�����
//=============================================================================
void RoadManager::Close( void )
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXCOLOR diff;
	int closepoint = 0;//�O�t�F�[�Y�̕`���؂��Ă������̃`�F�b�N����
	// �����X�V
	pos = m_fadeLeft->GetPosition();
	if( pos.x < _husumaCloseLeftInitPos.x )
	{
		pos.x+=_husumaSpeed;
	}
	else
	{
		closepoint+=1;
	}
	m_fadeLeft->SetPosition( pos );
	m_fadeLeft->Update();

	// ���E�X�V
	pos = m_fadeRight->GetPosition();
	if( pos.x > _husumaCloseRightInitPos.x )
	{
		pos.x-=_husumaSpeed;
	}
	else
	{
		closepoint+=1;
	}

	if( closepoint == 2 )
	{
		m_closeFlag = true;
	}
	m_fadeRight->SetPosition( pos );
	m_fadeRight->Update();

	// �A�C�R���X�V
	rot = m_icon->GetRot();
	rot.z+=_iconRotSpeed;
	m_icon->SetRot(rot);
	diff = m_icon->GetColor();
	if( diff.a < 1.0f )
	{
		diff.a += _changeColorSpeed;
	}
	m_icon->Update();
	//�e�L�X�g�X�V

	pos = m_text->GetPosition();

	float sheetNum = m_text->GetSheetNum();
	if( sheetNum > _textSheetMax )
	{
		sheetNum = 0;
	}
	else
	{
		sheetNum+=0.01f;
	}
	if( (int)sheetNum % 1 == 0.0f )
	{
		m_text->SetUvX( 0.2f , sheetNum );
	}
	m_text->SetSheetNum( sheetNum );

	m_text->SetPosition( pos );
	diff = m_text->GetColor();
	if( diff.a < 1.0f )
	{
		diff.a += _changeColorSpeed;
	}
	m_text->Update();
}
// EOF

//*****************************************************************************
//
// �`��@�\ [renderer.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "renderer.h"
#include "..\common\safe.h"

#include "..\debugproc\debugproc.h"
#include "..\phase\phase.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Renderer::Renderer(void)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	m_device	= nullptr;
	m_debugproc	= nullptr;
	m_phase		= nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Renderer::~Renderer(void)
{
}

//=============================================================================
// ����
//=============================================================================
bool Renderer::Create(Renderer** outPointer, HWND hWnd, bool windowFlg)
{
	Renderer* pointer = new Renderer();
	if(!pointer->Initialize(hWnd, windowFlg))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// ������
//=============================================================================
bool Renderer::Initialize(HWND hWnd, bool windowFlg)
{
	HRESULT					hResult;		// ����������
	LPDIRECT3D9				object;			// Direct3D�I�u�W�F�N�g
	D3DDISPLAYMODE			displayMode;	// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS	presenParam;	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^

	//----------------------------
	// Direct3D�I�u�W�F�N�g�̍쐬
	//----------------------------
	object = Direct3DCreate9(D3D_SDK_VERSION);
	if(object == nullptr)
		return false;

	//----------------------------
	// ���݂̃f�B�X�v���C���[�h���擾
	//----------------------------
	hResult = object->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
	if(FAILED(hResult))
	{
		MessageBox(NULL, "Renderer", "�f�B�X�v���C���[�h�擾���ɃG���[���������܂����B", MB_OK);
		return false;
	}

	//----------------------------
	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	//----------------------------
	ZeroMemory(&presenParam, sizeof(presenParam));					// ���[�N���[���N���A
	presenParam.BackBufferCount			= 1;						// �o�b�N�o�b�t�@�̐�
	presenParam.BackBufferWidth			= SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	presenParam.BackBufferHeight		= SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	presenParam.BackBufferFormat		= displayMode.Format;		// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	presenParam.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	presenParam.Windowed				= windowFlg;				// �E�B���h�E���[�h
	presenParam.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	presenParam.AutoDepthStencilFormat	= D3DFMT_D24S8;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��(���i����D24S8)

	//----------------------------
	// �E�B���h�E����
	//----------------------------
	if(windowFlg)
	{// �E�B���h�E���[�h
		presenParam.FullScreen_RefreshRateInHz	= 0;								// ���t���b�V�����[�g
		presenParam.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		presenParam.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		presenParam.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	//----------------------------
	// �f�o�C�X�I�u�W�F�N�g����
	//----------------------------
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	hResult = object->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
									&presenParam,
									&m_device);
	if(FAILED(hResult))
	{// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		hResult = object->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
										&presenParam,
										&m_device);
		if(FAILED(hResult))
		{// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			hResult = object->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
											&presenParam,
											&m_device);
			if(FAILED(hResult))
			{
				MessageBox(NULL, "Renderer", "�f�o�C�X�I�u�W�F�N�g�����Ɏ��s���܂����B", MB_OK);
				return false;
			}
		}
	}

	//----------------------------
	// Direct3D�I�u�W�F�N�g�J��
	//----------------------------
	SafeRelease(object);

	//----------------------------
	// �f�o�C�X�ݒ�
	//----------------------------
	// �����_�[�X�e�[�g�p�����[�^
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	m_device->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^
	m_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	m_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�
	
	// �e�N�X�`���X�e�[�W�X�e�[�g
	m_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O������ݒ�
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	// ���e�X�g���l
	m_device->SetRenderState(D3DRS_ALPHAREF, 0x80);					// �s�����ɂ���l�̐ݒ�
	m_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	// ��r���@

	return true;
}

//=============================================================================
// �I��
//=============================================================================
void Renderer::Finalize(void)
{
	//----------------------------
	// �f�o�C�X�I�u�W�F�N�g�̊J��
	//----------------------------
	SafeRelease(m_device);
}

//=============================================================================
// �X�V
//=============================================================================
void Renderer::Update(void)
{
	//----------------------------
	// �t�F�[�Y�`��
	//----------------------------
	m_phase->Update();
}

//=============================================================================
// �`��
//=============================================================================
void Renderer::Draw(void)
{
	//----------------------------
	// Direct3D�ɂ��`��
	//----------------------------
	if(SUCCEEDED(m_device->BeginScene()))	//�J�n
	{
		//----------------------------
		// �t�F�[�Y�`��
		//----------------------------
		m_phase->Draw();

#ifdef _DEBUG
		//----------------------------
		// �f�o�b�O�`��
		//----------------------------
		m_debugproc->Draw();
#endif

		m_device->EndScene();	// �I��
	}

	//----------------------------
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	//----------------------------
	m_device->Present(nullptr, nullptr, nullptr, nullptr);
}

// [EOF]

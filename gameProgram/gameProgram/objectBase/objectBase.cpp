//*****************************************************************************
//
// �I�u�W�F�N�g�̊�� [objectBase.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "objectBase.h"

#include "..\list\objectList\objectList.h"
#include "..\list\updateList\updateList.h"
#include "..\list\drawList\drawList.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
ObjectBase::ObjectBase(LPDIRECT3DDEVICE9 device, ObjectList* objectList, int priority, OBJECT_TYPE type)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	// protected
	m_device = device;

	D3DXMatrixIdentity(&m_world);
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_texture = nullptr;

	//private
	m_priority	 = priority;
	m_type		 = type;
	m_deleteFlg	 = false;

	// �|�C���^�[
	m_objectList = objectList;
	m_objectPrev = nullptr;
	m_objectNext = nullptr;

	m_updateList = nullptr;
	m_updatePrev = nullptr;
	m_updateNext = nullptr;

	for(int cnt = 0; cnt < Shader::PATTERN_MAX; ++cnt)
	{
		m_drawList[cnt] = nullptr;
		m_drawPrev[cnt] = nullptr;
		m_drawNext[cnt] = nullptr;
	}

	//----------------------------
	// �I�u�W�F�N�g���X�g�փ����N
	//----------------------------
	m_objectList->Link(this);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
ObjectBase::~ObjectBase(void)
{
	// �`�惊�X�g���O
	for(int cnt = 0; cnt < Shader::PATTERN_MAX; ++cnt)
		if(m_drawList[cnt] != nullptr)
			m_drawList[cnt]->UnLink(this);

	// �X�V���X�g���O
	if(m_updateList)
		m_updateList->UnLink(this);

	// �I�u�W�F�N�g���X�g���O
	m_objectList->UnLink(this);
}

// EOF

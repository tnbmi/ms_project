//*****************************************************************************
//
// �`��Ǘ����X�g [drawList.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "drawList.h"
#include "..\..\common\safe.h"

#include "..\..\objectBase\objectBase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �ÓI�ϐ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int DrawList::m_priorityLevel = 5;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
DrawList::DrawList(Shader::PATTERN shaderPatternId) : List(m_priorityLevel)
{
	//----------------------------
	// �����o�[������
	//----------------------------
	for(int cnt = 0; cnt < m_priorityLevel; ++cnt)
	{
		m_top[cnt] = nullptr;
		m_end[cnt] = nullptr;
	}

	m_shaderPatternId = shaderPatternId;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
DrawList::~DrawList(void)
{
	List::~List();
}

//=============================================================================
// ����
//=============================================================================
bool DrawList::Create(DrawList** outPointer, Shader::PATTERN shaderPatternId)
{
	DrawList* pointer = new DrawList(shaderPatternId);

	*outPointer = pointer;
	return true;
}

//=============================================================================
// �����N
//=============================================================================
void DrawList::Link(ObjectBase* object, int priority)
{
	//----------------------------
	// �`�惊�X�g��ݒ�
	//----------------------------
	object->drawList(this, m_shaderPatternId);

	//----------------------------
	// �O�I�u�W�F�N�g�̐ݒ�
	//----------------------------
	if(m_top[priority] == nullptr)
	{
		// �擪�ɐݒ�
		m_top[priority] = object;
		object->drawPrev(m_shaderPatternId);
	}
	else
	{
		// �O�I�u�W�F�N�g����A��
		m_end[priority]->drawNext(object, m_shaderPatternId);
		object->drawPrev(m_end[priority], m_shaderPatternId);
	}

	//----------------------------
	// ���I�u�W�F�N�g�̐ݒ�
	//----------------------------
	object->drawNext(nullptr, m_shaderPatternId);

	//----------------------------
	// �I�[�I�u�W�F�N�g�̐ݒ�
	//----------------------------
	m_end[priority] = object;
}

//=============================================================================
// �����N����
//=============================================================================
void DrawList::UnLink(ObjectBase* object)
{
	ObjectBase*	prev	 = object->drawPrev(m_shaderPatternId);
	ObjectBase*	next	 = object->drawNext(m_shaderPatternId);
	int			priority = object->priority();

	//----------------------------
	// �O�I�u�W�F�N�g�̍Đݒ�
	//----------------------------
	if(prev != nullptr)
	{
		// �O�I�u�W�F�N�g�Ɏ��I�u�W�F�N�g��A��
		prev->drawNext(next, m_shaderPatternId);
	}
	else
	{
		// �擪�I�u�W�F�N�g��ύX
		m_top[priority] = next;

		if(m_top[priority] != nullptr)
			// �擪�̑O��nullptr��
			m_top[priority]->drawPrev(nullptr, m_shaderPatternId);
	}

	//----------------------------
	// ���I�u�W�F�N�g�̍Đݒ�
	//----------------------------
	if(next != nullptr)
	{
		// ���I�u�W�F�N�g�ɑO�I�u�W�F�N�g��A��
		next->drawPrev(prev, m_shaderPatternId);
	}
	else
	{
		// �I�[�I�u�W�F�N�g��ύX
		m_end[priority] = prev;

		if(m_end != nullptr)
			// �I�[��nullptr��
			m_end[priority]->drawNext(nullptr, m_shaderPatternId);
	}

	//----------------------------
	// �`�惊�X�g������
	//----------------------------
	object->drawList(nullptr, m_shaderPatternId);
}

//=============================================================================
// �S�I�u�W�F�N�g�̕`��
//=============================================================================
void DrawList::AllDraw(LPD3DXCONSTANTTABLE vsc, LPD3DXCONSTANTTABLE psc, D3DXMATRIX vp)
{
	ObjectBase* cur = nullptr;

	for(int cnt = 0; cnt < m_priorityLevel; ++cnt)
	{
		cur = m_top[cnt];

		while(cur)
		{
			cur->Draw(vsc, psc, vp);

			cur = cur->drawNext(m_shaderPatternId);
		}
	}
}

// EOF

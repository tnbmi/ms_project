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
	object->priority(m_shaderPatternId, priority);

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
		m_end[priority]->drawNext(m_shaderPatternId, object);
		object->drawPrev(m_shaderPatternId, m_end[priority]);
	}

	//----------------------------
	// ���I�u�W�F�N�g�̐ݒ�
	//----------------------------
	object->drawNext(m_shaderPatternId, nullptr);

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
	// ���݃`�F�b�N
	if(object->drawList(m_shaderPatternId) == nullptr)
		return;

	ObjectBase*	prev	 = object->drawPrev(m_shaderPatternId);
	ObjectBase*	next	 = object->drawNext(m_shaderPatternId);
	int			priority = object->priority(m_shaderPatternId);

	//----------------------------
	// �O�I�u�W�F�N�g�̍Đݒ�
	//----------------------------
	object->drawPrev(m_shaderPatternId, nullptr);
	if(prev != nullptr)
	{
		// �O�I�u�W�F�N�g�Ɏ��I�u�W�F�N�g��A��
		prev->drawNext(m_shaderPatternId, next);
	}
	else
	{
		// �擪�I�u�W�F�N�g��ύX
		m_top[priority] = next;

		if(m_top[priority] != nullptr)
			// �擪�̑O��nullptr��
			m_top[priority]->drawPrev(m_shaderPatternId, nullptr);
	}

	//----------------------------
	// ���I�u�W�F�N�g�̍Đݒ�
	//----------------------------
	object->drawNext(m_shaderPatternId, nullptr);
	if(next != nullptr)
	{
		// ���I�u�W�F�N�g�ɑO�I�u�W�F�N�g��A��
		next->drawPrev(m_shaderPatternId, prev);
	}
	else
	{
		// �I�[�I�u�W�F�N�g��ύX
		m_end[priority] = prev;

		if(m_end[priority] != nullptr)
			// �I�[��nullptr��
			m_end[priority]->drawNext(m_shaderPatternId, nullptr);
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

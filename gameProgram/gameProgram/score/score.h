//*****************************************************************************
//
// スコア [score.h]
// Author : YUTO HARADA
//
//*****************************************************************************

#ifndef SCORE_H_
#define SCORE_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\main\main.h"
#include "..\objectBase\objectBase.h"
class DrawListManager;
class UpdateList;
class Number;
class ResultImport;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Score
{
public:
	Score(Number *number1,Number *number2,Number *number3,Number *number4);
	~Score(void);

	static bool Create(Score** outPointer,LPDIRECT3DDEVICE9 device, ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList, int priority  , ObjectBase::OBJECT_TYPE type,ResultImport *import);
	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

	void pos( D3DXVECTOR3 pos ){ m_pos = pos; }
	D3DXVECTOR3 pos(){ return m_pos; }

	void scl( D3DXVECTOR3 scl ){ m_scl = scl; }
	D3DXVECTOR3 scl(){ return m_scl; }

	void col( D3DXCOLOR col ){ m_col = col; }
	D3DXCOLOR col(){ return m_col; }

	void score( const int score ){ m_score = score; }
	int  score(){ return m_score; }

	void StartRandView(const int RandViewFrame );

	struct NUMBERBOX
	{
		int num[10];
	};

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_scl;
	D3DXCOLOR   m_col;
	static const int _numMax = 4;
	NUMBERBOX m_numberBoxArray[_numMax];
	int m_score;
	int m_randViewFrame;//じゃかじゃかするフレーム
	int m_time;//じゃかじゃか経過時間
	Number *m_number[_numMax];
};

//=============================================================================
#endif

// EOF
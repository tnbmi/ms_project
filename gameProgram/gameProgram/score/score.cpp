//*****************************************************************************
//
// ソースのテンプレート [Score.cpp]
// Author : MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "score.h"
#include "..\number\number.h"
#include "..\list\objectList\objectList.h"
#include "..\list\updateList\updateList.h"
#include "..\list\drawList\drawListManager.h"
#include "..\import\result\resultImport.h"


//=============================================================================
// コンストラクタ
//=============================================================================
Score::Score(Number *number1,Number *number2,Number *number3,Number *number4)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	m_number[0] = number1;
	m_number[0]->pos(500,100,0);
	m_number[0]->scl(100,100,0);
	m_number[0]->rot(0,0,0);

	m_number[1] = number2;
	m_number[1]->pos(600,100,0);
	m_number[1]->scl(100,100,0);
	m_number[1]->rot(0,0,0);

	m_number[2] = number3;
	m_number[2]->pos(700,100,0);
	m_number[2]->scl(100,100,0);
	m_number[2]->rot(0,0,0);

	m_number[3] = number4;
	m_number[3]->pos(800,100,0);
	m_number[3]->scl(100,100,0);
	m_number[3]->rot(0,0,0);
}

//=============================================================================
// デストラクタ
//=============================================================================
Score::~Score(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool Score::Create(Score** outPointer,LPDIRECT3DDEVICE9 device, ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList, int priority  , ObjectBase::OBJECT_TYPE type,ResultImport *import)
{
	Number *number[4];

	for( int i = 0 ; i < 4 ; i++ )
	{
		Number::Create( &number[i],device,objectList,import->texture(ResultImport::NUM),ObjectBase::TYPE_2D );

		updateList->Link( number[i] );
		drawList->Link(number[i],0,Shader::PAT_2D);
	}
	Score* pointer = new Score(number[0],number[1],number[2],number[3]);
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool Score::Initialize(void)
{
	//----------------------------
	// コメント
	//----------------------------
	m_numberBoxArray[0].num[0] = 1;
	m_numberBoxArray[0].num[1] = 3;
	m_numberBoxArray[0].num[2] = 2;
	m_numberBoxArray[0].num[3] = 4;
	m_numberBoxArray[0].num[4] = 6;
	m_numberBoxArray[0].num[5] = 8;
	m_numberBoxArray[0].num[6] = 9;
	m_numberBoxArray[0].num[7] = 0;
	m_numberBoxArray[0].num[8] = 5;
	m_numberBoxArray[0].num[9] = 7;

	m_numberBoxArray[1].num[0] = 4;
	m_numberBoxArray[1].num[1] = 1;
	m_numberBoxArray[1].num[2] = 3;
	m_numberBoxArray[1].num[3] = 2;
	m_numberBoxArray[1].num[4] = 5;
	m_numberBoxArray[1].num[5] = 7;
	m_numberBoxArray[1].num[6] = 8;
	m_numberBoxArray[1].num[7] = 0;
	m_numberBoxArray[1].num[8] = 9;
	m_numberBoxArray[1].num[9] = 6;

	m_numberBoxArray[2].num[0] = 0;
	m_numberBoxArray[2].num[1] = 1;
	m_numberBoxArray[2].num[2] = 3;
	m_numberBoxArray[2].num[3] = 5;
	m_numberBoxArray[2].num[4] = 7;
	m_numberBoxArray[2].num[5] = 6;
	m_numberBoxArray[2].num[6] = 4;
	m_numberBoxArray[2].num[7] = 9;
	m_numberBoxArray[2].num[8] = 2;
	m_numberBoxArray[2].num[9] = 8;

	m_numberBoxArray[3].num[0] = 9;
	m_numberBoxArray[3].num[1] = 0;
	m_numberBoxArray[3].num[2] = 2;
	m_numberBoxArray[3].num[3] = 5;
	m_numberBoxArray[3].num[4] = 4;
	m_numberBoxArray[3].num[5] = 8;
	m_numberBoxArray[3].num[6] = 1;
	m_numberBoxArray[3].num[7] = 3;
	m_numberBoxArray[3].num[8] = 2;
	m_numberBoxArray[3].num[9] = 5;

	m_time = 0;
	m_randViewFrame = 0;
	m_score =1234;

	m_scl = D3DXVECTOR3( 100,100,0);
	m_pos = D3DXVECTOR3(0,0,0);


	return true;
}

//=============================================================================
// 終了
//=============================================================================
void Score::Finalize(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void Score::Update(void)
{
	int score;

	if( m_time < m_randViewFrame )
	{
		int idx = m_time % 10;
		score = m_numberBoxArray[0].num[idx] * 1000 + m_numberBoxArray[1].num[idx] * 100 + m_numberBoxArray[2].num[idx] * 10 + m_numberBoxArray[3].num[idx];
		m_time++;
	}
	else
	{
		score = m_score;
	}

	int _1 = score%10 ;
	score /=10;
	int _10= score%10 ;
	score /=10;
	int _100 = score%10;
	score /=10;
	int _1000= score%10;
	score /=10;
	
	m_number[0]->pos( m_pos - D3DXVECTOR3((m_scl.x/2)*3,0,0 ));
	m_number[1]->pos( m_pos - D3DXVECTOR3((m_scl.x/2)*1,0,0 ));
	m_number[2]->pos( m_pos + D3DXVECTOR3((m_scl.x/2)*1,0,0 ));
	m_number[3]->pos( m_pos + D3DXVECTOR3((m_scl.x/2)*3,0,0 ));

	m_number[0]->scl( m_scl );
	m_number[1]->scl( m_scl );
	m_number[2]->scl( m_scl );
	m_number[3]->scl( m_scl );

	m_number[0]->color( m_col );
	m_number[1]->color( m_col );
	m_number[2]->color( m_col );
	m_number[3]->color( m_col );

	m_number[0]->SetNumber(_1000 );
	m_number[1]->SetNumber( _100 );
	m_number[2]->SetNumber( _10 );
	m_number[3]->SetNumber( _1);
	
}

//=============================================================================
// 描画
//=============================================================================
void Score::Draw(void)
{
}

//=============================================================================
// じゃかじゃかする
//=============================================================================

void Score::StartRandView( const int randViewFrame )
{
	m_time = 0;
	m_randViewFrame = randViewFrame;
}


bool Score::isRandView()
{
	if( m_time >= m_randViewFrame )
	{
		return false;
	}

	return true;
}

// EOF

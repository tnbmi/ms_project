//*****************************************************************************
//
// 何か降らせる [rainManager.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "rainManager.h"
#include "..\list\objectList\objectList.h"
#include "..\list\drawList\drawListManager.h"
#include "..\list\updateList\updateList.h"
#include "..\common\complement.h"
#include "..\objectBase\instancingBillboard\instancingBillboard.h"
//=============================================================================
// コンストラクタ
//=============================================================================
RainManager::RainManager(void)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
}

//=============================================================================
// デストラクタ
//=============================================================================
RainManager::~RainManager(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool RainManager::Create(RainManager** outPointer,LPDIRECT3DDEVICE9 device,ObjectList* objectList,UpdateList *updateList,DrawListManager *drawList,
						const int PolygonMax,const char *texFilePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize)
{
	//ビルボードライン生成
	InstancingBillboard *bill;
	if( !InstancingBillboard::Create( &bill,device,objectList,1,ObjectBase::TYPE_3D,PolygonMax,texFilePath,texSize,oneSize ) )
	{
		return false;
	}

	updateList->Link( bill );
	drawList->Link(bill,0,Shader::PAT_INS );
	RainManager* pointer = new RainManager();
	if(!pointer->Initialize(bill))
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool RainManager::Initialize(InstancingBillboard *bill)
{
	//----------------------------
	// コメント
	//----------------------------
	m_insBill = bill;

	m_insBill->SetOption( InstancingBillboard::OPTION( false,false,true ) );

	//生成できるビルボード数が最大パーティクル数となる
	m_particleMax = m_insBill->GetPolygonMax();

	//エフェクト配列生成
	m_rainDataArray = new RAIN_DATA[ m_particleMax ];

	for( int i = 0 ; i < m_particleMax ; i++ )
	{
		m_rainDataArray[ i ].isDelete = true;
	}

	for( int i = 0 ; i < _seedMax ; i++ )
	{
		m_seedDataArray[i].isUse = false;
	}

	m_cursol = 0;
	m_seedCursol =0;
	m_gravity = -0.01f;
	m_wind = D3DXVECTOR3(0.001f,0,0 );
	return true;
}

//=============================================================================
// 終了
//=============================================================================
void RainManager::Finalize(void)
{
	delete []m_rainDataArray;
}

//=============================================================================
// 更新
//=============================================================================
void RainManager::Update(void)
{
	//Seedから生成する
	for( int i = 0 ; i < _seedMax; i++ )
	{
		if( m_seedDataArray[i].isUse )
		{
			m_seedDataArray[i].frame++;

			if( m_seedDataArray[i].frame >= m_seedDataArray[i].createFrame )
			{
				m_seedDataArray[i].frame = 0.0f;

				RAIN_DATA data;
				int create = (int)RandRange( (float)m_seedDataArray[i].createSum[1],(float)m_seedDataArray[i].createSum[0] );
				for( int j = 0 ; j < create ; j++ )
				{
					int idx = LentRainData();

					if( idx < 0 )
					{
						break;
					}

					D3DXVECTOR3 pos;
					D3DXVECTOR3 force;
					float mass;

					pos.x = RandRange( m_seedDataArray[i].setPos[1].x,m_seedDataArray[i].setPos[0].x );
					pos.y = RandRange( m_seedDataArray[i].setPos[1].y,m_seedDataArray[i].setPos[0].y );
					pos.z = RandRange( m_seedDataArray[i].setPos[1].z,m_seedDataArray[i].setPos[0].z );
					force.x = RandRange( m_seedDataArray[i].force[1].x,m_seedDataArray[i].force[0].x );
					force.y = RandRange( m_seedDataArray[i].force[1].y,m_seedDataArray[i].force[0].y );
					force.z = RandRange( m_seedDataArray[i].force[1].z,m_seedDataArray[i].force[0].z );
					
					mass = RandRange( m_seedDataArray[i].mass[1],m_seedDataArray[i].mass[0] );

					m_rainDataArray[idx].pos = pos;
					m_rainDataArray[idx].force = force;
					m_rainDataArray[idx].mass = mass;

				}
				LentRainData();
			}
		}
	}

	//粒の更新
	for( int i = 0 ; i < m_particleMax ; i++ )
	{
		if( !m_rainDataArray[i].isDelete )
		{
			//重力
			m_rainDataArray[i].force.y += m_gravity;
			m_rainDataArray[i].force += m_wind;
			m_rainDataArray[i].acc += m_rainDataArray[i].force / m_rainDataArray[i].mass;

			m_rainDataArray[i].spd += m_rainDataArray[i].acc;
			m_rainDataArray[i].pos += m_rainDataArray[i].spd;

			m_rainDataArray[i].poly->pos = m_rainDataArray[i].pos;
			m_rainDataArray[i].poly->rot = D3DXVECTOR3(0,0,0);
			m_rainDataArray[i].poly->scl = D3DXVECTOR3(100,100,0);
			m_rainDataArray[i].poly->col = D3DXCOLOR(1,1,1,1);
			m_rainDataArray[i].poly->uvOffset = D3DXVECTOR2(0,0);

			if( m_rainDataArray[i].pos.y < 0 )
			{
				m_rainDataArray[i].isDelete = true;
				m_rainDataArray[i].poly->isDelete = true;
			}

		}
	}

}


int RainManager::LentRainData()
{
	int idx = -1;

	for( int i = 0 ; i < m_particleMax ; i++ )
	{
		if( m_rainDataArray[ m_cursol ].isDelete )
		{
			idx = m_cursol;
			break;
		}


		m_cursol++;

		if( m_cursol >= m_particleMax )
		{
			m_cursol = 0;
		}
	}

	if(idx == -1)
	{
		return idx;
	}

	m_rainDataArray[idx].acc = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_rainDataArray[idx].spd = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_rainDataArray[idx].isDelete = false;
	m_rainDataArray[idx].poly = m_insBill->LentInsPolygonData();

	return idx;
}

void RainManager::SetRainSeed(const int idx,const float createFrame,	const int createSumMin,const int createSumMax,
												const float massMin,const float massMax,
												const D3DXVECTOR3 &forceMin,const D3DXVECTOR3 &forceMax,
												const D3DXVECTOR3 &setPosMin,const D3DXVECTOR3 &setPosMax )
{
	if( idx < 0 || idx >= _seedMax)
	{
		return;
	}

	m_seedDataArray[ idx ].createFrame = createFrame;
	m_seedDataArray[ idx ].createSum[0] = createSumMin;
	m_seedDataArray[ idx ].createSum[1] = createSumMax;
	m_seedDataArray[ idx ].mass[0]      = massMin;
	m_seedDataArray[ idx ].mass[1]      = massMax;
	m_seedDataArray[ idx ].force[0]     = forceMin;
	m_seedDataArray[ idx ].force[1]     = forceMax;
	m_seedDataArray[ idx ].setPos[0]    = setPosMin;
	m_seedDataArray[ idx ].setPos[1]    = setPosMax;
	m_seedDataArray[ idx ].frame = 0.0f;

	m_seedDataArray[ idx ].isUse = true;

}

// EOF

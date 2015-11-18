//*****************************************************************************
//
// エフェクトマネージャクラス [effectManager.cpp]
// Author : YUTO HARADA
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "effectManager.h"
#include "..\..\objectBase\instancingBillboard\instancingBillboard.h"



//補完用
float Cube( float T ){ return T * T * ( 3.0f - 2.0f * T ); }
float Linear( float T ){ return T ; }
float EaseIn( float T ){return T*T;}
float EaseOut( float T ){ return T*(2-T);}

//=============================================================================
// コンストラクタ
//=============================================================================
EffectManager::EffectManager(InstancingBillboard *insBill)
{
	//----------------------------
	// メンバー初期化
	//----------------------------
	 m_insBill = insBill ;
}

//=============================================================================
// デストラクタ
//=============================================================================
EffectManager::~EffectManager(void)
{
}

//=============================================================================
// 生成
//=============================================================================
bool EffectManager::Create(EffectManager** outPointer,InstancingBillboard *insBill )
{
	EffectManager* pointer = new EffectManager(insBill);
	if(!pointer->Initialize())
		return false;

	*outPointer = pointer;
	return true;
}

//=============================================================================
// 初期化
//=============================================================================
bool EffectManager::Initialize(void)
{
	//----------------------------
	// コメント
	//----------------------------

	//生成できるビルボード数が最大パーティクル数となる
	m_particleMax = m_insBill->GetPolygonMax();

	m_databaseCursol = 0;

	//エフェクト配列生成
	m_particleArray = new PARTICLE[ m_particleMax ];

	for( int i = 0 ; i < m_particleMax ; i++ )
	{
		m_particleArray[ i ].isDelete = true;
	}

	m_cursol = 0;

	return true;
}

//=============================================================================
// 終了
//=============================================================================
void EffectManager::Finalize(void)
{
	if( m_particleArray != nullptr )
	{
		delete []m_particleArray;
		m_particleArray = nullptr;
	}

	for( int i = 0 ; i < m_databaseCursol ; i++ )
	{
		delete []m_effectDatabase[i].particleArray;
		m_databaseCursol = 0;
	}
}

//=============================================================================
// 更新
//=============================================================================
void EffectManager::Update(void)
{
	for( int i = 0 ; i < m_particleMax ; i++ )
	{
		//エフェクト死亡

		if( !m_particleArray[i].isDelete )
		{

			if( m_particleArray[i].lifeSpan <= 0 )
			{
				m_particleArray[i].polygon->isDelete = true;
				m_particleArray[i].isDelete = true;
				continue;
			}

			D3DXVECTOR3 pos,scl;
			D3DXVECTOR4 col;

			switch( m_particleArray[i].mode )
			{
				case EffectManager::COMP_MODE_LINE:
					pos = Lerp( m_particleArray[i].key[0].pos,m_particleArray[i].key[1].pos,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,Linear );
					scl = Lerp( m_particleArray[i].key[0].scl,m_particleArray[i].key[1].scl,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,Linear );
					col = Lerp( m_particleArray[i].key[0].col,m_particleArray[i].key[1].col,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,Linear );
					break;																														  
				case EffectManager::COMP_MODE_EASEIN:																							  
					pos = Lerp( m_particleArray[i].key[0].pos,m_particleArray[i].key[1].pos,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,EaseIn );
					scl = Lerp( m_particleArray[i].key[0].scl,m_particleArray[i].key[1].scl,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,EaseIn );
					col = Lerp( m_particleArray[i].key[0].col,m_particleArray[i].key[1].col,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,EaseIn );
																																				  
					break;																														  
				case EffectManager::COMP_MODE_EASEOUT:																							  
					pos = Lerp( m_particleArray[i].key[0].pos,m_particleArray[i].key[1].pos,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,EaseOut );
					scl = Lerp( m_particleArray[i].key[0].scl,m_particleArray[i].key[1].scl,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,EaseOut );
					col = Lerp( m_particleArray[i].key[0].col,m_particleArray[i].key[1].col,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,EaseOut );
																																				  
					break;																														  
				case EffectManager::COMP_MODE_CUBE:																								  
					pos = Lerp( m_particleArray[i].key[0].pos,m_particleArray[i].key[1].pos,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,Cube );
					scl = Lerp( m_particleArray[i].key[0].scl,m_particleArray[i].key[1].scl,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,Cube );
					col = Lerp( m_particleArray[i].key[0].col,m_particleArray[i].key[1].col,0.0f,(float)m_particleArray[i].compFrame,m_particleArray[i].compErase,Cube );
					break;
			}

			m_particleArray[i].compErase++;

			if( m_particleArray[i].compErase > m_particleArray[i].compFrame )
			{
				m_particleArray[i].compErase = m_particleArray[i].compFrame;
				m_particleArray[i].lifeSpan = 0;
			}

			//アニメーション更新
			D3DXVECTOR2 offset = m_particleArray[i].anim.startUvOffset;
			int anim_cnt = 0;
			//アニメーションがあれば
			if( m_particleArray[i].anim.animSum > 0 )
			{
				//１アニメーションが切り替わるフレーム数を算出
				int change_frame = m_particleArray[i].animFrame / m_particleArray[i].anim.animSum;

				m_particleArray[i].animErase++;

				//何個分かでアニメ―ションカウントを決定
				anim_cnt = m_particleArray[i].animErase / change_frame;

				offset.x += 1 * ( anim_cnt % m_particleArray[i].anim.animSum );
			}
			//適用
			m_particleArray[i].polygon->pos = pos;
			m_particleArray[i].polygon->col = D3DXCOLOR( col.x,col.y,col.z,col.w );
			m_particleArray[i].polygon->scl = scl;
			m_particleArray[i].polygon->uvOffset = offset;

		}

	}
}


//---------------------------------------------------------------------------------
//補完
//---------------------------------------------------------------------------------

D3DXVECTOR3 EffectManager::Lerp( const D3DXVECTOR3 &st,const D3DXVECTOR3 &ed,float min,float max,float elapsed,float (*func)(float) )
{
	float t = ( elapsed - min ) / ( max - min );// 時間を媒介変数に

	if( t < 0.0 )
	{
		t = 0;
	}

	if( t > 1.0 )
	{
		t = 1;
	}

	//各補完関数でレートを決定
	float rate = (*func)(t);

	return st * ( 1.0f - rate ) + ed * rate;
}

D3DXVECTOR4 EffectManager::Lerp( const D3DXVECTOR4 &st,const D3DXVECTOR4 &ed,float min,float max,float elapsed,float (*func)(float)  )
{
	float t = ( elapsed - min ) / ( max - min );// 時間を媒介変数に

	if( t < 0.0 )
	{
		t = 0;
	}

	if( t > 1.0 )
	{
		t = 1;
	}

	//各補完関数でレートを決定
	float rate = (*func)(t);

	return st * ( 1.0f - rate ) + ed * rate;
}

//-------------------------------------------------------------------------------------------------------
//エフェクト生成
//-------------------------------------------------------------------------------------------------------

void EffectManager::AddParticle(const D3DXVECTOR3 &posSt,const D3DXVECTOR3 &sclSt,const D3DXVECTOR4 &colSt,
								const D3DXVECTOR3 &posEd,const D3DXVECTOR3 &sclEd,const D3DXVECTOR4 &colEd,
								const int compFrame,const int life,const int animSum,const int animFrame,const int offsetX,const int offsetY,
								const COMPLETIONMODE compMode )
{

	int idx = -1;

	for( int i = 0 ; i < m_particleMax ; i++ )
	{
		if( m_particleArray[ m_cursol ].isDelete )
		{
			idx = m_cursol;
			break;
		}


		m_cursol++;

		if( m_cursol >= m_particleMax-1 )
		{
			m_cursol = 0;
		}
	}

	//インスタンシング側のエラーチェックしないのはパーティクル最大数 = ポリゴンだから
	if( idx != -1 )
	{
		PARTICLE p;

		p.key[0].pos = posSt;
		p.key[1].pos = posEd;
		p.key[0].scl = sclSt;
		p.key[1].scl = sclEd;
		p.key[0].col = colSt;
		p.key[1].col = colEd;
		p.compFrame = compFrame;
		p.lifeSpan = life;
		p.anim.animSum = animSum;
		p.animFrame = animFrame;
		p.mode = compMode;
		p.anim.startUvOffset.x = offsetX;
		p.anim.startUvOffset.y = offsetY;
		p.isDelete = false;

		p.compErase = 0;
		p.animErase = 0;
		
		p.polygon = m_insBill->LentInsPolygonData();
		p.polygon->pos = p.key[0].pos;
		p.polygon->rot = D3DXVECTOR3(0,0,0);
		p.polygon->scl = p.key[0].scl;
		p.polygon->col = D3DXCOLOR( colSt.x,colSt.y,colSt.z,colSt.w );
		p.polygon->uvOffset = p.anim.startUvOffset;
		p.polygon->isDelete = false;
		m_particleArray[ idx ] = p;
	}
}

void EffectManager::AddEffectFromDataBase( const int idx,const D3DXVECTOR3 pos )
{

	//関数呼びまくるよりは早い気がする
	for( int i = 0 ; i < m_effectDatabase[idx].particleSum ; i++ )
	{
		int lentidx = -1;

		for( int j = 0 ; j < m_particleMax ; j++ )
		{
			if( m_particleArray[ m_cursol ].isDelete )
			{
				lentidx = m_cursol;
				break;
			}


			m_cursol++;

			if( m_cursol >= m_particleMax-1 )
			{
				m_cursol = 0;
			}
		}

		//取れないってことはもう空きないんで終了
		if( lentidx == -1 )
		{
			break;
		}

		//セット
		m_particleArray[ lentidx ].isDelete     = false;
		m_particleArray[ lentidx ].animErase    = 0;
		m_particleArray[ lentidx ].compErase    = 0;
		m_particleArray[ lentidx ].anim.animSum         = m_effectDatabase[ idx ].particleArray[i].anim.animSum;
		m_particleArray[ lentidx ].anim.startUvOffset   = m_effectDatabase[ idx ].particleArray[i].anim.startUvOffset;
		m_particleArray[ lentidx ].animFrame            = m_effectDatabase[ idx ].particleArray[i].animFrame;
		m_particleArray[ lentidx ].compFrame            = m_effectDatabase[ idx ].particleArray[i].compFrame;
		m_particleArray[ lentidx ].key[0].pos           = m_effectDatabase[ idx ].particleArray[i].key[0].pos + pos;
		m_particleArray[ lentidx ].key[0].scl           = m_effectDatabase[ idx ].particleArray[i].key[0].scl;
		m_particleArray[ lentidx ].key[0].col           = m_effectDatabase[ idx ].particleArray[i].key[0].col;
		m_particleArray[ lentidx ].key[1].pos           = m_effectDatabase[ idx ].particleArray[i].key[1].pos + pos;
		m_particleArray[ lentidx ].key[1].scl           = m_effectDatabase[ idx ].particleArray[i].key[1].scl;
		m_particleArray[ lentidx ].key[1].col           = m_effectDatabase[ idx ].particleArray[i].key[1].col;
		m_particleArray[ lentidx ].lifeSpan             = m_effectDatabase[ idx ].particleArray[i].lifeSpan;
		m_particleArray[ lentidx ].mode                 = m_effectDatabase[ idx ].particleArray[i].mode;
		m_particleArray[ lentidx ].polygon              = m_insBill->LentInsPolygonData();
		m_particleArray[ lentidx ].polygon->pos = m_particleArray[ lentidx ].key[0].pos;
		m_particleArray[ lentidx ].polygon->scl = m_particleArray[ lentidx ].key[0].scl;
		m_particleArray[ lentidx ].polygon->col = D3DXCOLOR(m_particleArray[ lentidx ].key[0].col.x,m_particleArray[ lentidx ].key[0].col.y,
														m_particleArray[ lentidx ].key[0].col.z,m_particleArray[ lentidx ].key[0].col.w );
		m_particleArray[ lentidx ].polygon->uvOffset = m_particleArray[ lentidx ].anim.startUvOffset;
		m_particleArray[ lentidx ].polygon->isDelete = false;

	}
}

//-------------------------------------------------------------------------------------------------------------
//テクスチャロード
//-------------------------------------------------------------------------------------------------------------

void EffectManager::LoadTexture( const char *filePath,const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize )
{
	m_insBill->LoadTexture( filePath );
	m_insBill->SetVtxBuffTexInfo( texSize,oneSize );
}

//-------------------------------------------------------------------------------------------------------------
//テクスチャ情報
//-------------------------------------------------------------------------------------------------------------

void EffectManager::SetTexInfo( const D3DXVECTOR2 &texSize,const D3DXVECTOR2 &oneSize )
{
	m_insBill->SetVtxBuffTexInfo( texSize,oneSize );
}

//------------------------------------------------------------------------------------------------------------
//カメラ位置セット
//------------------------------------------------------------------------------------------------------------

void EffectManager::SetCamPos( const D3DXVECTOR3 &camPos )
{
	m_insBill->SetCameraPos( camPos );
}

//-----------------------------------------------------------------------------------------------------------
//エフェクトファイルをデータベースにロード
//-----------------------------------------------------------------------------------------------------------

void EffectManager::LoadEffectData( const char *filePath )
{
	FILE *file = NULL;
	fopen_s( &file,filePath,"rb");
	int size;

	//グループ数保存
	fread(&size,sizeof(int),1,file );

	int byte = sizeof( D3DXVECTOR3 )*2+ sizeof( D3DXVECTOR2 ) * 3 + sizeof( D3DXVECTOR4 ) * 2 + sizeof( int )*4;
	int create_sum = 0;

	//総エフェクト数をカウント
	for( int group = 0; group < size ; group++ )
	{
		//生成エフェクト数保存
		int buf;
		fread( &buf,sizeof(int),1,file );
		create_sum += buf;

		for( int i = 0 ; i < buf; i++ )
		{
			fseek(file,byte,SEEK_CUR );
		}

	}

	fseek( file,0,SEEK_SET );

	//グループ数保存
	fread(&size,sizeof(int),1,file );

	m_effectDatabase[m_databaseCursol ].particleArray = new PARTICLE[ create_sum ];
	m_effectDatabase[m_databaseCursol ].particleSum = create_sum;

	int cnt = 0;

	for( int group = 0; group < size ; group++ )
	{
		//生成エフェクト数保存
		int buf;
		fread( &buf,sizeof(int),1,file );

		for( int i = 0 ; i < buf; i++ )
		{
			D3DXVECTOR2 scl_st,scl_ed;
			int life;
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].key[0].pos,sizeof( D3DXVECTOR3 ),1,file );
			fread( &scl_st,sizeof( D3DXVECTOR2 ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].key[0].col,sizeof( D3DXVECTOR4 ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].key[1].pos,sizeof( D3DXVECTOR3 ),1,file );
			fread( &scl_ed,sizeof( D3DXVECTOR2 ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].key[1].col,sizeof( D3DXVECTOR4 ),1,file );
			fread( &life,  sizeof( int ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].mode,sizeof( int ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].anim.startUvOffset,sizeof( D3DXVECTOR2 ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].anim.animSum,sizeof( int ),1,file );
			fread( &m_effectDatabase[m_databaseCursol].particleArray[cnt].animFrame,sizeof( int ),1,file );

			//
			m_effectDatabase[m_databaseCursol].particleArray[cnt].lifeSpan = life;
			m_effectDatabase[m_databaseCursol].particleArray[cnt].compFrame = m_effectDatabase[m_databaseCursol].particleArray[cnt].lifeSpan;
			m_effectDatabase[m_databaseCursol].particleArray[cnt].key[0].scl = D3DXVECTOR3( scl_st.x,scl_st.y,1 );
			m_effectDatabase[m_databaseCursol].particleArray[cnt].key[1].scl = D3DXVECTOR3( scl_ed.x,scl_ed.y,1 );

			cnt++;
		}

	}

	fclose( file );

	m_databaseCursol++;
}

//---------------------------------------------------------------------------------------
//オプションセット
//---------------------------------------------------------------------------------------

void EffectManager::SetOption( InstancingBillboard::OPTION option )
{
	m_insBill->SetOption( option );
}


// EOF
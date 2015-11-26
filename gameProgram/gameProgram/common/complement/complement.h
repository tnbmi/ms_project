//*****************************************************************************
//
// 補完関数 [complement.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _COMPLEMENT_H
#define _COMPLEMENT_H
//=============================================================================

#include<d3dx9.h>

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インライン定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

inline float Cube   ( const float t ){ return t * t * ( 3.0f - 2.0f * t ); }
inline float Linear ( const float t ){ return t;  }
inline float EaseIn ( const float t ){ return t*t;}
inline float EaseOut( const float t ){ return t*(2-t); }

inline D3DXVECTOR3 LerpVec3(  const D3DXVECTOR3 &st,const D3DXVECTOR3 &ed,float min,float max,float elapsed,float (*func)(float) )
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

inline D3DXVECTOR4 LerpVec4( const D3DXVECTOR4 &st,const D3DXVECTOR4 &ed,float min,float max,float elapsed,float (*func)(float)  )
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

//=============================================================================
#endif
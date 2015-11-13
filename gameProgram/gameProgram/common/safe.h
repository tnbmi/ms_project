//*****************************************************************************
//
// セーフ関数 [safe.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_SAFE_H
#define _MY_SAFE_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インライン定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
template <typename TEM>
inline void SafeDelete(TEM*& p)
{
	if(p != nullptr)
	{
		delete p;
		p = nullptr;
	}
}

template <typename TEM>
inline void SafeDeleteArray(TEM*& p)
{
	if(p != nullptr)
	{
		delete[] p;
		p = nullptr;
	}
}

template <typename TEM>
inline void SafeRelease(TEM*& p)
{
	if(p != nullptr)
	{
		p->Release();
		p = nullptr;
	}
}

template <typename TEM>
inline void SafeFinalize(TEM*& p)
{
	if(p != nullptr)
	{
		p->Finalize();
		p = nullptr;
	}
}

template <typename TEM>
inline void SafeFinalizeDelete(TEM*& p)
{
	if(p != nullptr)
	{
		p->Finalize();
		delete[] p;
		p = nullptr;
	}
}

//=============================================================================
#endif

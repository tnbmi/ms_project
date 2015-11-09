//*****************************************************************************
//
// リザルトフェーズ [result.h]
// Author : MAI TANABE
//
//*****************************************************************************

#ifndef MY_RESULT_H_
#define MY_RESULT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\..\main\main.h"
#include "..\phase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class ResultImport;
class Camera;

class Result : public Phase
{
public:
	Result(LPDIRECT3DDEVICE9 device);
	virtual ~Result(void);

	bool Initialize(void);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:
	bool InitObject(void);

	ResultImport*	m_import;
	Camera*			m_camera;
};

//=============================================================================
#endif

// EOF


#pragma once

//*******************************************************************************
//
//	Scriptクラス
//
//*******************************************************************************

//	include
#include	"textLoader.h"

//	class
class Script	:	public	textLoader
{
private:

public:
	//	初期化・解放
	Script( void );
	~Script( void );

	//	動作関数
	bool	Open( LPSTR filename );


};


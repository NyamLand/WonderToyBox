
#pragma once

//*******************************************************************************
//
//	BasePlayerクラス
//
//*******************************************************************************

//	include
#include	"BaseChara.h"

//	class
class BasePlayer
{
private:
public:
	//	初期化・解放
	BasePlayer( void );
	~BasePlayer( void );

	//	動作関数
	void	Control( void );
};


#pragma once

//*******************************************************************************
//
//	Script�N���X
//
//*******************************************************************************

//	include
#include	"textLoader.h"

//	class
class Script	:	public	textLoader
{
private:

public:
	//	�������E���
	Script( void );
	~Script( void );

	//	����֐�
	bool	Open( LPSTR filename );


};


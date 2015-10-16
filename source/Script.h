
#pragma once

//*******************************************************************************
//
//	ScriptƒNƒ‰ƒX
//
//*******************************************************************************

//	include
#include	"textLoader.h"

//	class
class Script	:	public	textLoader
{
private:

public:
	//	‰Šú‰»E‰ğ•ú
	Script( void );
	~Script( void );

	//	“®ìŠÖ”
	bool	Open( LPSTR filename );


};


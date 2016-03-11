
#pragma once

//***********************************************************************
//
//	LastResultクラス
//
//***********************************************************************

//	include

//	class
class LastResult
{
private:
public:
	//	初期化・解放
	LastResult( void );
	~LastResult( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
};

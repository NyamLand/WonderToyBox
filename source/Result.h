
#pragma once

//***********************************************************************
//
//	Resultクラス
//
//***********************************************************************

//	include

//	class
class Result
{
private:
public:
	//	初期化・解放
	Result( void );
	~Result( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
};

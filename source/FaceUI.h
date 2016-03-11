
#pragma once

//***********************************************************************
//
//	FaceUIクラス
//
//***********************************************************************

//	include

//	class
class FaceUI
{
private:


public:
	//	初期化・解放
	FaceUI( void );
	~FaceUI( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
};

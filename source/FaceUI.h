
#pragma once

//**********************************************************************
//
//	FaceUIクラス
//
//**********************************************************************

//	include

//	class
class FaceUI
{
private:

	iex2DObj*	face;
	ImageObj	faceImage[PLAYER_MAX];
	int			posXTable[PLAYER_MAX];

public:
	//	初期化・解放
	FaceUI( void );
	~FaceUI( void );
	void	Initialize( void );
	void	TableInitialize( void );

	//	更新・描画
	void	Update( void );
	void	FaceImageUpdate( int num, int mode );
	void	Render( void );


};
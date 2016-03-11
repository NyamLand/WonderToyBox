
#pragma once

//**********************************************************************
//
//	FaceUI�N���X
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
	//	�������E���
	FaceUI( void );
	~FaceUI( void );
	void	Initialize( void );
	void	TableInitialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	FaceImageUpdate( int num, int mode );
	void	Render( void );


};

#pragma once

//***********************************************************************
//
//	Result�N���X
//
//***********************************************************************

//	include

//	class
class Result
{
private:
public:
	//	�������E���
	Result( void );
	~Result( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
};

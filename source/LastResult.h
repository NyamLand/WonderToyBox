
#pragma once

//***********************************************************************
//
//	LastResult�N���X
//
//***********************************************************************

//	include

//	class
class LastResult
{
private:
public:
	//	�������E���
	LastResult( void );
	~LastResult( void );
	bool	Initialize( void );
	void	Release( void );
	void	Load( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
};

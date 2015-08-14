
#ifndef __SQUIRREL_H__
#define	__SQUIRREL_H__

//*********************************************************************************
//
//	Squirrel�N���X
//
//*********************************************************************************
class Squirrel : public		Player
{
private:

public:
	//	�������E���
	Squirrel( void );
	~Squirrel( void );
	void	SetMotionData( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	����֐�
	void	ModeManagement( void );
	void	Move( void );
	void	Jump( void );
	void	Attack( int attackKind );
	void	Damage( int type );
	bool	QuickArts( void );
	bool	PowerArts( void );
	bool	HyperArts( void );
	void	Guard( void );
};

//*********************************************************************************
#endif // !__SQUIRREL_H__

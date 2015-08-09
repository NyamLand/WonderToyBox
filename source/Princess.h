
#ifndef __PRINCESS_H__
#define	__PRINCESS_H__

//*********************************************************************************
//
//	Princess�N���X
//
//*********************************************************************************
class Princess : public		Player
{
private:


public:
	//	�������E���
	Princess( void );
	~Princess( void );
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
#endif // !__PRINCESS_H__

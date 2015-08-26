
#ifndef __KNIGHT_H__
#define	__KNIGHT_H__

//*********************************************************************************
//
//	Knight�N���X
//
//*********************************************************************************
class Knight : public	Player
{
private:

private:
	float	lance_r;

public:
	//	�������E���
	Knight( void );
	~Knight( void );
	void	SetMotionData( void );

	//	�X�V�E�`��
	void	Render( iexShader* shader, LPSTR technique );

	//	����֐�
	bool	QuickArts( void );
	bool	PowerArts( void );
	bool	HyperArts( void );

	//	���ݒ�
	void	SetAttackParam( int attackKind );
};

//*********************************************************************************
#endif // !__KNIGHT_H__


#ifndef __TIGER_H__
#define	__TIGER_H__

//*******************************************************************************
//
//	Tiger�N���X
//
//*******************************************************************************

class Tiger	: public	Player
{
private:
	int		attackCount;
	bool	attackFlag;


public:
	//	�������E���
	Tiger( void );
	~Tiger( void );
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


//*******************************************************************************
#endif // !__Tiger_H__

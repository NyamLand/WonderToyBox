
#ifndef __TIGER_H__
#define	__TIGER_H__

//*******************************************************************************
//
//	Tiger�N���X
//
//*******************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Tiger	: public	BaseChara
{
private:
	//	�萔
	enum MOTION_DATA
	{
		STAND = 1,			//	����
		POSTURE,				//	�\��
		RUN = 4,				//	����
		ATTACK1,				//	�U���P�i�K��
		ATTACK2,				//	�U���Q�i�K��
		ATTACK3,				//	�U���R�i�K��
		JUMP,
		GUARD,
	};

private:
	int		attackCount;
	bool	attackFlag;

private:
	void	MotionManagement( int motion )override;

public:
	//	�������E���
	Tiger( void );
	~Tiger( void );

	//	�X�V�E�`��
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;

	//	����֐�
	bool	QuickArts( void )override;
	bool	PowerArts( void )override;
	bool	HyperArts( void )override;

	//	���ݒ�
	void	SetAttackParam( int attackKind )override;
};


//*******************************************************************************
#endif // !__Tiger_H__

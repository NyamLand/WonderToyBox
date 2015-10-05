
#ifndef __SQUIRREL_H__
#define	__SQUIRREL_H__

//*********************************************************************************
//
//	Squirrel�N���X
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Squirrel : public		BaseChara
{
private:
	//	�萔
	enum MOTION_DATA
	{
		STAND = 1,		//	����
		POSTURE,				//	�\��
		RUN = 4,		//	����
		ATTACK1,				//	�U���P�i�K��
		ATTACK2,				//	�U���Q�i�K��
		ATTACK3,				//	�U���R�i�K��
		JUMP,
		GUARD,
	};

private:
	void	MotionManagement( int motion )override;

public:
	//	�������E���
	Squirrel( void );
	~Squirrel( void );

	//	�X�V�E�`��
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;

	//	����֐�
	bool	QuickArts( void )override;
	bool	PowerArts( void )override;
	bool	HyperArts( void )override;

	//	���ݒ�
	void	SetAttackParam( int attackKind )override;
};

//*********************************************************************************
#endif // !__SQUIRREL_H__

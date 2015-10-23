
#ifndef __KNIGHT_H__
#define	__KNIGHT_H__

//*********************************************************************************
//
//	Knight�N���X
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Knight : public	BaseChara
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
	float	lance_r;

private:
	void	MotionManagement( int motion )override;

public:
	//	�������E���
	Knight( void );
	~Knight( void );
	bool	Initialize( int playerNum, Vector3 pos )override;

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
#endif // !__KNIGHT_H__

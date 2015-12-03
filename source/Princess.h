
#ifndef __PRINCESS_H__
#define	__PRINCESS_H__

//*********************************************************************************
//
//	Princess�N���X
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Princess : public		BaseChara
{
private:
	enum MOTION_DATA
	{
		STAND = 0,			//	����
		POSTURE = 0,		//	�\��
		RUN,					//	����
		JUMP,
		ATTACK1,				//	�U���P�i�K��
		ATTACK2,				//	�U���Q�i�K��
		ATTACK3,				//	�U���R�i�K��
		GUARD,
	};

private:
	void	MotionManagement( int motion )override;

public:
	//	�������E���
	Princess( void );
	~Princess( void );
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
#endif // !__PRINCESS_H__


#ifndef __PIRATE_H__
#define	__PIRATE_H__

//*********************************************************************************
//
//	Princess�N���X
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Pirate : public		BaseChara
{
private:
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

	enum QuickArts_DATA
	{
		NORMAL_SHOT = 0,
		TIMER_SHOT,
	};

private:
	void	MotionManagement( int motion )override;
	iexMesh*	cannon;

public:
	//	�������E���
	Pirate( void );
	~Pirate( void );
	bool	Initialize( int playerNum, Vector3 pos )override;

	//	�X�V�E�`��
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;

	//	����֐�
	bool	QuickArts( void )override;
	bool	PowerArts( void )override;
	bool	HyperArts( void )override;
	void	SetCannonPos( void );
	//	���ݒ�
	void	SetAttackParam( int attackKind )override;


};

//*********************************************************************************
#endif // !__PRINCESS_H__

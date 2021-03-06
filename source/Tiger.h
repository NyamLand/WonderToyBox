
#ifndef __TIGER_H__
#define	__TIGER_H__

//*******************************************************************************
//
//	TigerNX
//
//*******************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Tiger	: public	BaseChara
{
private:
	//	è
	enum MOTION_DATA
	{
		STAND = 1,			//	§¿
		POSTURE,				//	\¦
		RUN = 4,				//	è
		ATTACK1,				//	UPiKÚ
		ATTACK2,				//	UQiKÚ
		ATTACK3,				//	URiKÚ
		JUMP,
		GUARD,
	};

private:
	int		attackCount;
	bool	attackFlag;

private:
	//void	MotionManagement( int motion )override;

public:
	//	ú»Eðú
	Tiger( void );
	~Tiger( void );
	bool	Initialize( int playerNum, Vector3 pos )override;

	//	XVE`æ
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;

	//	®ìÖ
	bool	QuickArts( void )override;
	bool	PowerArts( void )override;
	bool	HyperArts( void )override;

	//	îñÝè
	void	SetAttackParam( int attackKind )override;
};


//*******************************************************************************
#endif // !__Tiger_H__

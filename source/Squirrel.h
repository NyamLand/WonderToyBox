
#ifndef __SQUIRREL_H__
#define	__SQUIRREL_H__

//*********************************************************************************
//
//	SquirrelNX
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Squirrel : public		BaseChara
{
private:
	//	è
	enum MOTION_DATA
	{
		STAND = 1,		//	§¿
		POSTURE,				//	\¦
		RUN = 4,		//	è
		ATTACK1,				//	UPiKÚ
		ATTACK2,				//	UQiKÚ
		ATTACK3,				//	URiKÚ
		JUMP,
		GUARD,
	};

private:
//	void	MotionManagement( int motion )override;

public:
	//	ú»Eðú
	Squirrel( void );
	~Squirrel( void );
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

//*********************************************************************************
#endif // !__SQUIRREL_H__

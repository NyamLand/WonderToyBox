
#ifndef __PRINCESS_H__
#define	__PRINCESS_H__

//*********************************************************************************
//
//	PrincessNX
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
		STAND = 0,			//	§¿
		POSTURE = 0,		//	\¦
		RUN,					//	è
		JUMP,
		ATTACK1,				//	UPiKÚ
		ATTACK2,				//	UQiKÚ
		ATTACK3,				//	URiKÚ
		GUARD,
	};

private:
	void	MotionManagement( int motion )override;

public:
	//	ú»Eðú
	Princess( void );
	~Princess( void );
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
#endif // !__PRINCESS_H__

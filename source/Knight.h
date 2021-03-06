
#ifndef __KNIGHT_H__
#define	__KNIGHT_H__

//*********************************************************************************
//
//	KnightNX
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Knight : public	BaseChara
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
	float	lance_r;

private:
	//void	MotionManagement( int motion )override;

public:
	//	ú»Eðú
	Knight( void );
	~Knight( void );
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
#endif // !__KNIGHT_H__

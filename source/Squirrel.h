
#ifndef __SQUIRREL_H__
#define	__SQUIRREL_H__

//*********************************************************************************
//
//	SquirrelƒNƒ‰ƒX
//
//*********************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Squirrel : public		BaseChara
{
private:
	//	’è”
	enum MOTION_DATA
	{
		STAND = 1,		//	—§‚¿
		POSTURE,				//	\‚¦
		RUN = 4,		//	‘–‚è
		ATTACK1,				//	UŒ‚‚P’iŠK–Ú
		ATTACK2,				//	UŒ‚‚Q’iŠK–Ú
		ATTACK3,				//	UŒ‚‚R’iŠK–Ú
		JUMP,
		GUARD,
	};

private:
	void	MotionManagement( int motion )override;

public:
	//	‰Šú‰»E‰ğ•ú
	Squirrel( void );
	~Squirrel( void );

	//	XVE•`‰æ
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;

	//	“®ìŠÖ”
	bool	QuickArts( void )override;
	bool	PowerArts( void )override;
	bool	HyperArts( void )override;

	//	î•ñİ’è
	void	SetAttackParam( int attackKind )override;
};

//*********************************************************************************
#endif // !__SQUIRREL_H__

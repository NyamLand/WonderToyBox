
#ifndef __PRINCESS_H__
#define	__PRINCESS_H__

//*********************************************************************************
//
//	PrincessƒNƒ‰ƒX
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
		STAND = 0,			//	—§‚¿
		POSTURE = 0,		//	\‚¦
		RUN,					//	‘–‚è
		JUMP,
		ATTACK1,				//	UŒ‚‚P’iŠK–Ú
		ATTACK2,				//	UŒ‚‚Q’iŠK–Ú
		ATTACK3,				//	UŒ‚‚R’iŠK–Ú
		GUARD,
	};

private:
	void	MotionManagement( int motion )override;

public:
	//	‰Šú‰»E‰ğ•ú
	Princess( void );
	~Princess( void );
	bool	Initialize( int playerNum, Vector3 pos )override;

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
#endif // !__PRINCESS_H__

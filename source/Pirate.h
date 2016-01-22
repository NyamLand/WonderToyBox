
#ifndef __PIRATE_H__
#define	__PIRATE_H__

//*********************************************************************************
//
//	PrincessƒNƒ‰ƒX
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
		STAND = 1,			//	—§‚¿
		POSTURE,				//	\‚¦
		RUN = 4,				//	‘–‚è
		ATTACK1,				//	UŒ‚‚P’iŠK–Ú
		ATTACK2,				//	UŒ‚‚Q’iŠK–Ú
		ATTACK3,				//	UŒ‚‚R’iŠK–Ú
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
	//	‰Šú‰»E‰ğ•ú
	Pirate( void );
	~Pirate( void );
	bool	Initialize( int playerNum, Vector3 pos )override;

	//	XVE•`‰æ
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;

	//	“®ìŠÖ”
	bool	QuickArts( void )override;
	bool	PowerArts( void )override;
	bool	HyperArts( void )override;
	void	SetCannonPos( void );
	//	î•ñİ’è
	void	SetAttackParam( int attackKind )override;


};

//*********************************************************************************
#endif // !__PRINCESS_H__

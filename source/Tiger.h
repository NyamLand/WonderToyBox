
#ifndef __TIGER_H__
#define	__TIGER_H__

//*******************************************************************************
//
//	TigerƒNƒ‰ƒX
//
//*******************************************************************************

//	include
#include	"BaseChara.h"

//	class
class Tiger	: public	BaseChara
{
private:
	//	’è”
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

private:
	int		attackCount;
	bool	attackFlag;

private:
	void	MotionManagement( int motion )override;

public:
	//	‰Šú‰»E‰ğ•ú
	Tiger( void );
	~Tiger( void );

	//	XVE•`‰æ
	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr )override;

	//	“®ìŠÖ”
	bool	QuickArts( void )override;
	bool	PowerArts( void )override;
	bool	HyperArts( void )override;

	//	î•ñİ’è
	void	SetAttackParam( int attackKind )override;
};


//*******************************************************************************
#endif // !__Tiger_H__

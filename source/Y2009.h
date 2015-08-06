
#ifndef __Y2009_H__
#define	__Y2009_H__

//*******************************************************************************
//
//	Player_Y2009ƒNƒ‰ƒX
//
//*******************************************************************************

class Y2009	:	public	Player
{
private:
	//	’è”
	enum MotionNum
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

public:
	//	‰Šú‰»E‰ğ•ú
	Y2009( void );
	~Y2009( void );
	void	SetMotionData( void );
	
	//	XVE•`‰æ
	void	Update( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	“®ìŠÖ”
	void	ModeManagement( void );
	void	Move( void );
	void	Jump( void );
	void	Attack( void );
};

//*******************************************************************************
#endif // !__Y2009_H__

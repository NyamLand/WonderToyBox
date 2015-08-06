
#ifndef __PRINCESS_H__
#define	__PRINCESS_H__

//*********************************************************************************
//
//	PrincessƒNƒ‰ƒX
//
//*********************************************************************************
class Princess : public		Player
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

private:
	float	attack_r;
	float	attack_t;

public:
	//	‰Šú‰»E‰ğ•ú
	Princess( void );
	~Princess( void );
	void	SetMotionData( void );
	
	//	XVE•`‰æ
	void	Update( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	“®ìŠÖ”
	void	ModeManagement( void );
	void	Move( void );
	void	Jump( void );
	void	Attack( int attackKind );
	bool	QuickArts( void );
	bool	PowerArts( void );
	bool	HyperArts( void );

};

//*********************************************************************************
#endif // !__PRINCESS_H__

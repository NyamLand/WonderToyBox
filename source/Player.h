
#ifndef		__PLAYER_H__
#define		__PLAYER_H__

//****************************************************************************************
//
//	PlayerŠÖ˜AƒNƒ‰ƒX
//
//****************************************************************************************
class Player : public	BaseObj
{
private:
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

	//	•Ï”

public:
	//	‰Šú‰»E‰ğ•ú
	Player( void );
	~Player( void );
	virtual	void	SetMotionData( void ) = 0;

	//	XVE•`‰æ
	virtual	void	Update( void ) = 0;
	virtual	void	Render( iexShader* shader, LPSTR technique ) = 0;
	void				Render( void );
	//	“®ìŠÖ”
	virtual	void	ModeManagement( void ) = 0;

};

//****************************************************************************************
#endif // !__PLAYER_H__

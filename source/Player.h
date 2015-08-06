
#ifndef		__PLAYER_H__
#define		__PLAYER_H__

//****************************************************************************************
//
//	PlayerึANX
//
//****************************************************************************************
class Player : public	BaseObj
{
private:
	enum MotionNum
	{
		STAND = 1,			//	งฟ
		POSTURE,				//	\ฆ
		RUN = 4,				//	่
		ATTACK1,				//	UPiKฺ
		ATTACK2,				//	UQiKฺ
		ATTACK3,				//	URiKฺ
		JUMP,
		GUARD,
	};

	//	ฯ

public:
	//	๚ปE๐๚
	Player( void );
	~Player( void );
	virtual	void	SetMotionData( void ) = 0;

	//	XVE`ๆ
	virtual	void	Update( void ) = 0;
	virtual	void	Render( iexShader* shader, LPSTR technique ) = 0;
	void				Render( void );
	//	ฎ์ึ
	virtual	void	ModeManagement( void ) = 0;

};

//****************************************************************************************
#endif // !__PLAYER_H__

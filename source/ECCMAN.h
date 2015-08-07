
#ifndef __ECCMAN_H__
#define	__ECCMAN_H__

//********************************************************************************
//
//	Player_ECCMANNX
//
//********************************************************************************
class ECCMAN : public	Player
{
private:
	enum MotionNum
	{
		POSTURE,				//	\¦
		RUN,						//	è
		ATTACK1,				//	UPiKÚ
		JUMP,
		LANDING,				//	n
		ATTACK2,				//	UQiKÚ
		ATTACK3,				//	URiKÚ
		GUARD,					//	K[h
	};

public:
	//	ú»Eðú
	ECCMAN( void );
	~ECCMAN( void );
	void	SetMotionData( void );
	
	//	XVE`æ
	void	Update( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	®ìÖ
	void	ModeManagement( void );
	void	Move( void );
	void	Jump( void );
	void	Attack( void );
};

//********************************************************************************
#endif // !__ECCMAN_H__

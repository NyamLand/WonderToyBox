
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
	enum ECCMANMotion
	{
		POSTURE,				//	\¦
		RUN,						//	θ
		ATTACK1,				//	UPiKΪ
		JUMP,
		LANDING,				//	n
		ATTACK2,				//	UQiKΪ
		ATTACK3,				//	URiKΪ
		GUARD,					//	K[h
	};

public:
	//	ϊ»Eπϊ
	ECCMAN( void );
	~ECCMAN( void );
	void	SetMotionNum( void );
};

//********************************************************************************
#endif // !__ECCMAN_H__

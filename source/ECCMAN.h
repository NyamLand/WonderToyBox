
#ifndef __ECCMAN_H__
#define	__ECCMAN_H__

//********************************************************************************
//
//	Player_ECCMANクラス
//
//********************************************************************************
class ECCMAN : public	Player
{
private:
	enum ECCMANMotion
	{
		POSTURE,				//	構え
		RUN,						//	走り
		ATTACK1,				//	攻撃１段階目
		JUMP,
		LANDING,				//	着地
		ATTACK2,				//	攻撃２段階目
		ATTACK3,				//	攻撃３段階目
		GUARD,					//	ガード
	};

public:
	//	初期化・解放
	ECCMAN( void );
	~ECCMAN( void );
	void	SetMotionNum( void );
};

//********************************************************************************
#endif // !__ECCMAN_H__


#ifndef __ECCMAN_H__
#define	__ECCMAN_H__

//********************************************************************************
//
//	Player_ECCMAN�N���X
//
//********************************************************************************
class ECCMAN : public	Player
{
private:
	enum ECCMANMotion
	{
		POSTURE,				//	�\��
		RUN,						//	����
		ATTACK1,				//	�U���P�i�K��
		JUMP,
		LANDING,				//	���n
		ATTACK2,				//	�U���Q�i�K��
		ATTACK3,				//	�U���R�i�K��
		GUARD,					//	�K�[�h
	};

public:
	//	�������E���
	ECCMAN( void );
	~ECCMAN( void );
	void	SetMotionNum( void );
};

//********************************************************************************
#endif // !__ECCMAN_H__

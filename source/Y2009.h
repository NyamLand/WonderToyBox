
#ifndef __Y2009_H__
#define	__Y2009_H__

//*******************************************************************************
//
//	Player_Y2009�N���X
//
//*******************************************************************************

class Y2009	:	public	Player
{
private:
	//	�萔
	enum MotionNum
	{
		STAND = 1,			//	����
		POSTURE,				//	�\��
		RUN = 4,				//	����
		ATTACK1,				//	�U���P�i�K��
		ATTACK2,				//	�U���Q�i�K��
		ATTACK3,				//	�U���R�i�K��
		JUMP,
		GUARD,
	};

public:
	//	�������E���
	Y2009( void );
	~Y2009( void );
	void	SetMotionData( void );
	
	//	�X�V�E�`��
	void	Update( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	����֐�
	void	ModeManagement( void );
	void	Move( void );
	void	Jump( void );
	void	Attack( void );
};

//*******************************************************************************
#endif // !__Y2009_H__

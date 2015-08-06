
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
	enum MotionNum
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

//********************************************************************************
#endif // !__ECCMAN_H__

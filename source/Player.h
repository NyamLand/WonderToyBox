
#ifndef		__PLAYER_H__
#define		__PLAYER_H__

//****************************************************************************************
//
//	Player�֘A�N���X
//
//****************************************************************************************
class Player : public	BaseObj
{
private:
	const float GRAVITY = -0.01f;
	
	//	�萔
	static const int MIN_INPUT_STATE = 300;	//	�X�e�B�b�N����ŏ��l

	enum Y2009Motion
	{
		STAND = 1,		//	����
		POSTURE,				//	�\��
		RUN = 4,		//	����
		ATTACK1,				//	�U���P�i�K��
		ATTACK2,				//	�U���Q�i�K��
		ATTACK3,				//	�U���R�i�K��
		JUMP
	};

	//	�ϐ�

public:
	//	�������E���
	Player( void );
	~Player( void );

	//	�X�V�E�`��
	void	Update( void );

	//	����֐�
	void	Move( void );
	void	Move( float speed );
	void	Attack( void );
	void	Damage( void );
	void	Jump( void );

	//	�p�x�␳�֐�
	void	AngleAdjust( float speed );
	void	AngleAdjust( const Vector3& direction, float speed );
};

//****************************************************************************************
#endif // !__PLAYER_H__

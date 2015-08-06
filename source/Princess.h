
#ifndef __PRINCESS_H__
#define	__PRINCESS_H__

//*********************************************************************************
//
//	Princess�N���X
//
//*********************************************************************************
class Princess : public		Player
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

private:
	float	attack_r;
	float	attack_t;

public:
	//	�������E���
	Princess( void );
	~Princess( void );
	void	SetMotionData( void );
	
	//	�X�V�E�`��
	void	Update( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	����֐�
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

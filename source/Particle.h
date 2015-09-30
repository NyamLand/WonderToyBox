
#ifndef		__PARTICLE_H__
#define		__PARTICLE_H__

//****************************************************************************************
//
//	Particle�֘A
//
//****************************************************************************************

class Particle
{
private:
	static	iexParticle*	particle;

	static	iex2DObj*		par[];
	static	int				timer;

public:
	//	�������E���
	Particle( void );
	~Particle( void );
	static bool	Initialize( void );
	static void Release( void );

	//	�X�V�E�`��
	static void	Update( void );
	static void	Render( void );

	//	����֐�
	static void	BlueFlame( const Vector3& pos, const float& scale = 1.0f );		//	��
	static void Spark( const Vector3& pos, const float& scale = 1.0f );			//	�Ή�
	static void Hit( const Vector3& pos, const int& time = 1, const float& scale = 1.0f);			//	�q�b�g��
	static void Smoke( const Vector3& pos, const int& time = 1, const float& scale = 1.0f);			//	�q�b�g��
};

//****************************************************************************************
#endif // !__PARTICLE_H__

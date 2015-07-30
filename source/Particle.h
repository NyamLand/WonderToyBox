
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
};

//****************************************************************************************
#endif // !__PARTICLE_H__

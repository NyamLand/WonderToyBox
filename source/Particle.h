
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
	iexParticle*		pt;
	int						timer;

public:
	//	�������E���
	bool	Initialize( void );
	void Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	BlueFlame( const Vector3& pos, const float& scale = 1.0f );		//	��
	void	Spark( const Vector3& pos, const float& scale = 1.0f );			//	�Ή�
	void	Hit( const Vector3& pos, const float& scale = 1.0f, const int& time = 5 );										//	��
	void	Smoke( const Vector3& pos, const float& scale = 1.0f, const int& time = 20 );										//	��
	void	PowerUp(const Vector3& pos, const float& scale = 1.0f, const int move = 100, const int& time = 10);				//	���UP
	void	SpeedUp(const Vector3& pos, const float& scale = 1.0f, const int move = 100, const int& time = 11);				//	���DOWN
	void	Suck(const Vector3& pos, const Vector3& target, const Vector3& side, const float& length, const float& scale = 1.0f, const int& time = 60);					//	�z������
	void	Dust(const Vector3& pos, const Vector3& back, const Vector3& side, const float& scale = 1.0f, const int& time = 1);
	void	Bom(const Vector3& pos, const float& move, const float& scale = 1.0f, const int& time = 6);
	void	Flower( const Vector3& pos, float scale, const Vector3& color);
	void	FlowerDisseminate( const Vector3& pos, float scale, float speed, const Vector3& color );
	void	Bomb( const Vector3& pos, float scale, const Vector3& color );
	void	CoinGet(const Vector3& pos, const float& scale = 1.0f);										//	�R�C�������p�G�t�F�N�g
	void	CoinUp( const Vector3& pos, const float& scale = 1.0f );									//	�R�C���l���p�G�t�F�N�g
	void	Magnet(const Vector3& pos, const float& length, const float& scale = 1.0f, const int& speed = 10);					//	�����񂹃G�t�F�N�g
	void	Death( const Vector3& pos, float scale, const Vector3& color );				//	���S�G�t�F�N�g
	void	Semicircle(const Vector3& pos, const Vector3& front, const Vector3& right, const Vector3& up, const float& scale = 1.0f);	//	�ːi���̕���؂�	
	void	CannonSmoke(const Vector3& pos, const Vector3& front, const Vector3& right, const Vector3& up, const float& scale = 1.0f);	//	��C��ł������̉�





	//	���擾
	static	Particle*	GetInstance( void );
};

#define	particle ( Particle::GetInstance() )

//****************************************************************************************
#endif // !__PARTICLE_H__

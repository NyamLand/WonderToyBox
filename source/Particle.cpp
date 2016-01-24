
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Random.h"
#include	"Particle.h"

//*******************************************************************************
//
//	Paticle�N���X
//
//*******************************************************************************

//------------------------------------------------------------------------
//	�O���[�o��
//------------------------------------------------------------------------

namespace
{
	namespace
	{
		enum EFF_TYPE
		{
			SPARK,
			STAR,
			SMOKE,
			POWER_UP,
			SPEED_UP,
			DUST,
			SUCK,
			FLOWER = 8,
			COIN,
		};
	}


}


//------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------

	//	������
	bool	Particle::Initialize( void )
	{
		pt = nullptr;
		pt = new iexParticle();
		pt->Initialize( "DATA/Effect/particle.png", 10000 );

		timer = 0;

		return	true;
	}

	//	���
	void	Particle::Release( void )
	{
		SafeDelete( pt );
	}

//------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------

	//	�X�V
	void	Particle::Update( void )
	{
		pt->Update();
	}

	//	�`��
	void	Particle::Render( void )
	{
		pt->Render();
	}

//------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------

	//	��
	void	Particle::BlueFlame( const Vector3& pos, const float& scale )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<5; j++ )
		{
			Pos.x = pos.x + ( Random::GetFloat( -1000.0f, 0.0f )	 * ( 0.001f * scale ) );
			Pos.y = pos.y + ( Random::GetFloat( -500.0f, 0.0f )	 * ( 0.001f * scale ) );
			Pos.z = pos.z + ( Random::GetFloat( -1000.0f, 0.0f )		* ( 0.001f * scale ) );

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = Random::GetFloat( 0.0f,100.0f ) * ( scale * 0.00015f );
			Power.z = 0.0f;

			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			pt->Set( SPARK, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.2f, 1.0f, 0.3f, scale, RS_COPY );
		}
	}

	//	�Ή�
	void	Particle::Spark( const Vector3& pos, const float& scale )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<10; j++ )
		{
			Pos.x = pos.x;
			Pos.y = pos.y;
			Pos.z = pos.z;

			Move.x = Random::GetFloat(-100.0f, 100.0f) *(0.002f * scale);
			Move.y = 1.0f * scale;
			Move.z = Random::GetFloat(-100.0f, 100.0f) * (0.002f * scale);

			Power.x = 0.0f;
			Power.y = Random::GetFloat(-100.0f, 0.0f) * (scale * 0.001f);
			Power.z = 0.0f;

			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			pt->Set( STAR, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.0f, scale, RS_COPY );
		}
	}

	//	��
	void	Particle::Hit( const Vector3& pos, const float& scale,const int& time )
	{
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;

		for ( int j = 0; j<10; j++ )
		{
			Pos.x = pos.x;
			Pos.y = pos.y;
			Pos.z = pos.z;

			Move.x = Random::GetFloat(-50.0f, 50.0f) * (0.005f * scale);
			Move.y = 0.8f * scale + (0.2f * scale * (j % 2));
			Move.z = Random::GetFloat(-20.0f, 20.0f) * (0.02f * scale);

			Power.x = 0.0f;
			Power.y = -(scale * 0.05f);
			Power.z = 0.0f;

			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			pt->Set( STAR, 0, 1.0f, 30, 0.0f, 15, 0.5f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.0f, scale, RS_COPY );
		}
	}

	//	��
	void	Particle::Smoke(const Vector3& pos, const float& scale, const int& time)
	{
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j<1; j++)
		{
			Pos.x = pos.x;
			Pos.y = pos.y;
			Pos.z = pos.z;

			Move.x = -0.1f * scale;
			Move.y = 0.08f * scale;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;

			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			pt->Set(SMOKE, 0, 1.0f, 30, 1.0f, 15, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY);
		}
	}


	//		�p���[�A�b�v
	void	Particle::PowerUp(const Vector3& pos, const float& scale , const int move, const int& time)
	{
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j<1; j++)
		{
			Pos.x = pos.x + (Random::GetInt(-move, move)	 * (0.01f * scale));
			Pos.y = pos.y;
			Pos.z = pos.z + (Random::GetInt(-move, move)	 * (0.01f * scale));

			Move.x = 0.0f;
			Move.y = Random::GetFloat(50.0f, 100.0f)	 * (0.001f * scale);
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;


			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			pt->Set(POWER_UP, 0, 1.0f, 45, 0.0f, 20, 0.5f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY);
		}
	}

	//	�X�s�[�h�A�b�v
	void	Particle::SpeedUp(const Vector3& pos, const float& scale, const int move, const int& time)
	{
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j<1; j++)
		{
			Pos.x = pos.x + (Random::GetInt(-move, move)	 * (0.01f * scale));
			Pos.y = pos.y;
			Pos.z = pos.z + (Random::GetInt(-move, move)	 * (0.01f * scale));

			Move.x = 0.0f;
			Move.y = Random::GetFloat(50.0f, 100.0f)	 * (0.001f * scale);
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;


			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			pt->Set(SPEED_UP, 0, 1.0f, 45, 0.0f, 20, 0.5f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY);
		}
	}

	//	�z������
	void	Particle::Suck(const Vector3& pos, const Vector3& target, const Vector3& side, const float& length, const float& scale, const int& time)
	{
		Vector3	Pos, Move, Power;
		int speed = 5;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j < 1; j++)
		{
			Vector3 v1, center, ran_pos;
			float	t_len;

			//	�^�[�Q�b�g�ƃ|�W�V�����̒��S�_���Ƃ�
			v1 = pos - target;
			t_len = v1.Length();
			v1.Normalize();
			center = target + v1 * (t_len / 2);

			//	�����_���Ŗ@���p�|�W�V�������Ƃ�
			ran_pos = center + side * (Random::GetFloat(-(t_len / 2) , t_len / 2));

			//	�@�����g���čŏI�I�����ʒu�����߂�
			v1 = ran_pos - pos;
			v1.Normalize();
			Pos =  pos + v1 * t_len;

			//	�������t��
			//Move = -v1 * scale;
			Move = -v1 * length / (speed * length);

			Power = Move * (Random::GetFloat(0.0001f, 0.001f));

			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			pt->Set(SUCK, 0, 1.0f, speed * (int)length, 1.0f, 0, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY);
		}
	}

	//	����
	void	Particle::Dust(const Vector3& pos, const Vector3& back, const Vector3& side, const float& scale, const int& time)
	{
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;
		for (int j = 0; j < 1; j++)
		{
			Pos = pos;


			Vector3 ideal_pos,ran_pos;
			
			//	�w��̗��z�|�W�V����
			ideal_pos = pos + back * 1.0f;

			//	�����_���Ŗ@���p�|�W�V�������Ƃ�
			ran_pos = ideal_pos + side * (Random::GetFloat(-1.0f, 1.0f));
			
			//	�@�����Z�b�g
			Move = ran_pos - pos;
			Move.Normalize();

			Move *= (Random::GetFloat(0.05f, 0.2f) * scale);


			Power = Move * (Random::GetFloat(0.00001f, 0.0001f)) + Vector3(0.0f, 1.0f, 0.0f) * (Random::GetFloat(0.0001f, 0.001f));


			////	�@�����g���čŏI�I�����ʒu�����߂�
			//v1 = ran_pos - pos;
			//v1.Normalize();
			//Pos = pos + v1 * t_len;

			////	�������t��
			//Move = -v1;

			//Power = Move * (Random::GetFloat(0.0001f, 0.001f));


			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			pt->Set(DUST, 0, 0.0f, 60, 0.0f, 20, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0f, 1.0f, scale, RS_COPY);
		}
	}

	//	����
	void	Particle::Bom(const Vector3& pos, const float& move, const float& scale, const int& time)
	{
		Vector3	Pos, Move, Power;
		timer++;
		if (timer % time != 0) return;

		for (int j = 0; j<10; j++)
		{
			Pos.x = pos.x + (Random::GetFloat(-move, move) * (0.2f * scale));
			Pos.y = pos.y + (Random::GetFloat( 0.0f, move) * (0.2f * scale));
			Pos.z = pos.z + (Random::GetFloat(-move, move) * (0.2f * scale));

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;

			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			pt->Set(DUST, 0, 1.0f, 60, 0.0f, 40, 0.5f, &Pos, &Move, &Power, 0.8f, 0.0f, 0.0f, scale, RS_COPY);

			if (j <= 8) continue;

			Pos.x = pos.x + (Random::GetFloat(-move, move) * (0.2f * scale));
			Pos.y = pos.y + (Random::GetFloat(0.0f, move) * (0.2f * scale));
			Pos.z = pos.z + (Random::GetFloat(-move, move) * (0.2f * scale));

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power.x = 0.0f;
			Power.y = 0.0f;
			Power.z = 0.0f;

			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			IEX_SetParticle(DUST, 0, 1.0f, 60, 0.0f, 40, 0.5f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.8f, scale, RS_SUB);
		}
	}

	//	��
	void	Particle::Flower( const Vector3& pos, float scale, const Vector3& color)
	{
		Vector3	Pos, Move, Power;

		for ( int i = 0; i < 5; i++ )
		{
			Pos.x = pos.x + sinf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 200 ) - 100 ) * 0.007f * scale;
			Pos.y = pos.y + ( Random::GetInt( 0, 200 ) - 100 ) * 0.007f * scale;
			Pos.z = pos.z + cosf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 200 ) - 100 ) * 0.007f * scale;

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Power = Pos - pos;
			Power.Normalize();
			Power *= 0.001f * scale;

			pt->Set( FLOWER, 0, 0.0f, 30, 0.0f, 20, 1.0f, &Pos, &Move, &Power, color.x, color.y, color.z, scale * 0.1f, RS_COPY );
		}
	}

	//	�Ԃ΂�܂�
	void	Particle::FlowerDisseminate( const Vector3& pos, float scale, float speed, const Vector3& color )
	{
		Vector3	Pos, Move, Power;

		for ( int i = 0; i < 5; i++ )
		{
			Pos.x = pos.x + sinf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 200 ) - 100 ) * 0.007f * scale;
			Pos.y = pos.y;
			Pos.z = pos.z + cosf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 200 ) - 100 ) * 0.007f * scale;

			Move.x = 0.0f;
			Move.y = -0.1f;
			Move.z = 0.0f;

			Power = Pos - pos;
			Power.y = 2.0f;
			Power.Normalize();
			Power *= ( 0.0005f * speed ) * scale;

			pt->Set( FLOWER, 0, 0.5f, 30, 1.0f, 30, 0.5f, &Pos, &Move, &Power, color.x, color.y, color.z, scale * 0.1f, RS_COPY );
		}
	}

	//	�����G�t�F�N�g
	void	Particle::Bomb( const Vector3& pos, float scale, const Vector3& color )
	{
		Vector3	Pos, Move, Power;
		for ( int j = 0; j<10; j++ )
		{
			Pos.x = pos.x + sinf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 100 ) - 50 ) * 0.001f * scale;
			Pos.y = pos.y + sinf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 100 ) - 50 ) * 0.001f * scale;
			Pos.z = pos.z + cosf( Random::GetFloat( 0.0f, 2 * D3DX_PI ) ) * ( Random::GetInt( 0, 100 ) - 50 ) * 0.001f * scale;

			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;

			Vector3	vec = Pos - pos;
			vec.Normalize();
			Power = vec* 0.01f;

			pt->Set( DUST, 0, 1.0f, 30, 1.0f, 60, 0.1f, &Pos, &Move, &Power, color.x, color.y, color.z, scale * 15, RS_COPY);
		}
	}

	//	�R�C���G�t�F�N�g
	void	Particle::CoinGet(const Vector3& pos, const float& scale)
	{
		Vector3	Pos, Move, Power;

		for (int j = 0; j<10; j++)
		{
			Pos.x = pos.x;
			Pos.y = pos.y;
			Pos.z = pos.z;

			Move.x = Random::GetFloat(-50.0f, 50.0f) * (0.005f * scale);
			Move.y = 0.8f * scale + (0.2f * scale * (j % 2));
			Move.z = Random::GetFloat(-20.0f, 20.0f) * (0.02f * scale);

			Power.x = 0.0f;
			Power.y = -(scale * 0.05f);
			Power.z = 0.0f;

			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			pt->Set(COIN, 0, 1.0f, 30, 1.0f, 15, 11.0f, &Pos, &Move, &Power, 0.8f, 0.8f, 0.0f, scale, RS_COPY);
		}
	}

	//	�����񂹃G�t�F�N�g
	void	Particle::Magnet(const Vector3& pos, const float& length, const float& scale, const int& speed)
	{
		Vector3	Pos, Move, Power;
		for (int j = 0; j < 1; j++)
		{
			Vector3 ran_pos, direction;

			direction.x = Random::GetFloat(-1.0f, 1.0f);
			direction.y = Random::GetFloat(0.0f, 1.0f);
			direction.z = Random::GetFloat(-1.0f, 1.0f);

			direction.Normalize();
			
			Pos = pos + direction * length;

			Move = -direction * length / (speed * length);

			Power = Vector3(0.0f, 0.0f, 0.0f);
			//Power = Move * (Random::GetFloat(0.0001f, 0.001f));

			//	

			//	�摜�^�C�v�A�o���t���[���A�o���������x�A�ŏI�t���[���A�ŏI�����x�A�ō��t���[���A�ō������x�A�o���ʒu�A�ړ��l�A�^�́A	�Ԑ����A�ΐ����A�����A�X�P�[���A�����_�[�X�e�[�g
			pt->Set(SUCK, 0, 1.0f, speed * (int)length, 0.0f, speed / 3, 1.0f, &Pos, &Move, &Power, 1.0f, 1.0, 0.2f, scale, RS_COPY);

		}
	}

//------------------------------------------------------------------------
//	���擾
//------------------------------------------------------------------------

	//	���̎擾
	Particle*	Particle::GetInstance( void )
	{
		static	Particle	out;
		return	&out;
	}
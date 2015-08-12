
#include	"iextreme.h"
#include	"system/System.h"
#include	<random>
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"Y2009.h"
#include	"ECCMAN.h"
#include	"Princess.h"

#include	"PlayerManager.h"

//****************************************************************************************
//
//	PlayerManager�N���X
//
//****************************************************************************************

//------------------------------------------------------------------------------
//	�O���[�o��
//------------------------------------------------------------------------------

	//	���̂̐錾
	PlayerManager*		m_Player = NULL;

//------------------------------------------------------------------------------
//	�������E���
//------------------------------------------------------------------------------

	//	�R���X�g���N�^
	PlayerManager::PlayerManager( void )
	{

	}
	
	//	�f�X�g���N�^
	PlayerManager::~PlayerManager( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ ){
			SafeDelete( c_Player[i] );
		}

		for ( int i = 0; i < OBJ_MAX; i++ )
		{
			SafeDelete( org[i] );
		}
	}

	//	������
	void	PlayerManager::Initialize( int input, int type, Vector3 pos )
	{
		switch ( type )
		{
		case	PlayerData::Y2009:
			c_Player[input] = new Y2009();
			break;

		case PlayerData::ECCMAN:
			c_Player[input] = new ECCMAN();
			break;

		case PlayerData::PRINCESS:
			c_Player[input] = new Princess();
			break;

		default:
			c_Player[input] = new Y2009();
			break;
		}

		//	���f���ǂݍ���
		Load();

		//	�����ݒ�
		c_Player[input]->Initialize( input, org[type]->Clone(), pos );


	}

	//	���f���ǂݍ���
	void	PlayerManager::Load( void )
	{
		org[PlayerData::Y2009] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::ECCMAN] = new iex3DObj( "DATA/CHR/ECCMAN/ECCMAN.IEM" );
		org[PlayerData::PRINCESS] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::KNIGHT] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::KING] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::SQUIRREL] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::TIGER] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::ANIMA] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::CROWS] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[PlayerData::BEAR] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
	}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------

	//	�X�V
	void	PlayerManager::Update( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
			//	�e�v���C���[�X�V
			c_Player[i]->Update();
		}

		//	�����蔻��
		HitCheck();
	}

	//	�`��
	void	PlayerManager::Render( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
			c_Player[i]->Render();
			DrawSphere( c_Player[i]->GetPos(), 2.0f, 0xFF000000 );
		}
	}

	//	�V�F�[�_�[�t�`��
	void	PlayerManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
			c_Player[i]->Render( shader, technique );
			Vector3	p_pos = c_Player[i]->GetPos();

			//	�f�o�b�O�p
			if ( !debug )	continue;
			DrawCapsule( p_pos, Vector3( p_pos.x, p_pos.y + 3.0f, p_pos.z ), 1.0f, 0xFFFFFFFF );
			char	str[256];
			sprintf_s( str, "p%d_coin = %d", i + 1, c_Player[i]->GetCoinNum() );
			DrawString( str, 20, 150 + i * 30 );
		}
	}

//------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------

	//	�����蔻��
	void	PlayerManager::HitCheck( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
			for ( int n = 0; n < PLAYER_NUM; n++ )
			{
				//	�����������玟��
				if ( i == n )	continue;
				if ( c_Player[n]->GetUnrivaled() )	continue;
				
				switch ( c_Player[i]->GetAttackParam() )
				{
				case	PlayerData::SPHEREVSCAPSULE:
					HitCheckSphereVSCapsule( c_Player[i], c_Player[n] );
					break;

				case	PlayerData::SPHEREVSSPHERE:
					break;

				case	PlayerData::CAPSULEVSCAPSULE:
					break;

				case	PlayerData::SPHEREVSCYRINDER:
					break;
				}
			}
		}
	}

	//	�R�C�����Z
	void	PlayerManager::AddCoin( int player )
	{
		c_Player[player]->AddCoin();
	}

	//	�R�C�����Z
	void	PlayerManager::SubCoin( int player )
	{
		c_Player[player]->SubCoin();
	}

//------------------------------------------------------------------------------
//	�����蔻��֐�
//------------------------------------------------------------------------------

	//	��VS�J�v�Z��
	void	PlayerManager::HitCheckSphereVSCapsule( Player* sphere, Player* cupsule )
	{
		//	�����蔻��()
		static float	dist = 1.0f;
		float	p1_attack_r = sphere->GetAttack_R();
		Vector3	p1_attackPos = sphere->GetAttackPos();
		Vector3	p2_pos_bottom = cupsule->GetPos();
		Vector3	p2_pos_top = Vector3( p2_pos_bottom.x, p2_pos_bottom.y + 3.0f, p2_pos_bottom.z );

		if ( Collision::CapsuleVSSphere( p2_pos_bottom, p2_pos_top, dist, p1_attackPos, p1_attack_r ) )
		{
			//	�Ƃ肠�����G�t�F�N�g�ƃR�C���o��
			//	�ϐ�������
			std::uniform_real_distribution<float>	vecrand( -1.0f, 1.0f );
			std::uniform_real_distribution<float>	powrand( 0.1f, 0.3f );
			float	effectScale = 1.0f;
			Particle::Spark( p2_pos_bottom, effectScale );
			Vector3	vec = Vector3( vecrand( ran ), 1.0f, vecrand( ran ) );
			vec.Normalize();

			Vector3	knockBackVec = p2_pos_bottom - p1_attackPos;
			knockBackVec.y = p2_pos_bottom.y;
			knockBackVec.Normalize();
			cupsule->SetKnockBackVec( knockBackVec );
			cupsule->SetMode( PlayerData::DAMAGE_STRENGTH );

			if ( cupsule->GetCoinNum() > 0 )
			{
				m_CoinManager->Set( p2_pos_top, vec, powrand( ran ) );
				cupsule->SubCoin();
			}
		}
	}

//------------------------------------------------------------------------------
//	���擾�E�ݒ�
//------------------------------------------------------------------------------

	//	���擾
	Vector3	PlayerManager::GetPos( int player ){	return	c_Player[player]->GetPos();	}
	Matrix	PlayerManager::GetMatrix( int player ){ return	c_Player[player]->GetMatrix(); }
	float		PlayerManager::GetAngle( int player ){ return		c_Player[player]->GetAngle(); }
	bool		PlayerManager::GetUnrivaled( int player ){ return	c_Player[player]->GetUnrivaled(); }
	int			PlayerManager::GetCoinNum( int player ){ return	c_Player[player]->GetCoinNum(); }

	//	���ݒ�
	void		PlayerManager::SetPos( int player, Vector3 pos ){ c_Player[player]->SetPos( pos ); }
	void		PlayerManager::SetPos( int player, float x, float y, float z ){ c_Player[player]->SetPos( x, y, z ); }
	void		PlayerManager::SetAngle( int player, float angle ){ c_Player[player]->SetAngle( angle ); }
	void		PlayerManager::SetScale( int player, float scale ){ c_Player[player]->SetScale( scale ); }
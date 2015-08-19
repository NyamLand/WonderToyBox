
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Coin.h"
#include	"CoinManager.h"
#include	"BaseObj.h"
#include	"Player.h"
#include	"Y2009.h"
#include	"ECCMAN.h"
#include	"Princess.h"
#include	"Squirrel.h"
#include	"Knight.h"
#include	"GameManager.h"

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

		case PlayerData::KNIGHT:
			c_Player[input] = new Knight();
			break;

		case PlayerData::SQUIRREL:
			c_Player[input] = new Squirrel();
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
		org[PlayerData::SQUIRREL] = new iex3DObj( "DATA/CHR/SQUIRREL/SQUIRREL.IEM" );
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
			DrawCapsule( p_pos, Vector3( p_pos.x, p_pos.y + 3.0f, p_pos.z ), 1.0f, 0xFFFFFFFF );
		}
	}

//------------------------------------------------------------------------------
//	����֐�
//------------------------------------------------------------------------------

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

	//	�����蔻��
	void	PlayerManager::HitCheck( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ )
		{
			for ( int n = 0; n < PLAYER_NUM; n++ )
			{
				if ( i == n )	continue;
				int		attackParam = c_Player[i]->GetAttackParam();

				switch ( attackParam )
				{
				case PlayerData::SPHEREVSCAPSULE:
					HitCheckSphereVSCapsule( c_Player[i], c_Player[n] );
					break;

				case PlayerData::CAPSULEVSCAPSULE:
					HitCheckCapsuleVSCapsule( c_Player[i], c_Player[n] );
					break;

				case PlayerData::SPHEREVSCYRINDER:
					break;

				case PlayerData::SPHEREVSPOINT:
					break;

				case PlayerData::SPHEREVSSPHERE:
					break;
				}
			}
		}
	}

	//	���ƃJ�v�Z���̓����蔻��
	void	PlayerManager::HitCheckSphereVSCapsule( Player* p1, Player* p2 )
	{		
		//	�U�������
		Vector3	p1_attackPos		=		p1->GetAttackPos();
		float		p1_attack_r		=		p1->GetAttack_R();

		//	�U��������
		Vector3	p2_bottom = p2->GetPos();
		Vector3	p2_top = Vector3( p2_bottom.x, p2_bottom.y + 3.0f, p2_bottom.z );
		float		p2_r = 1.0f;

		//	�U������
		bool	isHit = Collision::CapsuleVSSphere( p2_bottom, p2_top, p2_r, p1_attackPos, p1_attack_r );
	
		//	�������Ă�����
		if ( isHit )
		{		
			//	�G�t�F�N�g����
			float	effectScale = 1.0f;
			Particle::Spark( p2_top, effectScale );

			//	�m�b�N�o�b�N
			Vector3	knockBackVec = p1_attackPos - p2_bottom;
			knockBackVec.y = p2_bottom.y;
			knockBackVec.Normalize();
			p2->SetKnockBackVec( -knockBackVec );
			p2->SetMode( PlayerData::DAMAGE_STRENGTH );

			//	�R�C���΂�܂������ݒ�
			std::uniform_real_distribution<float>	vecrand( -1.0f, 1.0f );
			Vector3	vec = Vector3( vecrand( ran ), 1.0f, vecrand( ran ) );
			vec.Normalize();

			//	�v���C���[�ԍ��擾�Ƃ΂�܂��p���[�ݒ�
			float	power = 0.2f;
			int		p2_Num = p2->GetP_Num();
			int		p2_coinNum = GameManager::GetCoinNum( p2_Num );

			//	�R�C��������΂΂�܂�
			if ( p2_coinNum > 0 )
			{
				m_CoinManager->Set( p2_top, vec, power );
				GameManager::SubCoin( p2_Num );
			}
		}
	}

	//	�J�v�Z���ƃJ�v�Z���̓����蔻��
	void	PlayerManager::HitCheckCapsuleVSCapsule( Player* p1, Player* p2 )
	{
		//	�U�������
		Vector3	p1_attack_bottom = p1->GetAttackPos_Top();
		Vector3	p1_attack_top = p1->GetAttackPos_Bottom();
		float		p1_attack_r = p1->GetAttack_R();

		//	�U��������
		Vector3	p2_bottom = p2->GetPos();
		Vector3	p2_top = Vector3( p2_bottom.x, p2_bottom.y + 3.0f, p2_bottom.z );
		float		p2_r = 1.0f;

		//	�U������
		bool	isHit = Collision::CapsuleVSCapsule( p1_attack_bottom, p1_attack_top, p1_attack_r, p2_bottom, p2_top, p2_r );

		//	�������Ă�����
		if ( isHit )
		{
			//	�G�t�F�N�g����
			float	effectScale = 1.0f;
			Particle::Spark( p2_top, effectScale );

			//	�m�b�N�o�b�N
			Vector3	knockBackVec = p1_attack_top - p2_bottom;
			knockBackVec.y = p2_bottom.y;
			knockBackVec.Normalize();
			p2->SetKnockBackVec( -knockBackVec );
			p2->SetMode( PlayerData::DAMAGE_STRENGTH );
			
			//	�R�C���΂�܂������ݒ�
			std::uniform_real_distribution<float>	vecrand( -1.0f, 1.0f );
			Vector3	vec = Vector3( vecrand( ran ), 1.0f, vecrand( ran ) );
			vec.Normalize();

			//	�v���C���[�ԍ��擾�Ƃ΂�܂��p���[�ݒ�
			float	power = 0.15f;
			int		p2_Num = p2->GetP_Num();
			int		p2_coinNum = GameManager::GetCoinNum( p2_Num );

			//	�R�C��������΂΂�܂�
			if ( p2_coinNum > 0 )
			{
				m_CoinManager->Set( p2_top, vec, power );
				GameManager::SubCoin( p2_Num );
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
	int			PlayerManager::GetCoinNum( int player ){ return	c_Player[player]->GetCoinNum(); }
	int			PlayerManager::GetType( int player ){ return c_Player[player]->GetType(); }
	bool		PlayerManager::GetUnrivaled( int player ){ return c_Player[player]->GetUnrivaled(); }

	//	���ݒ�
	void		PlayerManager::SetPos( int player, Vector3 pos ){ c_Player[player]->SetPos( pos ); }
	void		PlayerManager::SetPos( int player, float x, float y, float z ){ c_Player[player]->SetPos( x, y, z ); }
	void		PlayerManager::SetAngle( int player, float angle ){ c_Player[player]->SetAngle( angle ); }
	void		PlayerManager::SetScale( int player, float scale ){ c_Player[player]->SetScale( scale ); }
	void		PlayerManager::SetType( int player, int type ){ c_Player[player]->SetType(type); }

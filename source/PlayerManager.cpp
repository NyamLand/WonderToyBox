
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"BaseObj.h"
#include	"Player.h"
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
		for (int i = 0; i < PLAYER_NUM; i++){
			SafeDelete( c_Player[i] );
		}
	}

	//	������
	void	PlayerManager::Initialize( int input, int type, Vector3 pos )
	{
		c_Player[input] = new Player();
		c_Player[input]->Initialize( input, type, pos );
	}

//------------------------------------------------------------------------------
//	�X�V�E�`��
//------------------------------------------------------------------------------

	//	�X�V
	void	PlayerManager::Update( void )
	{
		for ( int i = 0; i < PLAYER_NUM; i++ ){
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
			DrawSphere( c_Player[i]->GetPos(), 2.0f, 0xFF000000 );
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
				if ( c_Player[i]->GetAttackParam() != 0 )
				{
					//	�����������玟��
					if ( i == n )	continue;

					//	�����蔻��(���͂Ƃ肠�������Ŏ��)
					static float	dist = 2.0f;
					if ( Collision::DistCheck( c_Player[i]->GetPos(), c_Player[n]->GetPos(), dist ) )
					{
						//	�Ƃ肠�����G�t�F�N�g�o��
						Vector3	p_pos = c_Player[n]->GetPos();
						float	effectScale = 2.0f;
						Effect1( p_pos.x, p_pos.y, p_pos.z, effectScale );
					}
				}
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

	//	���ݒ�
	void		PlayerManager::SetPos( int player, Vector3 pos ){ c_Player[player]->SetPos( pos ); }
	void		PlayerManager::SetPos( int player, float x, float y, float z ){ c_Player[player]->SetPos( x, y, z ); }
	void		PlayerManager::SetAngle( int player, float angle ){ c_Player[player]->SetAngle( angle ); }
	void		PlayerManager::SetScale( int player, float scale ){ c_Player[player]->SetScale( scale ); }
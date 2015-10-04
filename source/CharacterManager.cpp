
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Random.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Sound.h"
#include	"CoinManager.h"
#include	"CharacterManager.h"

//****************************************************************************************
//
//	CharacterManager�N���X
//
//****************************************************************************************

//-------------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------------

	//	������
	bool	CharacterManager::Initialize( int playerNum, int type, Vector3 pos, bool isPlayer )
	{
		if ( !loadflag )	Load();

		switch ( type )
		{
		case CHARACTER_TYPE::PRINCESS:
			character[playerNum] = new Princess();
			break;

		case CHARACTER_TYPE::KNIGHT:
			character[playerNum] = new Knight();
			break;

		case CHARACTER_TYPE::SQUIRREL:
			character[playerNum] = new Squirrel();
			break;

		case CHARACTER_TYPE::TIGER:
			character[playerNum] = new Tiger();
			break;

		default:
			return	false;
		}

		//	�����ݒ�
		character[playerNum]->Initialize( playerNum, org[type]->Clone(), pos, isPlayer );
		return	true;
	}

	//	���
	void	CharacterManager::Release( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )	SafeDelete( character[i] );
		for ( int i = 0; i < OBJ_MAX; i++ )		SafeDelete( org[i] );
		loadflag = false;
	}

	//	�ǂݍ���
	void	CharacterManager::Load( void )
	{
		loadflag = true;
		org[CHARACTER_TYPE::PRINCESS] = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
		org[CHARACTER_TYPE::KNIGHT] = new iex3DObj( "DATA/CHR/�~�m�^�E���X/minotaurus.IEM" );
		org[CHARACTER_TYPE::SQUIRREL] = new iex3DObj( "DATA/CHR/SQUIRREL/SQUIRREL.IEM" );
		org[CHARACTER_TYPE::TIGER] = new iex3DObj( "DATA/CHR/ECCMAN/ECCMAN.IEM" );
	}

//-------------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------------

	//	�X�V
	void	CharacterManager::Update( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	�e�L�����N�^�[�X�V
			character[i]->Update();
			canHyper = GetCanHyper( i );
		}

		//	�����蔻��
		HitCheck();

		//	�ǂ񂯂u�[�X�g
		DonketsuBoost();
	}

	//	�`��
	void	CharacterManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			character[i]->Render( shader, technique );
			Vector3	p_pos = character[i]->GetPos();

			if ( !debug )continue;
			DrawCapsule( p_pos, Vector3( p_pos.x, p_pos.y + 3.0f, p_pos.z ), 1.0f );

			char str[256];
			wsprintf( str, "p%d�̍U���́F%d", i + 1, GetPower(i) );
			DrawString( str, 200, 200 + i * 20 );
		}
	}

//-------------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------------

	//	�ǂ񂯂u�[�X�g
	void	CharacterManager::DonketsuBoost( void )
	{
		if ( gameManager->GetDonketsuBoostState() )
		{
			//	(���肳�ꂽ)�r�����N�����擾�E�ǂ񂯂��[�h�Z�b�g
			int worst = gameManager->GetWorst();
			SetBoosting( worst, true );

			//	�r�����Ȃ�̃L�����������ʂ��Ă��ꂼ��ɍ������X�e�[�^�X�㏸
			RaiseStatus( worst, gameManager->GetCharacterType( worst ) );
		}
	}

	//	�X�e�[�^�X�㏸
	void	CharacterManager::RaiseStatus( int worst, int type )
	{

	}

//-------------------------------------------------------------------------------------
//	�����蔻��֐�
//-------------------------------------------------------------------------------------

	//	�����蔻��
	void	CharacterManager::HitCheck( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			for ( int n = 0; n < PLAYER_MAX; n++ )
			{
				//	���������肪���G��Ԃ��Ƃ���
				if ( i == n )	continue;
				if ( character[n]->GetUnrivaled() )	continue;

				//	�U���^�C�v���擾
				int		attackParam = character[i]->GetAttackParam();

				//	�^�C�v�ʓ����蔻��
				switch ( attackParam )
				{
				case COLLISION_TYPE::SPHEREVSCAPSULE:
					HitCheckSphereVSCapsule(character[i], character[n]);
					break;

				case COLLISION_TYPE::CAPSULEVSCAPSULE:
					HitCheckCapsuleVSCapsule(character[i], character[n]);
					break;

				case COLLISION_TYPE::SPHEREVSCYRINDER:
					break;

				case COLLISION_TYPE::SPHEREVSPOINT:
					break;

				case COLLISION_TYPE::SPHEREVSSPHERE:
					break;
				}
			}
		}
	}

	//	���ƃJ�v�Z���̓����蔻��
	void	CharacterManager::HitCheckSphereVSCapsule( BaseChara* bc1, BaseChara* bc2 )
	{
		//	�U�������
		Vector3	bc1_attackPos = bc1->GetAttackPos();
		float		bc1_attack_r = bc1->GetAttack_R();

		//	�U��������
		Vector3	bc2_bottom = bc2->GetPos();
		Vector3	bc2_top = Vector3( bc2_bottom.x, bc2_bottom.y + 3.0f, bc2_bottom.z );
		float		bc2_r = 1.0f;

		//	�U������
		bool	isHit = Collision::CapsuleVSSphere( bc2_bottom, bc2_top, bc2_r, bc1_attackPos, bc1_attack_r );

		//	�������Ă�����
		if (isHit)
		{
			//	�G�t�F�N�g����
			float	effectScale = 1.0f;
			particle->Spark( bc2_top, effectScale );

			//	�m�b�N�o�b�N
			Vector3	knockBackVec = bc1_attackPos - bc2_bottom;
			knockBackVec.y = bc2_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = bc1->GetDamageColor();
			bc2->SetPassColor( color );
			bc2->SetKnockBackVec( -knockBackVec );

			switch ( bc1->GetKnockBackType() )
			{
			case KNOCKBACK_TYPE::STRENGTH:
				bc2->SetMode( MODE_STATE::DAMAGE_STRENGTH );
				break;
			case KNOCKBACK_TYPE::MIDDLE:
				bc2->SetMode( MODE_STATE::DAMAGE_MIDDLE );
				break;
			case KNOCKBACK_TYPE::WEAK:
				bc2->SetMode( MODE_STATE::DAMAGE_WEAK );
				break;
			case KNOCKBACK_TYPE::LEANBACKWARD:
				bc2->SetLeanFrame( bc1->GetLeanFrame() );
				bc2->SetMode( MODE_STATE::DAMAGE_LEANBACKWARD );
				break;
			}

			//	�F�ݒ�

			//	�R�C���΂�܂������ݒ�
			Vector3	vec = Vector3( Random::GetFloat( -1.0f, 1.0f ), 1.0f, Random::GetFloat( -1.0f, 1.0f ) );
			vec.Normalize();

			//	�v���C���[�ԍ��擾�Ƃ΂�܂��p���[�ݒ�
			float	power = 0.2f;
			int		bc2_Num = bc2->GetPlayerNum();
			int		bc2_coinNum = gameManager->GetCoinNum( bc2_Num );

			//	�R�C��������΂΂�܂�
			if ( bc2_coinNum > 0 )
			{
				m_CoinManager->Set( bc2_top, vec, power );
				gameManager->SubCoin( bc2_Num );
			}
		}
	}

	//	�J�v�Z���ƃJ�v�Z���̓����蔻��
	void	CharacterManager::HitCheckCapsuleVSCapsule( BaseChara* bc1, BaseChara* bc2 )
	{
		//	�U�������
		Vector3	bc1_attack_bottom = bc1->GetAttackPos_Bottom();
		Vector3	bc1_attack_top = bc1->GetAttackPos_Top();
		float		bc1_attack_r = bc1->GetAttack_R();

		//	�U��������
		Vector3	bc2_bottom = bc2->GetPos();
		Vector3	bc2_top = Vector3( bc2_bottom.x, bc2_bottom.y + 3.0f, bc2_bottom.z );
		float		bc2_r = 1.0f;

		//	�U������
		bool	isHit = Collision::CapsuleVSCapsule( bc1_attack_bottom, bc1_attack_top, bc1_attack_r, bc2_bottom, bc2_top, bc2_r );

		//	�������Ă�����
		if ( isHit )
		{
			//	�G�t�F�N�g����
			float	effectScale = 1.0f;
			particle->Spark( bc2_top, effectScale );

			//	�m�b�N�o�b�N
			Vector3	knockBackVec = bc1_attack_top - bc2_bottom;
			knockBackVec.y = bc2_bottom.y;
			knockBackVec.Normalize();
			Vector3	color = bc1->GetDamageColor();
			bc2->SetPassColor( color );
			bc2->SetKnockBackVec( -knockBackVec );

			switch ( bc1->GetKnockBackType() )
			{
			case KNOCKBACK_TYPE::STRENGTH:
				bc2->SetMode( MODE_STATE::DAMAGE_STRENGTH );
				break;
			case KNOCKBACK_TYPE::MIDDLE:
				bc2->SetMode( MODE_STATE::DAMAGE_MIDDLE );
				break;
			case KNOCKBACK_TYPE::WEAK:
				bc2->SetMode( MODE_STATE::DAMAGE_WEAK );
				break;
			case KNOCKBACK_TYPE::LEANBACKWARD:
				bc2->SetLeanFrame( bc1->GetLeanFrame() );
				bc2->SetMode( MODE_STATE::DAMAGE_LEANBACKWARD );
				break;
			}

			//	�R�C���΂�܂������ݒ�
			Vector3	vec = Vector3( Random::GetFloat( -1.0f, 1.0f ), 1.0f, Random::GetFloat( -1.0f, 1.0f ) );
			vec.Normalize();

			//	�v���C���[�ԍ��擾�Ƃ΂�܂��p���[�ݒ�
			float	power = 0.15f;
			int		bc2_Num = bc2->GetPlayerNum();
			int		bc2_coinNum = gameManager->GetCoinNum( bc2_Num );

			//	�R�C��������΂΂�܂�
			if ( bc2_coinNum > 0 )
			{
				m_CoinManager->Set( bc2_top, vec, power );
				gameManager->SubCoin( bc2_Num );
			}
		}
	}

//-------------------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------------------

	//	�s��擾
	Matrix	CharacterManager::GetMatrix( int player )const
	{
		return	character[player]->GetMatrix();
	}

	//	���W�擾
	Vector3	CharacterManager::GetPos( int player )const
	{
		return	character[player]->GetPos();
	}

	//	�_���[�W���F�擾
	Vector3	CharacterManager::GetDamageColor( int player )const
	{
		return	character[player]->GetDamageColor();
	}

	//	�����擾
	float		CharacterManager::GetAngle( int player )const
	{
		return	character[player]->GetAngle();
	}

	//	�n�C�p�[�A�[�c�g�p�ێ擾
	bool		CharacterManager::GetCanHyper( int player )const
	{
		return	character[player]->GetCanHyper();
	}

	//	���G��Ԏ擾
	bool		CharacterManager::GetUnrivaled( int player )const
	{
		return	character[player]->GetUnrivaled();
	}

	//	�U����ގ擾
	int			CharacterManager::GetAttackParam( int player )const
	{
		return	character[player]->GetAttackParam();
	}

	//	�v���C���[�ԍ��擾
	int			CharacterManager::GetPlayerNum( int player )const
	{
		return	character[player]->GetPlayerNum();
	}

	//	�U���͎擾
	int			CharacterManager::GetPower( int player )const
	{
		return	character[player]->GetPower();
	}

	//	���̎擾
	CharacterManager*	CharacterManager::GetInstance( void )
	{
		static	CharacterManager	out;
		return	&out;
	}

//-------------------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------------------

	//	���W�ݒ�
	void		CharacterManager::SetPos( int player, Vector3 pos )
	{
		character[player]->SetPos( pos );
	}

	//	�����ݒ�
	void		CharacterManager::SetAngle( int player, float angle )
	{
		character[player]->SetAngle( angle );
	}

	//	�X�P�[���ݒ�
	void		CharacterManager::SetScale( int player, float scale )
	{
		character[player]->SetScale( scale );
	}

	//	�m�b�N�o�b�N�����ݒ�
	void		CharacterManager::SetKnockBackVec( int player, Vector3 vec )
	{
		character[player]->SetKnockBackVec( vec );
	}

	//	���[�h�ݒ�
	void		CharacterManager::SetMode( int player, int mode )
	{
		character[player]->SetMode( mode );
	}

	//	�u�[�X�g��Ԏ擾
	void		CharacterManager::SetBoosting( int player, bool boosting )
	{
		character[player]->SetBoosting( boosting );
	}

	//	�����莞�Ԏ擾
	void		CharacterManager::SetLeanFrame( int player, int leanframe )
	{
		character[player]->SetLeanFrame( leanframe );
	}

	//	�n���F�ݒ�
	void		CharacterManager::SetPassColor( int player, Vector3 color )
	{
		character[player]->SetPassColor( color );
	}
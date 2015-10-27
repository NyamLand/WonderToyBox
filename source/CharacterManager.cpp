
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


	//	�R���X�g���N�^
	CharacterManager::CharacterManager( void )
	{

	}

	//	�f�X�g���N�^
	CharacterManager::~CharacterManager( void )
	{

	}

	//	������
	bool	CharacterManager::Initialize( int playerNum, int type, Vector3 pos, bool isPlayer )
	{
		switch ( type )
		{
		case CHARACTER_TYPE::PRINCESS:
			if ( isPlayer )		character[playerNum] = new Princess();
			else					character[playerNum] = new Princess_CPU();
			break;

		case CHARACTER_TYPE::KNIGHT:
			if ( isPlayer )		character[playerNum] = new Knight();
			else					character[playerNum] = new Knight_CPU();
			break;

		case CHARACTER_TYPE::SQUIRREL:
			if ( isPlayer )		character[playerNum] = new Squirrel();
			else					character[playerNum] = new Squirrel_CPU();
			break;

		case CHARACTER_TYPE::TIGER:
			if ( isPlayer )		character[playerNum] = new Tiger();
			else					character[playerNum] = new Tiger_CPU();
			break;
		}

		//	�����ݒ�
		character[playerNum]->Initialize( playerNum, pos, isPlayer );
		return	true;
	}

	//	���
	void	CharacterManager::Release( void )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			SafeDelete( character[i] );
		}
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
			SetRank(i, gameManager->GetRank(i));
		}

		//	�����蔻��
		HitCheck();

		//	�ǂ񂯂u�[�X�g
		DonketsuBoost();
	}

	//	�`��
	void	CharacterManager::Render( iexShader* shader, LPSTR technique )
	{
		DrawString("rank", 1020, 30);
		DrawString("AImode", 1060, 30);

		DrawString("Aimode�ꗗ", 1130, 30);
		DrawString("�O�FATTACK", 1120, 50);
		DrawString("�P�FRUN�i�R�C���j",	1120, 70);
		DrawString("�Q�FGETAWAY�i������j", 1120, 90);
		DrawString("�R�FGUARD", 1120, 110);
		DrawString("�S�FJUMP", 1120, 130);
		DrawString("�T�FWAIT", 1120, 150);
		
		char str[256];
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			character[i]->Render( shader, technique );
			
			//�@�f�o�b�O����
			sprintf_s(str, "%d�o�F%d    %d", i + 1, GetRank(i), GetAIMode(i));
			DrawString(str, 1000, 50 + i * 20);
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
			SetParameterInfo(worst, PARAMETER_STATE::BOOST);

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
			//	�U���^�C�v�擾( �U�����łȂ���������� )
			int	 attackParam = character[i]->GetAttackParam();
			if ( attackParam == 0 )	continue;
			for ( int n = 0; n < PLAYER_MAX; n++ )
			{
				//	���������肪���G��Ԃ��Ƃ���
				if ( i == n )	continue;
				if ( character[n]->GetUnrivaled() )	continue;

				//	�^�C�v�ʓ����蔻��
				switch ( attackParam )
				{
				case COLLISION_TYPE::SPHEREVSCAPSULE:
					HitCheckSphereVSCapsule( character[i], character[n] );
					break;

				case COLLISION_TYPE::CAPSULEVSCAPSULE:
					HitCheckCapsuleVSCapsule( character[i], character[n] );
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
		if ( isHit )
		{
			if ( bc2->GetUnrivaled() )	return;
			bc2->SetUnrivaled( true );
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

			SetKnockBackParam(bc1, bc2);

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
			if (bc1->GetMode() == MODE_STATE::HYPERARTS){};
			//	���G��Ԏ擾�E�ݒ�
			if ( bc2->GetUnrivaled() )	return;
			bc2->SetUnrivaled( true );

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


			SetKnockBackParam(bc1, bc2);

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

	//	�X���b�v��Ԏ擾
	bool		CharacterManager::GetParameterState( int player, int type )const
	{
		return	character[player]->GetParameterState( type );
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

	//�@���ʎ擾
	int			CharacterManager::GetRank( int player )const
	{
		return character[player]->GetRank();
	}

	//�@���[�h�擾
	int			CharacterManager::GetMode( int player )const
	{
		return character[player]->GetMode();
	}
	int			CharacterManager::GetAIMode(int player)const
	{
		return character[player]->GetAIMode();
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
	void		CharacterManager::SetAIMode( int player, int mode )
	{
		character[player]->SetAIMode(mode);
	}

	//	�u�[�X�g��Ԏ擾
	/*void		CharacterManager::SetBoosting( int player, bool boosting )
	{
		character[player]->SetBoosting( boosting );
	}*/

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

	//	�p�����[�^���ݒ�
	void		CharacterManager::SetParameterInfo( int player, int parameterInfo )
	{
		character[player]->SetParameterState( parameterInfo );
	}

	//    ���ʐݒ�
	void        CharacterManager::SetRank(int player, int rank)
	{
		character[player]->SetRank(rank);
	}

	//�m�b�N�o�b�N���ݒ�
	void		CharacterManager::SetKnockBackParam(BaseChara* bc1, BaseChara* bc2)
	{
		switch (bc1->GetKnockBackType())
		{
		case KNOCKBACK_TYPE::STRENGTH:
			bc2->SetForce(3.0f);
			if (bc1->GetKnockBackIsUp()) bc2->SetMode(MODE_STATE::DAMAGE_FLYUP);
			else bc2->SetMode(MODE_STATE::DAMAGE);
			break;

		case KNOCKBACK_TYPE::MIDDLE:
			bc2->SetForce(2.0f);
			if (bc1->GetKnockBackIsUp()) bc2->SetMode(MODE_STATE::DAMAGE_FLYUP);
			else bc2->SetMode(MODE_STATE::DAMAGE);
			break;

		case KNOCKBACK_TYPE::WEAK:
			bc2->SetForce(1.0f);
			if (bc1->GetKnockBackIsUp()) bc2->SetMode(MODE_STATE::DAMAGE_FLYUP);
			else bc2->SetMode(MODE_STATE::DAMAGE);
			break;

		case KNOCKBACK_TYPE::LEANBACKWARD:
			bc2->SetLeanFrame(bc1->GetLeanFrame());
			bc2->SetMode(MODE_STATE::DAMAGE_LEANBACKWARD);
			break;
		}
	}
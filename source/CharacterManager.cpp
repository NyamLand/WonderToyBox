
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Random.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Sound.h"
#include	"Stage.h"
#include	"CoinManager.h"
#include	"Particle.h"
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

		case CHARACTER_TYPE::SCAVENGER:
			if ( isPlayer )		character[playerNum] = new Scavenger();
			else					character[playerNum] = new Scavenger_CPU();
			break;

		case CHARACTER_TYPE::THIEF:
			if ( isPlayer )		character[playerNum] = new Thief();
			else					character[playerNum] = new Thief_CPU();
			break;

		case CHARACTER_TYPE::PIRATE:
			if ( isPlayer )		character[playerNum] = new Pirate();
			else					character[playerNum] = new Pirate_CPU();
			break;
		}

		//	�����ݒ�
		character[playerNum]->SetLife( gameManager->GetStartLife( playerNum ) );
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
		//PlayerDistCheck();
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			//	�e�L�����N�^�[�X�V
			character[i]->Update();
			canHyper = GetCanHyper( i );
			SetRank(i, gameManager->GetRank(i));
		}

		//	�����蔻��
		HitCheck();

		//	�v���C���[�ʒu����

		//	�ǂ񂯂u�[�X�g
		DonketsuBoost();
	}

	//	�`��
	void	CharacterManager::Render( iexShader* shader, LPSTR technique )
	{
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			character[i]->Render( shader, technique );	
		}

		Vector3	pos = character[0]->GetPos();
		printf( "pos.x = %f pos.y = %f pos.z = %f\n", pos.x, pos.y, pos.z );
		
	}

	//�@�f�o�b�O
	void	CharacterManager::DrawDebug( void )
	{
		if (debug)
		{
			char str[256];

			//--------------------------------------------
			//�@�\��
			//--------------------------------------------
			//�@AI�ꗗ�\
			DrawString("rank", 1020, 30);
			DrawString("AImode", 1060, 30);
			DrawString("Aimode�ꗗ", 1130, 30);
			DrawString("�O�FATTACK", 1120, 50);
			DrawString("�P�FRUN�i�R�C���j", 1120, 70);
			DrawString("�Q�FGETAWAY�i������j", 1120, 90);
			DrawString("�R�FGUARD", 1120, 110);
			DrawString("�S�FJUMP", 1120, 130);
			DrawString("�T�FWAIT", 1120, 150);
			//	power�Espeed
			DrawString("tPower    tSpeed", 50, 50);

			//--------------------------------------------
			//�@�S�l���̃p�����[�^
			//--------------------------------------------
			for (int i = 0; i < PLAYER_MAX; i++)
			{
				//�@AImode�Erank
				sprintf_s(str, "%d�o�F%d    %d", i + 1, GetRank(i), GetAIMode(i));
				DrawString(str, 1000, 50 + i * 20);
				//�@totalPower�EtotalSpeed
				sprintf_s(str, "%d�o�F%d    %f", i + 1, GetTotalPower(i), GetTotalSpeed(i));
				DrawString(str, 600, 70 + i * 20);
			}
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
			SetParameterInfo( worst, PARAMETER_STATE::BOOST );

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

			//	�I�u�W�F�N�g�ւ̓����蔻��
			switch ( attackParam )
			{
			case	Collision::SPHEREVSCAPSULE:
				HitCheckObjectSphere( character[i] );
				break;

			case Collision::CAPSULEVSCAPSULE:
				HitCheckObjectCapsule( character[i] );
				break;
			}

			//	�v���C���[�ւ̓����蔻��
			for ( int n = 0; n < PLAYER_MAX; n++ )
			{
				//	���������肪���G��Ԃ��Ƃ���
				if ( i == n )	continue;
				if ( character[n]->GetParameterState( PARAMETER_STATE::UNRIVALED ) )			continue;
				if ( character[n]->GetAttackParam() == Collision::NONE )	continue;

				//	�^�C�v�ʓ����蔻��
				switch ( attackParam )
				{
				case Collision::SPHEREVSCAPSULE:
					HitCheckSphereVSCapsule( character[i], character[n] );
					break;

				case Collision::CAPSULEVSCAPSULE:
					HitCheckCapsuleVSCapsule( character[i], character[n] );
					break;

				case Collision::CAPSULEVSCYRINDER:
					HitCheckCapsuleVSCyrinder( character[i], character[n] );
					break;

				case Collision::SPHEREVSCYRINDER:
					break;

				case Collision::SPHEREVSPOINT:
					break;

				case Collision::SPHEREVSSPHERE:
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
		float		bc2_r = 2.0f;

		//	�U������
		bool	isHit = Collision::CapsuleVSSphere( bc2_bottom, bc2_top, bc2_r, bc1_attackPos, bc1_attack_r );

		//	�������Ă�����
		if ( isHit )
		{
			int bc1Mode = bc1->GetMode();
			int bc2Mode = bc2->GetMode();
			bool	bc2Shield = bc2->GetParameterState( PARAMETER_STATE::UNRIVALEDITEM );
			if (bc1Mode == MODE_STATE::HYPERARTS)
			{
				//���������u�Ԃɉ�ʗh�炷�A�~�߂�
				gameManager->SetShakeCamera(1.5f, 30);
				//��ʒ�~
				gameManager->SetTimeStop(SCREEN_STOPTIME);
			}
			if ( bc2Mode == MODE_STATE::GUARD || bc2Shield )
			{
				sound->PlaySE( SE::GUARD_SE );
				return;
			}
			
			//	���G�ɂ���
			if ( bc2->GetParameterState( PARAMETER_STATE::UNRIVALED ) )	return;
			bc2->SetParameterState( PARAMETER_STATE::UNRIVALED );

			//	������ʕt�^
			int addParam = bc1->GetAttack_addParam();
			bc2->SetParameterState( addParam );

			//�T�E���h�Đ�
			int attackMode = bc1->GetMode();
			switch ( attackMode )
			{
			case MODE_STATE::QUICKARTS:
				sound->PlaySE( SE::QUICK_HIT_SE );
				break;

			case MODE_STATE::POWERARTS:
				sound->PlaySE( SE::POWER_HIT_SE );
				break;

			case MODE_STATE::HYPERARTS:
				sound->PlaySE( SE::HYPER_HIT_SE );
				break;
			}
			//	���C�t���炷
			int power = bc1->GetPower();
			FOR(0, power) bc2->SubLife();

			//	�G�t�F�N�g����
			float	effectScale = 1.0f;
			particle->Spark( bc2_top, effectScale );

			//	�m�b�N�o�b�N
			if (bc2->GetKnockBackType() != KNOCKBACK_TYPE::NONE)
			{
				Vector3	knockBackVec = bc1_attackPos - bc2_top;
				knockBackVec.y = bc2_top.y;
				knockBackVec.Normalize();
				bc2->SetKnockBackVec( -knockBackVec );
				SetKnockBackParam(bc1, bc2);
			}
			//����F�ݒ�
			Vector3	color = bc1->GetDamageColor();
			if ( addParam == PARAMETER_STATE::CONFUSION )		color = Vector3( 0.0f, 1.0f, 0.0f );
			else if ( power == 0 )														color = Vector3( 0.5f, 0.5f, 0.0f );
			else																					color = Vector3( 1.0f, 0.0f, 0.0f );
			bc2->SetPassColor( color );
			//	�R�C���΂�܂������ݒ�
			Vector3	vec = Vector3( Random::GetFloat( -1.0f, 1.0f ), 1.0f, Random::GetFloat( -1.0f, 1.0f ) );
			vec.Normalize();

			//	�v���C���[�ԍ��擾�Ƃ΂�܂��p���[�ݒ�
			//float	power = 0.2f;
			float	totalpower = bc1->GetTotalPower()*0.1f;
			int		dropCoin = bc1->GetTotalPower();
			int		bc1_Num = bc1->GetPlayerNum();
			int		bc2_Num = bc2->GetPlayerNum();
			int		bc2_coinNum = gameManager->GetCoinNum( bc2_Num );


			int dropType = bc1->GetDropType();
			switch (dropType)
			{
			case DROP_TYPE::DROP:
				//	�U������totalPower���̃R�C�����΂�܂�
				for (int i = 0; i < dropCoin; i++)
				{
					if (bc2_coinNum <= 0) break;
					coinManager->Append(bc2_top, vec, totalpower, Coin::COIN);
					gameManager->SubCoin(bc2_Num);

				}
				break;
			case DROP_TYPE::SUCTION:
				for (int i = 0; i < dropCoin; i++)
				{
					bc2_coinNum = gameManager->GetCoinNum(bc2_Num);
					if (bc2_coinNum <= 0) break;
					gameManager->SubCoin(bc2_Num);
					gameManager->AddCoin(bc1_Num);
				}
				break;
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
		float		bc2_r = 2.0f;

		//	�U������
		bool	isHit = Collision::CapsuleVSCapsule( bc1_attack_bottom, bc1_attack_top, bc1_attack_r, bc2_bottom, bc2_top, bc2_r );


		//	�������Ă�����
		if ( isHit )
		{
			int	bc1Mode = bc1->GetMode();
			if (bc1Mode == MODE_STATE::HYPERARTS)
			{
				//���������u�Ԃɉ�ʗh�炷�A�~�߂�
				gameManager->SetShakeCamera(1.5f, 30);
				//��ʒ�~
				gameManager->SetTimeStop(SCREEN_STOPTIME);
			}

			if ( bc1Mode == MODE_STATE::GUARD)
			{
				sound->PlaySE(SE::GUARD_SE);
				return;
			}

			//	���G�ɂ���
			if (bc2->GetParameterState(PARAMETER_STATE::UNRIVALED))	return;
			bc2->SetParameterState(PARAMETER_STATE::UNRIVALED);
			//�T�E���h�Đ�
			int attackMode = bc1->GetMode();
			switch ( attackMode )
			{
			case MODE_STATE::QUICKARTS:
				sound->PlaySE(SE::QUICK_HIT_SE);
				break;

			case MODE_STATE::POWERARTS:
				sound->PlaySE(SE::POWER_HIT_SE);
				break;

			case MODE_STATE::HYPERARTS:
				sound->PlaySE(SE::HYPER_HIT_SE);
				break;
			}
			//	���C�t���炷
			int power = bc1->GetPower();
			FOR(0, power) bc2->SubLife();

			//	�G�t�F�N�g����
			float	effectScale = 1.0f;
			particle->Spark( bc2_top, effectScale );

			//	�m�b�N�o�b�N
			if (bc2->GetKnockBackType() != KNOCKBACK_TYPE::NONE)
			{
				Vector3	knockBackVec = bc1_attack_top - bc2_top;
				knockBackVec.y = bc2_top.y;
				knockBackVec.Normalize();
				bc2->SetKnockBackVec( -knockBackVec );
				SetKnockBackParam(bc1, bc2);
			}

			//����F�ݒ�
			Vector3	color = bc1->GetDamageColor();
			if ( power == 0 )														color = Vector3( 1.0f, 1.0f, 0.0f );
			else																			color = Vector3( 1.0f, 0.0f, 0.0f );
			bc2->SetPassColor( color );

			//	�R�C���΂�܂������ݒ�
			Vector3	vec = Vector3( Random::GetFloat( -1.0f, 1.0f ), 1.0f, Random::GetFloat( -1.0f, 1.0f ) );
			vec.Normalize();

			//	�v���C���[�ԍ��擾�Ƃ΂�܂��p���[�ݒ�
			//float	power = 0.15f;
			float	totalpower = bc1->GetTotalPower()*0.1f;
			int		dropCoin = bc1->GetTotalPower();
			int		bc1_Num = bc1->GetPlayerNum();
			int		bc2_Num = bc2->GetPlayerNum();
			int		bc2_coinNum = gameManager->GetCoinNum(bc2_Num);

			int dropType = bc1->GetDropType();
			switch (dropType)
			{
			case DROP_TYPE::DROP:
				//	�U������totalPower���̃R�C�����΂�܂�
				for (int i = 0; i < dropCoin; i++)
				{
					if (bc2_coinNum <= 0) break;
					coinManager->Append(bc2_top, vec, totalpower, Coin::COIN);
					gameManager->SubCoin(bc2_Num);

				}
				break;
			case DROP_TYPE::SUCTION:
				for (int i = 0; i < dropCoin; i++)
				{
					bc2_coinNum = gameManager->GetCoinNum(bc2_Num);
					if (bc2_coinNum <= 0) break;
					gameManager->SubCoin(bc2_Num);
					gameManager->AddCoin(bc1_Num);
				}
				break;
			}
		}
	}

	//	�J�v�Z���Ɖ~���̓����蔻��
	void	CharacterManager::HitCheckCapsuleVSCyrinder( BaseChara* bc1, BaseChara* bc2 )
	{
		//	�U�������
		Vector3	bc1_attack_bottom = bc1->GetAttackPos_Bottom();
		Vector3	bc1_attack_top = bc1->GetAttackPos_Top();
		float		bc1_attack_r = bc1->GetAttack_R();

		//	�U��������
		Vector3	bc2_bottom = bc2->GetPos();
		Vector3	bc2_top = Vector3(bc2_bottom.x, bc2_bottom.y + 3.0f, bc2_bottom.z);
		float		bc2_r = 2.0f;

		//	�U������
		bool	isHit = Collision::CapsuleVSCyrinder( bc1_attack_bottom, bc1_attack_top, bc1_attack_r, bc2_bottom, bc2_top, bc2_r );

		//	�������Ă�����
		if (isHit)
		{
			int	bc1Mode = bc1->GetMode();
			if (bc1Mode == MODE_STATE::HYPERARTS)
			{
				//���������u�Ԃɉ�ʗh�炷�A�~�߂�
				gameManager->SetShakeCamera(1.5f, 30);
				//��ʒ�~
				gameManager->SetTimeStop(SCREEN_STOPTIME);
			}

			if (bc1Mode == MODE_STATE::GUARD)
			{
				sound->PlaySE(SE::GUARD_SE);
				return;
			}

			//	���G�ɂ���
			if (bc2->GetParameterState(PARAMETER_STATE::UNRIVALED))	return;
			bc2->SetParameterState(PARAMETER_STATE::UNRIVALED);

			//	������ʕt�^
			int addParam = bc1->GetAttack_addParam();
			bc2->SetParameterState(addParam);

			//�T�E���h�Đ�
			int attackMode = bc1->GetMode();
			switch (attackMode)
			{
			case MODE_STATE::QUICKARTS:
				sound->PlaySE(SE::QUICK_HIT_SE);
				break;

			case MODE_STATE::POWERARTS:
				sound->PlaySE(SE::POWER_HIT_SE);
				break;

			case MODE_STATE::HYPERARTS:
				sound->PlaySE(SE::HYPER_HIT_SE);
				break;
			}
			//	���C�t���炷
			int power = bc1->GetPower();
			FOR(0, power) bc2->SubLife();

			//	�G�t�F�N�g����
			float	effectScale = 1.0f;
			particle->Spark(bc2_top, effectScale);

			//	�m�b�N�o�b�N
			if (bc2->GetKnockBackType() != KNOCKBACK_TYPE::NONE)
			{
				Vector3	knockBackVec = bc1_attack_top - bc2_top;
				knockBackVec.y = bc2_top.y;
				knockBackVec.Normalize();
				bc2->SetKnockBackVec(-knockBackVec);
				SetKnockBackParam(bc1, bc2);
			}

			//����F�ݒ�
			Vector3	color = bc1->GetDamageColor();
			if ( power == 0 )														color = Vector3( 1.0f, 1.0f, 0.0f );
			else																			color = Vector3( 1.0f, 0.0f, 0.0f );
			bc2->SetPassColor( color );

			//	�R�C���΂�܂������ݒ�
			Vector3	vec = Vector3(Random::GetFloat(-1.0f, 1.0f), 1.0f, Random::GetFloat(-1.0f, 1.0f));
			vec.Normalize();

			//	�v���C���[�ԍ��擾�Ƃ΂�܂��p���[�ݒ�
			//float	power = 0.15f;
			float	totalpower = bc1->GetTotalPower()*0.1f;
			int		dropCoin = bc1->GetTotalPower();
			int		bc1_Num = bc1->GetPlayerNum();
			int		bc2_Num = bc2->GetPlayerNum();
			int		bc2_coinNum = gameManager->GetCoinNum(bc2_Num);

			int dropType = bc1->GetDropType();
			switch (dropType)
			{
			case DROP_TYPE::DROP:
				//	�U������totalPower���̃R�C�����΂�܂�
				for (int i = 0; i < dropCoin; i++)
				{
					if (bc2_coinNum <= 0) break;
					coinManager->Append(bc2_top, vec, totalpower, Coin::COIN);
					gameManager->SubCoin(bc2_Num);

				}
				break;
			case DROP_TYPE::SUCTION:
				for (int i = 0; i < dropCoin; i++)
				{
					bc2_coinNum = gameManager->GetCoinNum(bc2_Num);
					if (bc2_coinNum <= 0) break;
					gameManager->SubCoin(bc2_Num);
					gameManager->AddCoin(bc1_Num);
				}
				break;
			}
		}
	}

	//	�I�u�W�F�N�g�ւ̍U��
	void	CharacterManager::HitCheckObjectCapsule( BaseChara* bc )
	{
		//	�U�������
		Vector3	bc_pos = bc->GetPos();
		Vector3	bc_attack_bottom = bc->GetAttackPos_Bottom();
		Vector3	bc_attack_top = bc->GetAttackPos_Top();
		float			bc_attack_r = bc->GetAttack_R();

		//	����p
		float			workFront = 0.0f;
		float			workBack = 0.0f;
		float			workRight = 0.0f;
		float			workLeft = 0.0f;
		Vector3	hitPos = Vector3( 0.0f, 0.0f, 0.0f );
		int			outId = 0;
		
		//	�S�����擾
		workFront = stage->GetFrontToObject( bc_pos, hitPos, outId );
		
		bool	isHit = false;
		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::CapsuleVSSphere( bc_attack_top, bc_attack_bottom, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	�ϋv�l����
				stage->SubDurableValue( outId );

				//	���G�ɂ���
				stage->SetUnrivaled( outId, true );
			}
		}

		workBack = stage->GetBackToObject( bc_pos, hitPos, outId );

		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::CapsuleVSSphere( bc_attack_top, bc_attack_bottom, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	�ϋv�l����
				stage->SubDurableValue( outId );

				//	���G�ɂ���
				stage->SetUnrivaled( outId, true );
			}
		}

		workRight = stage->GetRightToObject( bc_pos, hitPos, outId );

		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::CapsuleVSSphere( bc_attack_top, bc_attack_bottom, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	�ϋv�l����
				stage->SubDurableValue( outId );

				//	���G�ɂ���
				stage->SetUnrivaled( outId, true );
			}
		}

		workLeft = stage->GetLeftToObject( bc_pos, hitPos, outId );

		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::CapsuleVSSphere( bc_attack_top, bc_attack_bottom, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	�ϋv�l����
				stage->SubDurableValue( outId );

				//	���G�ɂ���
				stage->SetUnrivaled( outId, true );
			}
		}
	}

	//	�I�u�W�F�N�g�ւ̍U��
	void	CharacterManager::HitCheckObjectSphere( BaseChara* bc )
	{
		//	�U�������
		Vector3	bc_pos = bc->GetPos();
		Vector3	bc_attackPos = bc->GetAttackPos();
		float			bc_attack_r = bc->GetAttack_R();


		//	����p
		float			workFront = 0.0f;
		float			workBack = 0.0f;
		float			workRight = 0.0f;
		float			workLeft = 0.0f;
		Vector3	hitPos = Vector3( 0.0f, 0.0f, 0.0f );
		int			outId = 0;

		//	�S�����擾
		workFront = stage->GetFrontToObject( bc_pos, hitPos, outId );

		bool	isHit = false;
		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::SphereVSSphere( bc_attackPos, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	�G�t�F�N�g
				particle->BlueFlame( hitPos, 1.0f );

				//	�ϋv�l����
				stage->SubDurableValue( outId );

				//	���G�ɂ���
				stage->SetUnrivaled( outId, true );
			}
		}

		workBack = stage->GetBackToObject( bc_pos, hitPos, outId );

		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::SphereVSSphere( bc_attackPos, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	�G�t�F�N�g
				particle->BlueFlame( hitPos, 1.0f );

				//	�ϋv�l����
				stage->SubDurableValue( outId );

				//	���G�ɂ���
				stage->SetUnrivaled( outId, true );
			}
		}

		workRight = stage->GetRightToObject( bc_pos, hitPos, outId );

		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::SphereVSSphere( bc_attackPos, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	�G�t�F�N�g
				particle->BlueFlame( hitPos, 1.0f );

				//	�ϋv�l����
				stage->SubDurableValue( outId );

				//	���G�ɂ���
				stage->SetUnrivaled( outId, true );
			}
		}

		workLeft = stage->GetLeftToObject( bc_pos, hitPos, outId );

		if ( !stage->GetUnrivaled( outId ) )
		{
			isHit = Collision::SphereVSSphere( bc_attackPos, bc_attack_r, hitPos, 2.0f );

			if ( isHit )
			{
				//	�G�t�F�N�g
				particle->BlueFlame( hitPos, 1.0f );

				//	�ϋv�l����
				stage->SubDurableValue( outId );

				//	���G�ɂ���
				stage->SetUnrivaled( outId, true );
			}
		}
	}
	
	//	�v���C���[�ƃv���C���[�̈ʒu����
	void	CharacterManager::PlayerDistCheck( void )
	{
		Vector3	pos1, pos2;
		Vector3	vec;
		float			length;

		FOR( 0, PLAYER_MAX )
		{
			for ( int i = 0; i < PLAYER_MAX; i++ )
			{
				//	�������m�̓X�L�b�v
				if ( value == i )		continue;

				pos1 = character[value]->GetPos();
				pos2 = character[i]->GetPos();

				vec = pos2 - pos1;
				length = vec.Length();

				//	�߂���Ή�������
				if ( length < 4.0f )
				{
					vec.Normalize();
					vec *= -2.0f;
					character[value]->SetPos( pos1 + vec );
				}
			}
		}
	}
	

//-------------------------------------------------------------------------------------
//	���C�t����
//-------------------------------------------------------------------------------------
void	CharacterManager::AddLife(int player)const
{
	character[player]->AddLife();
}
void	CharacterManager::SubLife(int player)const
{
	character[player]->SubLife();
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

	//	�O���擾
	Vector3	CharacterManager::GetFront( int player )const
	{
		return	character[player]->GetFront();
	}

	//	����擾
	Vector3	CharacterManager::GetUp( int player )const
	{
		return	character[player]->GetUp();
	}

	//	�E���擾
	Vector3	CharacterManager::GetRight( int player )const
	{
		return	character[player]->GetRight();
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
		return	character[player]->GetParameterState(PARAMETER_STATE::UNRIVALED);
	}

	//	�X���b�v��Ԏ擾
	bool		CharacterManager::GetParameterState( int player, int type )const
	{
		return	character[player]->GetParameterState( type );
	}

	//	�W�����v�t���O
	bool		CharacterManager::GetJumpState( int player )const
	{
		return	character[player]->GetJumpFlag();
	}

	//	�R�C�������Ԏ擾
	bool		CharacterManager::GetCoinUnrivaled( int player )const
	{
		return	character[player]->GetCoinUnrivaled();
	}

	//�@�v���C���[�ł��邩���擾
	bool		CharacterManager::GetIsPlayer(int player)const
	{
		return	character[player]->GetIsPlayer();
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

	//	�U���͍��v�擾
	int			CharacterManager::GetTotalPower( int player )const
	{
		return	character[player]->GetTotalPower();
	}

	//�@�X�s�[�h�擾
	float		CharacterManager::GetTotalSpeed( int player )const
	{
		return	character[player]->GetTotalSpeed();
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
	
	//	AI�p���[�h�擾
	int			CharacterManager::GetAIMode( int player )const
	{
		return character[player]->GetAIMode();
	}

	//	���C�t�擾
	int			CharacterManager::GetLife( int player )const
	{
		return	character[player]->GetLife();
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

	//	AI�p���[�h�ݒ�
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

	//	�m�b�N�o�b�N�͐ݒ�
	void		CharacterManager::SetForce(int player, float force)
	{
		character[player]->SetForce(force);
	}

	//�m�b�N�o�b�N���ݒ�
	void		CharacterManager::SetKnockBackParam( BaseChara* bc1, BaseChara* bc2)
	{
		switch (bc1->GetKnockBackType())
		{
		case KNOCKBACK_TYPE::STRENGTH:
			bc2->SetForce(1.5f);
			(bc2->GetIsPlayer()) ?
				bc2->SetMode(MODE_STATE::DAMAGE) :
				bc2->SetAIMode(AI_MODE_STATE::DAMAGE);
			break;

		case KNOCKBACK_TYPE::MIDDLE:
			bc2->SetForce(1.0f);
			(bc2->GetIsPlayer()) ?
				bc2->SetMode(MODE_STATE::DAMAGE) :
				bc2->SetAIMode(AI_MODE_STATE::DAMAGE);
			break;

		case KNOCKBACK_TYPE::WEAK:
			bc2->SetForce(0.5f);
			(bc2->GetIsPlayer()) ?
				bc2->SetMode(MODE_STATE::DAMAGE) :
				bc2->SetAIMode(AI_MODE_STATE::DAMAGE);
			break;

		case KNOCKBACK_TYPE::LEANBACKWARD:
			bc2->SetLeanFrame(bc1->GetLeanFrame());
			bc2->SetMode(MODE_STATE::DAMAGE_LEANBACKWARD);
			break;
		}
	}

	//	�̗͐ݒ�
	void		CharacterManager::SetLife( int player, int life )
	{
		character[player]->SetLife( life );
	}
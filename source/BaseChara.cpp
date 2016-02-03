
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Collision.h"
#include	"Particle.h"
#include	"Camera.h"
#include	"EventManager.h"
#include	"Effect.h"
#include	"Sound.h"
#include	"Stage.h"

#include	"BaseChara.h"
#include	"CharacterManager.h"

//*******************************************************************************
//
//	BaseChara�N���X
//
//*******************************************************************************

//----------------------------------------------------------------------------
//	�O���[�o��
//----------------------------------------------------------------------------

#define	MIN_INPUT_STATE	300	//	�X�e�B�b�N����ŏ��l
#define	MIN_SLIP_LENGTH	0.01f	//	���蒷���ŏ��l
#define	SLIP_TIMER_MAX		300	
#define	JUMP_POWER		0.08f	//	�W�����v��
#define	WALL_DIST			2.5f		//	�ǂƂ̋���
namespace
{

	//	��_���[�W�p�F
	namespace
	{
		//	��_���[�W�p�e�F
		const Vector3	DAMAGE_COLOR[] =
		{
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(1.0f, 0.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(0.0f, 0.0f, 1.0f),
		};
	}
}

//----------------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------------

	//	�R���X�g���N�^
	BaseChara::BaseChara( void ) : obj( nullptr ), input( nullptr ),		//	pointer
		pos( 0.0f, 0.0f, 0.0f ), move( 0.0f, 0.0f, 0.0f ), angle( 0.0f, 0.0f, 0.0f ), objectMove( 0.0f, 0.0f, 0.0f ),	//	Vector3
		scale(0.0f), speed(0.0f),	totalSpeed(0.0f), drag(0.0f), force( 0.0f ), moveVec( 0.0f ), jumpPower( 0.0f ), dt( 0.0f ),	//	float
		isGround(false), isPlayer(false), jumpState( true ), checkWall(false), renderflag(true), coinUnrivaled(false),//	bool
		mode(0), playerNum(0), power(0), totalPower(0), leanFrame(0), jumpStep(0),damageStep(0),rank(0), life( 0 )		//	int
	{
	
	}

	//	�f�X�g���N�^
	BaseChara::~BaseChara( void )
	{
		Release();
	}

	//	������
	bool	BaseChara::Initialize( int playerNum, Vector3 pos, bool isPlayer )
	{
		//	�v���C���[�Ȃ�R���g���[���o�^
		this->isPlayer = isPlayer;
		if ( this->isPlayer )		this->input = ::input[playerNum];

		//	�p�����[�^������
		Initialize( playerNum, pos );

		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Update();

		if ( obj == nullptr )	return false;
		return	true;
	}

	//	������
	bool	BaseChara::Initialize( int playerNum, Vector3 pos )
	{
		//	���f���ݒ�
		obj = nullptr;
		
		//	�v���C���[�ԍ��o�^
		this->playerNum = playerNum;

		//	�p�����[�^������
		mode = MODE_STATE::WAIT;
		this->pos = pos;
		angle = Vector3( 0.0f, 0.0f, 0.0f );
		totalPower = power;
		totalSpeed = speed;

		//	�\���̏�����
		{
			//	�U����񏉊���
			{
				attackInfo.type = 0;
				attackInfo.bottom = Vector3( 0.0f, 0.0f, 0.0f );
				attackInfo.top = Vector3( 0.0f, 0.0f, 0.0f );
				attackInfo.pos = Vector3( 0.0f, 0.0f, 0.0f );
				attackInfo.Interval = 0;
				attackInfo.addParam = -1;
				attackInfo.r = 0.0f;
				attackInfo.t = 0.0f;
			}

			//	�m�b�N�o�b�N��񏉊���
			{
				knockBackInfo.type = 0;
				knockBackInfo.vec = Vector3(0.0f, 0.0f, 0.0f);
		}

			//	�_���[�W���F��񏉊���
			{
				damageColor.catchColor = Vector3( 0.0f, 0.0f, 0.0f );
				damageColor.passColor = DAMAGE_COLOR[playerNum];
			}

			//	�p�����[�^��ԏ�����
			ParameterInfoInitialize();

			//	AI��񏉊���
			{
				aiInfo.mode = AI_MODE_STATE::WAIT;
				aiInfo.param = 0;
				aiInfo.act_flag = false;
				aiInfo.step_autorun = 0;
				aiInfo.count_wait		= 30;
				aiInfo.count_run		= 3 * SECOND;
				aiInfo.count_runaway	= 3 * SECOND;
				aiInfo.count_attack		= 1 * SECOND;
				aiInfo.count_guard		= 1 * SECOND;
			}

			//	slip��񏉊���
			{
				slipInfo.speed = 0.003f;
				slipInfo.drag = 0.99f;
			}

			//	�p�����[�^���Z���\���̏�����
			{
				plusStatusInfo.power = 1;
				plusStatusInfo.speed = 0.1f;
				
				/* �� */
				plusStatusInfo.boostPower = 2;
				plusStatusInfo.boostSpeed = 0.2f;
			}

			
		}

		//	�e������
		ShadowInitialize();

		if ( obj == nullptr )	return	false;
		return	true;
	}

	//	���
	void	BaseChara::Release( void )
	{
		SafeDelete( obj );
		SafeDelete( shadow.obj );
	}

	//	�p�����[�^��ԏ�����
	void	BaseChara::ParameterInfoInitialize( void )
	{
		ParameterInfoInitialize(unrivaled);
		ParameterInfoInitialize( slip );
		ParameterInfoInitialize( boost );
		ParameterInfoInitialize( outrage );
		ParameterInfoInitialize( attackUp );
		ParameterInfoInitialize( speedUp );
		ParameterInfoInitialize( bomb );
		ParameterInfoInitialize( jump );
		ParameterInfoInitialize( magnet );
		ParameterInfoInitialize( confusion );
		ParameterInfoInitialize( respawn );
		ParameterInfoInitialize(unrivaledItem);
	}

	//	�p�����[�^��ԏ�����
	void	BaseChara::ParameterInfoInitialize( PARAMETER_INFO& ps )
	{
		ps.state = false;
		ps.timer = 0;
	}

	//	�e��񏉊���
	void	BaseChara::ShadowInitialize( void )
	{
		//	�e�\���̏�����
		{
			shadow.obj = new iex2DObj( "DATA/Effect/shadow.png" );
			shadow.pos = pos;
			shadow.scale = 1.0f;
			SetVertex( shadow.v[0], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 0.0f, 0.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[1], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 1.0f, 0.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[2], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 0.0f, 1.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[3], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 1.0f, 1.0f, 0xFFFFFFFF );
		}
	}

//----------------------------------------------------------------------------
//	�X�V�E�`��
//----------------------------------------------------------------------------

	//	�X�V
	void	BaseChara::Update( void )
	{
		if ( mode != MODE_STATE::GUARD && !GetParameterState( PARAMETER_STATE::UNRIVALEDITEM ) ) 	m_Effect->SetShield( GetPlayerNum(), false );

		// �U����Ďg�p�̏���
		if(attackInfo.Interval > 0 ) attackInfo.Interval--;

		//	���[�h�Ǘ�
		(isPlayer) ? ModeManagement() : CPU_ModeManagement();

		//	���S����
		if ( GetLife() <= 0 )
		{
			life = 0;
			if ( mode != MODE_STATE::DEATH )
			{
				SetMode( MODE_STATE::DEATH );
				sound->PlaySE( SE::DEATH_SE );
				particle->BlueFlame( pos, 1.5f );
			}
		}

		//	�p�����[�^���X�V
		ParameterInfoUpdate();
		ParameterAdjust();

		//	�d�͉��Z
		move.y += GRAVITY;

		//	�e���W�ݒ�
		shadow.pos = pos;

		//	�X�e�[�W�����蔻��
		StageCollisionCheck();
		
		//	�ړ��l���Z
		AddMove();

		//	�R�͌v�Z
		CalcDrag();

		//	�����`�F�b�N
		FallCheck();

		//	�e�X�V
		ShadowUpdate();

		//	���X�V
		obj->Animation();
		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Update();

		static DWORD last = timeGetTime();
		DWORD elapse = timeGetTime() - last;
		dt = elapse / 1000.0f;
		last += elapse;
	}

	//	�`��
	void	BaseChara::Render( iexShader* shader, LPSTR technique )
	{
		if ( shader == nullptr || technique == nullptr )
		{
			obj->Render();
		}
		else
		{
			//	�_���[�W�F����
			CalcColorParameter();
			
			//	�e�`��
			iexPolygon::Render3D( shadow.v, 2, shadow.obj, shader3D, "alpha" );
			
			if ( renderflag )
			obj->Render( shader, technique );
		}
	}

//----------------------------------------------------------------------------
//	����֐�
//----------------------------------------------------------------------------

	//	���[�h�Ǘ�
	void	BaseChara::ModeManagement( void )
	{
		switch ( mode )
		{
		case MODE_STATE::WAIT:
			Wait();
			break;

		case MODE_STATE::MOVE:
			Move();
			break;

		case MODE_STATE::POWERARTS:
		case MODE_STATE::HYPERARTS:
		case MODE_STATE::QUICKARTS:
			SetParameterState(PARAMETER_STATE::UNRIVALED);
			Attack( mode );
			break;

		case MODE_STATE::JUMP:
			Jump();
			break;

		case MODE_STATE::GUARD:
			Guard();
			break;

		case MODE_STATE::DAMAGE:
			Damage();
			break;

		case MODE_STATE::KNOCKBACK:
			KnockBack();
			break;

		case MODE_STATE::DAMAGE_LEANBACKWARD:
			KnockBackLeanBackWard();
			break;

		case MODE_STATE::DEATH:
			Death();
			break;
		}
	}

	void	BaseChara::CPU_ModeManagement()
	{
		switch (aiInfo.mode)
		{
		case AI_MODE_STATE::MOVE:		//�@�R�C�������ɍs��
			AutoMove();
			break;

		case AI_MODE_STATE::WAIT:
			AutoWait();
			break;

		case AI_MODE_STATE::POWERARTS:
		case AI_MODE_STATE::HYPERARTS:
		case AI_MODE_STATE::QUICKARTS:
			SetParameterState(PARAMETER_STATE::UNRIVALED);
			AutoAttack(aiInfo.mode);
			break;

		case AI_MODE_STATE::JUMP:
			Jump();
			break;

		case AI_MODE_STATE::GUARD:
			Guard();
			break;

		case AI_MODE_STATE::DAMAGE:
			Damage();
			break;

		case AI_MODE_STATE::KNOCKBACK:
			KnockBack();
			break;

		case AI_MODE_STATE::DAMAGE_LEANBACKWARD:
			KnockBackLeanBackWard();
			break;

		case AI_MODE_STATE::DEATH:
			Death();
			break;

		case AI_MODE_STATE::RUNAWAY:
			RunAway();
			break;
		}
	}

	//	�R�͉��Z
	void	BaseChara::CalcDrag( void )
	{
		if ( !slip.state )
		{
			move.x *= drag;
			move.z *= drag;
		}
		else
		{
			move.x *= slipInfo.drag;
			move.z *= slipInfo.drag;
		}
	}

	//	�ړ��l���Z
	void	BaseChara::AddMove( void )
	{
		pos += move;
	}

	//	�_���[�W���J���[�v�Z
	void	BaseChara::CalcColorParameter( void )
	{
		damageColor.param -= Vector3( 0.035f, 0.035f, 0.035f );
		if ( damageColor.param.x <= 0.0f )	damageColor.param.x = 0.0f;
		if ( damageColor.param.y <= 0.0f )	damageColor.param.y = 0.0f;
		if ( damageColor.param.z <= 0.0f )	damageColor.param.z = 0.0f;

		shader3D->SetValue( "colorParam", damageColor.param );
	}

	//	�X�e�[�W�����蔻��
	void	BaseChara::StageCollisionCheck( void )
	{
		int	outId;
		float		height = 0.0f;
		float		work = 0.0f;
		float		objectWork = 0.0f;
		objectMove = Vector3( 0.0f, 0.0f, 0.0f );
		Vector3	tempPos = Vector3( 0.0f, 0.0f, 0.0f );

		//	�ǔ���
		checkWall = stage->CheckWall( pos, move );

		Vector3 localPos = GetPos();
		Vector3 localAngle = Vector3(0.0f, GetAngle(), 0.0f);
		//��蕨
		bool ret = stage->GetLocal(localPos, localAngle );	//�{�b�N�X�̃��C���Ԃ��Ă��邩
		stage->ObjUpdate();
		//carrier->Update(mat);

		if (ret)
		{
			//carrier->GetWorld(localPos);
			stage->GetWorld(localPos, localAngle);
			/*player->SetMode(Player::MODE_MOVE);*/

			/*Vector3 m = GetMove();
			SetMove(Vector3(m.x, 0, m.z));*/

			SetPos(localPos);
			obj->SetPos(localPos);
			SetAngle(localAngle);
			obj->SetAngle(localAngle);
			obj->Update();
		}


		//	�������C����
		work = stage->GetHeight( pos );
		objectWork = stage->GetHeightToObject( pos, objectMove, outId );
		
		//	�e�ݒ�
		if ( work < objectWork )	height = objectWork;
		else									height = work;

		//	�ڒn����
		if ( pos.y < work || pos.y < objectWork )
		{
			if ( pos.y < objectWork )
			{
				pos.y = height = objectWork;
				pos += objectMove;
			}
			if ( pos.y < work )
			{
				pos.y = height = work;
			}
			move.y = 0.0f;
			isGround = true;
			if ( jumpPower <= 0.0f )
			{
				jumpState = true;
			}
		}
		//	�O�����C����
		objectWork = stage->GetFrontToObject( pos, tempPos, outId );
		if ( pos.z >= objectWork - WALL_DIST )
		{
			if ( pos.z >= objectWork - WALL_DIST )	pos.z = objectWork - WALL_DIST;
			move.z = 0.0f;
			checkWall = true;
		} 

		//	������C����
		objectWork = stage->GetBackToObject( pos, tempPos, outId );
		if ( pos.z <= objectWork + WALL_DIST )
		{
			if ( pos.z <= objectWork + WALL_DIST )	pos.z = objectWork + WALL_DIST;
			move.z = 0.0f;
			checkWall = true;
		}

		//	�E���C����
		objectWork = stage->GetRightToObject( pos, tempPos, outId );
		if ( pos.x >= objectWork - WALL_DIST )
		{
			if ( pos.x >= objectWork - WALL_DIST )	pos.x = objectWork - WALL_DIST;
			move.x = 0.0f;
			checkWall = true;
		}

		//	�����C����
		objectWork = stage->GetLeftToObject( pos, tempPos, outId );
		if ( pos.x <= objectWork + WALL_DIST )
		{
			if ( pos.x <= objectWork + WALL_DIST )	pos.x = objectWork + WALL_DIST;
			move.x = 0.0f;
			checkWall = true;
		}

		//	�e�����ݒ�
		shadow.pos.y = height + 0.1f;
	}

	//	�p�x����
	void	BaseChara::AngleAdjust( float speed )
	{
		if ( !( input->Get( KEY_AXISX ) || input->Get( KEY_AXISY ) ) )	return;

		//	���X�e�B�b�N�̓���
		float	axisX = input->Get( KEY_AXISX ) * 0.001f;
		float	axisY = -input->Get( KEY_AXISY ) * 0.001f;

		//	�J�����̑O�����������߂�
		Vector3	vEye( mainView->GetTarget() - mainView->GetPos() );
		float	cameraAngle = atan2f( vEye.x, vEye.z );

		//	���͕��������߂�
		float inputAngle = atan2f( axisX, axisY );

		//	�ڕW�̊p�x�����߂�
		float	targetAngle = cameraAngle + inputAngle;

		//	�e�ɓ�����
		AngleAdjust( Vector3( sinf( targetAngle ), 0.0f, cosf( targetAngle ) ), speed );
	}

	//	�p�x����
	void	BaseChara::AngleAdjust( const Vector3& direction, float speed )
	{
		//	���݂̌����ƖڕW�̌����̍������߂�
		float	targetAngle( atan2f( direction.x, direction.z ) );
		
		//	������Ԃ̎�������t�ɂ��Ă݂�
		if ( confusion.state ) 	targetAngle = -targetAngle;

		//	�ړ�����������Z�b�g
		moveVec = targetAngle;

		//	�p�x�������߂�
		float	dAngle( targetAngle - GetAngle() );

		//	���̐��K��
		if ( dAngle > 1.0f * D3DX_PI )	dAngle -= 2.0f * D3DX_PI;
		if ( dAngle < -1.0f * D3DX_PI ) 	dAngle += 2.0f * D3DX_PI;

		//	����speed���k�߂�
		if ( dAngle > 0.0f ){
			dAngle -= speed;
			if ( dAngle < 0.0f )  dAngle = 0.0f;
		}
		else{
			dAngle += speed;
			if ( dAngle > 0.0f )	dAngle = 0.0f;
		}

		//	�����ɔ��f
		SetAngle( targetAngle - dAngle );

		//	�v���C���[�̌������Έȏ�ɂȂ�Ȃ��悤�ɒ�������
		if ( GetAngle() >= 1.0f * D3DX_PI )		angle.y -= 2.0f * D3DX_PI;
		if ( GetAngle() <= -1.0f * D3DX_PI )		angle.y += 2.0f * D3DX_PI;
	}

	//	�m�b�N�o�b�N
	void	BaseChara::KnockBack( void )
	{
		//���[�V�����A�g�f�i�I�X(�]�͂�����Ί֐���)
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::SCAVENGER)
		{
			if (obj->GetFrame() >= 278) obj->SetFrame(278);
		}
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::PIRATE)
		{
			if (obj->GetFrame() >= 286) obj->SetFrame(286);
		}
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::THIEF)
		{
			if (obj->GetFrame() >= 408) obj->SetFrame(408);
		}
//		SetMotion(MOTION_NUM::POSTURE);

		//SetParameterState(PARAMETER_STATE::UNRIVALED);
		SetDrag(1.0f);	//�ꎞ�I�ɍR�͂Ȃ���
		if (move.y < 0 && isGround)
		{
			move = Vector3(0.0f, 0.0f, 0.0f);
			damageStep = 0;
			SetMode(MODE_STATE::MOVE);

			//���[�V�����A�g�f�i�I�X
			SetMotion(0);

			//SetUnrivaled(false);
		}
	}

	//	�m�b�N�o�b�N�^��
	void	BaseChara::AddKnockBackForce( float force )
	{
		//switch (damageStep)
		//{
		//case 0:
			isGround = false;
			SetDamageColor(damageColor.catchColor);
			move = knockBackInfo.vec * (force / 4);
			move.y = force / 4;

			SetMode( MODE_STATE::KNOCKBACK );
			jumpState = true;
			//damageStep++;
			//break;

		//case 1:
			//Damage();
			//break;
		//}
	}

	//	�m�b�N�o�b�N	������̂�
	void	BaseChara::KnockBackLeanBackWard( void )
	{
		static int branktime = 0;	//���̋����莞�ԁ@��Ń��[�V�����t���[������Ƃ�\����

		//SetParameterState(PARAMETER_STATE::UNRIVALED);
		if ( branktime == 0 )  SetDamageColor( damageColor.catchColor );
		branktime++;
		SetMove( Vector3( 0.0f, move.y, 0.0f ) );
//		SetMotion( MOTION_NUM::POSTURE );
		if ( branktime >= leanFrame )
		{
			branktime = 0;
			SetMode( MODE_STATE::MOVE );
			//SetUnrivaled(false);
		}
	}

	//	�ҋ@
	void	BaseChara::Wait( void )
	{
		SetCoinUnrivaled( true );
		SetDrag( 0.8f );
	}

	//	����
	void	BaseChara::Move( void )
	{
		if ( GetCoinUnrivaled() ) 	SetCoinUnrivaled( false );

		Control();
	}

	//	�U��
	void	BaseChara::Attack( int attackKind )
	{
//		SetMotion( MOTION_NUM::ATTACK1 );

		bool	isEnd = false;

		if (attackInfo.Interval > 0)
		{
			isEnd = true;
		}
		else
		{
			switch (attackKind)
			{
			case	MODE_STATE::QUICKARTS:
				isEnd = QuickArts();
				if (!isEnd)	SetAttackParam(attackKind);
				break;

			case MODE_STATE::POWERARTS:
				isEnd = PowerArts();
				if (!isEnd)	SetAttackParam(attackKind);
				break;

			case MODE_STATE::HYPERARTS:
				isEnd = HyperArts();
				canHyper = isEnd;
				if (!isEnd)	SetAttackParam(attackKind);
				break;
			}
		}

		//	���[�V�����I������
		if ( isEnd )
		{
			mode = MODE_STATE::MOVE;
			attackInfo.t = 0.0f;
			attackInfo.r = 0.0f;
			attackInfo.type = 0;
			attackInfo.pos = Vector3(0.0f, 0.0f, 0.0f);
			attackInfo.addParam = -1;
			attackInfo.top = Vector3(0.0f, 0.0f, 0.0f);
			attackInfo.bottom = Vector3(0.0f, 0.0f, 0.0f);
			knockBackInfo.type = 0;
			SetUnrivaled(false);
		}
	}

	//	�W�����v
	void	BaseChara::Jump( void )
	{
		if ( jumpState )
		{
			if ( isPlayer )
			{
				if ( input->Get( KEY_B ) == 3 )
				{
					jumpPower = JUMP_POWER;
					jumpState = false;
				}
			}
		}
		else
		{
			if ( jumpPower > 0.0f )		move.y += jumpPower;
			jumpPower -= JUMP_POWER * 0.1f;
		}
	}

	//	�K�[�h
	void	BaseChara::Guard( void )
	{
		move.x = move.z = 0.0f;
		//SetParameterState(PARAMETER_STATE::UNRIVALED);
//		SetMotion( MOTION_NUM::GUARD );

		//	�{�^�����͂Ȃ��Ɩ߂�
		if ( input->Get( KEY_B6 ) != 1 )
		{
			SetMode( MODE_STATE::MOVE );
			m_Effect->SetShield( GetPlayerNum(), false );
			SetUnrivaled(false);
		}
	}

	//	�_���[�W
	void	BaseChara::Damage( void )
	{

		//���[�V�����A�g�f�i�I�X(�]�͂�����Ί֐���)
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::SCAVENGER)
		{
			if (obj->GetFrame() <= 250) SetMotion(9);
		}
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::PIRATE)
		{
			SetMotion(7);
		}
		if (gameManager->GetCharacterType(playerNum) == CHARACTER_TYPE::PIRATE)
		{
			SetMotion(8);
		}
		
		AddKnockBackForce(force);
	}

	//	���S
	void	BaseChara::Death( void )
	{
		static	float	param = 0.0f;	//	��
		static	bool	initflag = false;		//	�����ʉ߃t���O
		static	float	moveAngle = 0.0f;
		
		//	���S�����G
		SetParameterState(PARAMETER_STATE::UNRIVALED);
		SetCoinUnrivaled( true );
//		SetMotion( MOTION_NUM::DEATH );

		//	�R�C�������΂�܂�
		if ( !initflag )
		{
			int	coinNum = gameManager->GetCoinNum( this->playerNum );
			FOR( 0, coinNum / 2 )
			{
				//	�R�C�������΂�܂�
				if (coinNum > 0)
				{
					coinManager->Append( GetPos(), Vector3( Random::GetFloat( -1.0f, 1.0f ), 1.0f, Random::GetFloat( 0.0f, 1.0f ) ), Random::GetFloat( -1.0f, 1.0f ), Coin::COIN );
					gameManager->SubCoin( playerNum );
				}
					
			}
			initflag = true;
		}

		//	�҂����ԉ��Z�i���j���[�V�����o������ύX
		param += 0.01f;
		if ( param >= 1.0f )	param = 1.0f;

		CubicFunctionInterpolation( moveAngle, 0.0f, D3DX_PI * 0.5f, param );
		SetAngle( Vector3( moveAngle, GetAngle(), 0.0f ) );

		//	���̓��[�V�����Ȃ��̂łQ�b�҂����Ԃ�ݒ�
		if ( param >= 1.0f )
		{
			//	���X�|�[��
			pos = gameManager->InitPos[this->playerNum];
			SetLife( gameManager->GetStartLife( this->playerNum ) );	//	���C�t�𖞃^���ɂ���
			SetMode( MODE_STATE::WAIT );											//	�ҋ@�ɂ��ē����Ȃ�����
			SetParameterState( PARAMETER_STATE::RESPAWN );			//	�_�Ł����G�����J�n
			SetAngle( Vector3( 0.0f, 0.0f, 0.0f ) );
			param = 0.0f;
			initflag = false;
		}
	}

	//	����
	void	BaseChara::Run( void )
	{
		//	���X�e�B�b�N�̓��̓`�F�b�N
		float	axisX = ( float )input->Get( KEY_AXISX );
		float	axisY = ( float )input->Get( KEY_AXISY );
		float	length = sqrtf( axisX * axisX + axisY * axisY );

		if ( length > MIN_INPUT_STATE )
		{
			//���[�V�����A�g�f�i�I�X
			SetMotion( 1 );
			//SetMotion( MOTION_NUM::RUN );
			static	float adjustSpeed = 0.3f;
			AngleAdjust( adjustSpeed );
			if ( !slip.state )
			{
				move.x = sinf( moveVec ) * totalSpeed;
				move.z = cosf( moveVec ) * totalSpeed;
			}
			else
			{
				if ( move.Length() < speed )
				{
					move.x += sinf( moveVec ) * slipInfo.speed;
					move.z += cosf( moveVec ) * slipInfo.speed;
				}				
			}
		}
		else
		{
			//���[�V�����A�g�f�i�I�X
			SetMotion(0);
//			SetMotion( MOTION_NUM::POSTURE );
			SetDrag( 0.8f );
		}
	}

	//	�����`�F�b�N
	void	BaseChara::FallCheck( void )
	{
		if ( pos.y < -3.0f )
		{
			//	�R�C���𔼕����炷
			int		coinNum = gameManager->GetCoinNum( playerNum ) / 2;
			FOR( 0, coinNum )
			{
				if ( coinNum > 0 )
				{
					gameManager->SubCoin( this->playerNum );
					coinManager->Append( GetPos(), Vector3( Random::GetFloat( -0.7f, 0.7f ), 1.0f, Random::GetFloat( 0.3f, 0.7f ) ), Random::GetFloat( -0.7f, 0.7f ), Coin::COIN );
				}
			}

			//	�����ʒu�ɔz�u���҂���Ԃɂ��čs���s�\�ɂ��遨���X�|�[����Ԃ�ݒ�
			SubLife();
			pos = gameManager->InitPos[this->playerNum];
			if ( life <= 0 ) 	SetMode( MODE_STATE::MOVE );
			else
			{
				SetMode( MODE_STATE::WAIT );
				SetParameterState( PARAMETER_STATE::RESPAWN );
			}
		}
	} 

	//	�p�����[�^����
	void	BaseChara::ParameterAdjust( void )
	{
		totalPower = power;
		totalSpeed = speed;

		if ( attackUp.state )	totalPower = power + plusStatusInfo.power;
		if ( speedUp.state )	totalSpeed = speed + plusStatusInfo.speed;
		
		//�@�u�[�X�g��
		if ( boost.state )
		{
			totalPower = power + plusStatusInfo.boostPower;
			totalSpeed = speed + plusStatusInfo.boostSpeed;
			if ( attackUp.state )	totalPower += plusStatusInfo.power;
			if ( speedUp.state )	totalSpeed += plusStatusInfo.speed;
		}
	}

	//	�e�X�V
	void	BaseChara::ShadowUpdate( void )
	{
		//	�X�P�[���v�Z( �����ɉ����ĉe�̃X�P�[���𒲐��A�e�̑傫���̍ő�l�̓��f���̑傫����2.5�{�ɐݒ� )
		float	t = pos.y / 50.0f;
		static	float	maxScale = 2.5f;
		shadow.scale = maxScale - ( maxScale * t );

		//	���_�Z�b�g
		static	DWORD	vertexColor = 0xFFFFFFFF;
		SetVertex( shadow.v[0], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 0.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[1], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 1.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[2], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 0.0f, 1.0f, vertexColor );
		SetVertex( shadow.v[3], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 1.0f, 1.0f, vertexColor );
	}

	//	���C�t���Z
	void	BaseChara::AddLife( void )
	{
		life++;
		int	maxLife = gameManager->GetStartLife( this->playerNum );
		if ( life >= maxLife )
		{
			life = maxLife;
		}
	}

	//	���C�t���Z
	void	BaseChara::SubLife( void )
	{
		life--;
		
		if ( life <= 0 )
		{
			life = 0;
			SetMode( AI_MODE_STATE::DEATH );
		}
	}

//-------------------------------------------------------------------------------------
//	�p�����[�^��񓮍�֐�
//-------------------------------------------------------------------------------------

	//	�X�e�[�g�Ǘ�
	void	BaseChara::ParameterInfoUpdate( void )
	{
		//--------�e�C�x���g�E�A�C�e�����ʏ���������--------//

		//��莞�Ԗ��G
		Unrivaled();

		//	�U���̓A�b�v�A�C�e�����ʓ���
		AttackUp();

		//	�X���b�v�C�x���g
		EventSlip();

		//	�A�C�e���E�}�O�l�b�g
		ItemMagnet();

		//	����
		Confusion();

		//	���X�|�[��
		Respawn();

		//	�X�s�[�h�A�b�v
		SpeedUp();

		//	��莞�Ԗ��G(�A�C�e��)
		ItemUnrivaled();
	}

	//	��莞�Ԗ��G
	void	BaseChara::Unrivaled(void)
	{
		if (!unrivaled.state)
		{
			unrivaled.timer = 0;
			return;
		}

		//	���G�ɂ���
		unrivaled.state = true;

		//	�^�C�}�[���Z
		unrivaled.timer--;

		//	���Ԃ���������ʎ�����
		if (unrivaled.timer <= 0)
		{
			unrivaled.timer = 0;
			unrivaled.state = false;
		}
	}

	//	�U����Up�A�C�e�����ʓ���
	void	BaseChara::AttackUp( void )
	{
		if ( !attackUp.state )	return;

		particle->PowerUp( pos );

		//	�^�C�}�[���Z
		attackUp.timer--;

		//	���Ԃ���������ʎ�����
		if ( attackUp.timer <= 0 )
		{
			attackUp.timer = 0;
			attackUp.state = false;
		}
	}

	//	�X���b�v
	void	BaseChara::EventSlip( void )
	{
		if ( !slip.state )	return;

		//	�^�C�}�[���Z
		slip.timer--;

		//	���Ԃ���������ʎ�����
		if ( slip.timer <= 0 )
		{
			slip.timer = 0;
			slip.state = false;
		}
	}

	//	�}�O�l�b�g
	void	BaseChara::ItemMagnet( void )
	{
		if ( !magnet.state )	return;

		particle->Magnet( pos, 6.0f, 0.25f, 5 );

		//	�^�C�}�[���Z
		magnet.timer--;

		//	���Ԃ���������ʎ�����
		if ( magnet.timer <= 0 )
		{
			magnet.timer = 0;
			magnet.state = false;
		}
	}

	//	�ǂ񂯂u�[�X�g
	void	BaseChara::BoostUp( void )
	{
		/*if (boost.state)
		{
			power = totalPower;
			speed = totalSpeed;
		}*/
	}

	//	�\�����

	//	�X�s�[�hUp�A�C�e�����ʓ���
	void	BaseChara::SpeedUp( void )
	{
		if ( !speedUp.state )	return;

		particle->SpeedUp( pos );

		//	�^�C�}�[���Z
		speedUp.timer--;

		//	���Ԃ���������ʎ�����
		if ( speedUp.timer <= 0 )
		{
			speedUp.timer = 0;
			speedUp.state = false;
		}
	}

	//	�W�����v�A�C�e�����ʓ���

	//	�����A�C�e�����ʓ���

	//	����
	void	BaseChara::Confusion( void )
	{
		if ( !confusion.state )	return;

		//	�^�C�}�[���Z
		confusion.timer--;

		//	���Ԃ���������ʎ�����
		if ( confusion.timer <= 0 )
		{
			confusion.timer = 0;
			confusion.state = false;
		}
	}

	//	���X�|�[��
	void	BaseChara::Respawn( void )
	{
		if ( !respawn.state )	return;

		//	�^�C�}�[���Z
		respawn.timer--;

		//	���G���
		unrivaled.state = true;

		//	�`��t���O�؂�ւ�
		if ( respawn.timer % 10 == 0 )	renderflag = !renderflag;

		if ( respawn.timer <= 3 * SECOND - 30 )	SetMode( MODE_STATE::MOVE );

		//	���Ԃ���������ʎ�����
		if ( respawn.timer <= 0 )
		{
			unrivaled.state = false;
			renderflag = true;
			respawn.timer = 0;
			respawn.state = false;
			//SetMode( MODE_STATE::MOVE );
		}
	}

	//	��莞�Ԗ��G(�A�C�e��)
	void	BaseChara::ItemUnrivaled(void)
	{
		if ( !unrivaledItem.state )	return;

		//	���G�ɂ���
		unrivaled.state = true;

		//	�^�C�}�[���Z
		unrivaledItem.timer--;

		//	���Ԃ���������ʎ�����
		if ( unrivaledItem.timer <= 0 )
		{
			m_Effect->SetShield( playerNum, false );
			unrivaledItem.timer = 0;
			unrivaledItem.state = false;
			unrivaled.state = false;
		}
	}

//----------------------------------------------------------------------------
//	������@�ʓ���
//----------------------------------------------------------------------------

	//	�R���g���[������
	void	BaseChara::Control( void )
	{
		//	�X�e�B�b�N�ňړ�
		Run();

		if ( input->Get( KEY_D ) == 3 )		mode = MODE_STATE::QUICKARTS;
		if ( input->Get( KEY_C ) == 3 )		mode = MODE_STATE::POWERARTS;
		if ( canHyper )
		{
			if (input->Get(KEY_A) == 3)
			{
				if (GetLife() > 1)
				{
					SubLife();
					mode = MODE_STATE::HYPERARTS;
				}
			}
		}
	
		//if ( input->Get( KEY_B ) == 3 )
		//{
		//	if ( jumpState )
		//	{
		//		mode = MODE_STATE::JUMP;
		//	}
		//}
		Jump();

		if ( input->Get( KEY_B6 ) == 3 )
		{
			m_Effect->SetShield( GetPlayerNum(), true );
			mode = MODE_STATE::GUARD;
		}


	}

	//	AI����
	void	BaseChara::ControlAI( void )
	{
		AutoPickCoin(CPU_SERCH_COIN_MIN);

		//--------------------------------------------
		//�@�����ł͊e���[�h�ɂȂ邽�߂̏���������
		//--------------------------------------------

		/*	
			�E�R�C�������鎞�̓R�C�������ɍs���B�i1,2��������������Ǝ~�܂�j�A�i�R�C����遨����T���j
			�@�� �m���œK���ɍU���o���i�L�����ɂ���ċ�����ς���j
			�E�i�����������ăW�����v���o����悤�ɂ������B
			�E�R�C�����Ȃ����͂P�ʂ������͋������߂�������U���B
			�E�N�����߂��ōU���s�ׂ����Ă�����m���ŃK�[�h�B
			�E�����ǂ񂯂ɂȂ�����W�����炢�̊m���Ńn�C�p�[�A�[�c���g���B
		*/

		//�@�t�B�[���h�ɃR�C�����������ȏ�@���@�R�C���D��
		//if (coinManager->GetFreeCoinNum() > CPU_SERCH_COIN_MIN)
		//{
		//	aiInfo.mode = AI_MODE_STATE::MOVE;
		//}
		
		//�@�R�C�������ȉ�
		/*
			�P��  �F������(80%)	��	�K�[�h(20%)
			�Q�ʁ@�F�U��(50%)	��	����(30%)	���@�R�C��(20%)
			�R�ʁ@�F�U��(60%)	��	�R�C��(40%)
			�S�ʁ@�F�U��(80%)	��  �R�C��(20%)
		*/
		
		//else
		//{
			//�@���ʕʂɂ��ꂼ��m���ōs������
			//static int randi;
			//const int randi_MAX = 11;
			//if (!aiInfo.act_flag) randi = Random::GetInt(0, randi_MAX);
			//switch (rank)
			//{
			//case 1:
			//	// ������F�K�[�h�i�W�F�Q�j
			//	if		(randi < 8)				aiInfo.mode = AI_MODE_STATE::RUNAWAY;
			//	else if (randi > randi_MAX - 2)	aiInfo.mode = AI_MODE_STATE::GUARD;
			//	else							aiInfo.mode = AI_MODE_STATE::WAIT;
			//	break;
		
			//case 2:
			//	//�@�U���F������F�R�C���i�T�F�R�F�Q�j
			//	if		(randi < 4)					aiInfo.mode = AI_MODE_STATE::ATTACK;
			//	else if (randi > randi_MAX - 3)		aiInfo.mode = AI_MODE_STATE::RUNAWAY;
			//	else if (randi == 4 || randi == 5)	aiInfo.mode = AI_MODE_STATE::MOVE;
			//	else								aiInfo.mode = AI_MODE_STATE::WAIT;
			//	break;
		
			//case 3:
			//	//�@�U���F�R�C���i�U�F�S�j
			//	if		(randi < 6)				aiInfo.mode = AI_MODE_STATE::ATTACK;
			//	else if (randi > randi_MAX - 4)	aiInfo.mode = AI_MODE_STATE::MOVE;
			//	else							aiInfo.mode = AI_MODE_STATE::WAIT;
			//	break;
		
			//case 4:
			//	//�@�U���F�R�C���i�W�F�Q�j
			//	if		(randi < 8)				aiInfo.mode = AI_MODE_STATE::ATTACK;
			//	else if (randi > randi_MAX - 2)	aiInfo.mode = AI_MODE_STATE::MOVE;
			//	else							aiInfo.mode = AI_MODE_STATE::WAIT;
			//	break;
			//}
		//}

		//�@�f�o�b�O�i���邾���j
		//aiInfo.mode = AI_MODE_STATE::MOVE;

		//	�ǂ����m������W�����v
		if ( checkWall )
		{
			if ( jumpState )
			{
				//mode = MODE_STATE::JUMP;
				jumpPower = JUMP_POWER;
				jumpState = false;
			}
		}

		Jump();

	}


	


//----------------------------------------------------------------------------
//	AI����֐�
//----------------------------------------------------------------------------
	
	void	BaseChara::AutoMove( void )
	{
		//if (GetCoinUnrivaled()) 	SetCoinUnrivaled(false);

		//�@�e���[�h�ɑJ��
		ControlAI();
	}

	//�@�R�C�������ɍs��
	void	BaseChara::AutoPickCoin( int freeCoinMin )
	{	
		if (coinManager->GetFreeCoinNum() < freeCoinMin) return;

		//-----------------------------------------
		//�@target�Ɍ�����1�`3������
		//-----------------------------------------
		Vector3			target = Vector3( 0.0f, 0.0f, 0.0f );
		static	float	adjustSpeed = 0.2f;
		bool			existence = false;
		enum
		{
			AUTORUN_WALK = 0,
			AUTORUN_STAND
		};
		existence = coinManager->GetMinPos(target, pos);

		//	�Ώۂ����݂��Ă�����ΏۂɌ������đ���
		if (existence)
		{
			//particle->BlueFlame(target, 1.0f);
//			SetMotion(MOTION_NUM::RUN);
			AutoAngleAdjust(adjustSpeed, target);
			if (!slip.state)
			{
				move.x = sinf(angle.y) * speed;
				move.z = cosf(angle.y) * speed;
			}
			else
			{
				if (move.Length() < speed)
				{
					move.x += sinf(angle.y) * slipInfo.speed;
					move.z += cosf(angle.y) * slipInfo.speed;
				}
			}
		}
	}
	
	//	��������
	void	BaseChara::AutoAngleAdjust( float speed, Vector3 target )
	{
		//	�J�����̑O�����������߂�
		Vector3	vEye( mainView->GetTarget() - mainView->GetPos() );
		float	cameraAngle = atan2f(vEye.x, vEye.z);

		Vector3	vec = target - pos;
		vec.Normalize();

		//	���͕��������߂�
		float inputAngle = atan2f( vec.x, vec.z );
		moveVec = inputAngle;

		//	�ڕW�̊p�x�����߂�
		float	targetAngle = cameraAngle + inputAngle;

		//	�e�ɓ�����
		AngleAdjust(Vector3(sinf(targetAngle), 0.0f, cosf(targetAngle)), speed);
	}

	void	BaseChara::AutoAttack( int attackKind )
	{
		/*
			�L�����ɂ���� �u�U�������v�u�U�����ԁv��ς���@���I�[�o�[���C�h
			�i��ɂP�ʂɋ߂Â��ăp���[�A�[�c���N�C�b�N�A�[�c�H�j
		*/

		bool	isEnd = false;

		switch (attackKind)
		{
		case AI_MODE_STATE::QUICKARTS:
			isEnd = QuickArts();
			if (!isEnd)	SetAttackParam(attackKind);
			break;

		case AI_MODE_STATE::POWERARTS:
			isEnd = PowerArts();
			if (!isEnd)	SetAttackParam(attackKind);
			break;

		case AI_MODE_STATE::HYPERARTS:
			isEnd = HyperArts();
			canHyper = isEnd;
			if (!isEnd)	SetAttackParam(attackKind);
			break;
		}

		//	���[�V�����I������
		if (isEnd)
		{
			aiInfo.mode = AI_MODE_STATE::MOVE;
			attackInfo.t = 0.0f;
			attackInfo.r = 0.0f;
			attackInfo.type = 0;
			attackInfo.pos = Vector3(0.0f, 0.0f, 0.0f);
			attackInfo.addParam = -1;
			attackInfo.top = Vector3(0.0f, 0.0f, 0.0f);
			attackInfo.bottom = Vector3(0.0f, 0.0f, 0.0f);
			knockBackInfo.type = 0;
			SetUnrivaled(false);
		}

		aiInfo.act_flag = true;

		if (aiInfo.count_attack <= 0)
		{
			aiInfo.count_attack = 1 * SECOND;
			aiInfo.act_flag = false;
			aiInfo.mode = AI_MODE_STATE::MOVE;
		}
		else aiInfo.count_attack--;
	}

	//�@������
	void	BaseChara::RunAway( void )
	{
//		SetMotion( MOTION_NUM::RUN );

		aiInfo.act_flag = true;

		Vector3 vec_add( ZERO_VECTOR3 );
		Vector3 target( ZERO_VECTOR3 );
		for (int i = 0; i < PLAYER_MAX; i++)
		{
			Vector3 vec[4];

			//	�v���C���[�̔ԍ����擾
			int	p_num = characterManager->GetPlayerNum( i );

			//	�����Ɠ����ԍ���������X�L�b�v
			if ( GetPlayerNum() == p_num )
			{
				vec[p_num] = Vector3( ZERO_VECTOR3 );
				continue;
			}
			
			//	�ȉ��͎���VS����̏���
			vec[i] = characterManager->GetPos( characterManager->GetPlayerNum( i ) ) - pos;

			//�@����R�l�ւ̃x�N�g�������Z
			vec_add += vec[i];
		}

		//�@����������͑���R�l�ɑ΂��Ĕ��Ε���
		vec_add.Normalize();
		target = pos - vec_add;

		//�@�p�x����
		static	float	adjustSpeed = 0.2f;
		AutoAngleAdjust( adjustSpeed, target );

		//�@�ړ�
		if ( !slip.state )
		{
			move.x = sinf( moveVec ) * totalSpeed;
			move.z = cosf( moveVec ) * totalSpeed;
		}
		else
		{
			if ( move.Length() < totalSpeed )
			{
				move.x += sinf( moveVec ) * slipInfo.speed;
				move.z += cosf( moveVec ) * slipInfo.speed;
			}
		}

		//�@�s�����s����
		if (aiInfo.count_runaway <= 0)
		{
			aiInfo.count_runaway = 3 * SECOND;
			aiInfo.act_flag = false;
			SetMode(MODE_STATE::MOVE);
		}
		else aiInfo.count_runaway--;
	}

	//�@�I�[�g�K�[�h(�����F�t���[����)
	void	BaseChara::AutoGuard()
	{
//		SetMotion(MOTION_NUM::GUARD);
		move.x = move.z = 0.0f;
		SetParameterState(PARAMETER_STATE::UNRIVALED);
		aiInfo.act_flag = true;
		
		if (aiInfo.count_guard <= 0)
		{
			SetUnrivaled(false);
			aiInfo.count_guard = 1 * SECOND;
			aiInfo.act_flag = false;
			SetMode(MODE_STATE::MOVE);
		}
		else aiInfo.count_guard--;
	}

	//�@�����~�܂�
	void	BaseChara::AutoWait()
	{
//		SetMotion(MOTION_NUM::STAND);
		SetDrag(0.8f);
		move.x = move.z = 0.0f;
		aiInfo.act_flag = true;

		if (aiInfo.count_wait <= 0)
		{
			aiInfo.count_wait = 45;
			aiInfo.act_flag = false;
			aiInfo.mode = AI_MODE_STATE::MOVE;
		}
		else aiInfo.count_wait--;
	}

//----------------------------------------------------------------------------
//	���擾
//----------------------------------------------------------------------------

	//	�s��擾
	Matrix	BaseChara::GetMatrix( void )const
	{
		return	obj->TransMatrix;
	}

	//	�{�[���s��擾
	Matrix	BaseChara::GetBoneMatrix( int num )const
	{
		//	�{�[���s��擾
		Matrix	mat = *obj->GetBone( num );
		mat *= obj->TransMatrix;

		return	mat;
	}

	//	���W�擾
	Vector3	BaseChara::GetPos( void )const
	{
		return	pos;
	}

	//	�ړ��l�擾
	Vector3	BaseChara::GetMove( void )const
	{
		return	move;
	}

	//	�O���擾
	Vector3	BaseChara::GetFront( void )const
	{
		Matrix	mat = GetMatrix();
		Vector3	out = Vector3( mat._31, mat._32, mat._33 );
		out.Normalize();
		return	out;
	}

	//	�E���擾
	Vector3	BaseChara::GetRight( void )const
	{
		Matrix	mat = GetMatrix();
		Vector3	out = Vector3( mat._11, mat._12, mat._13 );
		out.Normalize();
		return	out;
	}

	//	����擾
	Vector3	BaseChara::GetUp( void )const
	{
		Matrix	mat = GetMatrix();
		Vector3	out = Vector3( mat._21, mat._22, mat._23 );
		out.Normalize();
		return	out;
	}

	//	�_���[�W���F�擾
	Vector3	BaseChara::GetDamageColor( void )const
	{
		return	damageColor.passColor;
	}

	//	�U������ʒu�擾
	Vector3	BaseChara::GetAttackPos( void )const
	{
		return	attackInfo.pos;
	}

	//	�U������ʒu���擾
	Vector3	BaseChara::GetAttackPos_Bottom( void )const
	{
		return	attackInfo.bottom;
	}

	//	�U������ʒu��擾
	Vector3	BaseChara::GetAttackPos_Top( void )const
	{
		return	attackInfo.top;
	}

	//	�{�[���ʒu�擾
	Vector3	BaseChara::GetBonePos( int num )const
	{
		Matrix	mat = GetBoneMatrix( num );
		
		//	�s�񂩂���W���擾
		Vector3	bonePos = Vector3( mat._41, mat._42, mat._43 );

		return	bonePos;		
	}

	//	�{�[���O���擾
	Vector3	BaseChara::GetBoneFront( int num )const
	{
		//	�s��擾
		Matrix	mat = GetBoneMatrix( num );

		//	�s�񂩂�O���擾
		Vector3	front = Vector3( mat._31, mat._32, mat._33 );
		front.Normalize();

		return	front;
	}

	//	�{�[������擾
	Vector3	BaseChara::GetBoneUp( int num )const
	{
		//	�s��擾
		Matrix	mat = GetBoneMatrix( num );

		//	�s�񂩂����擾
		Vector3	up = Vector3( mat._21, mat._22, mat._23 );
		up.Normalize();

		return	up;
	}

	//	�{�[���E���擾
	Vector3	BaseChara::GetBoneRight( int num )const
	{
		//	�s��擾
		Matrix	mat = GetBoneMatrix( num );

		//	�s�񂩂�E���擾
		Vector3	right = Vector3( mat._11, mat._12, mat._13 );
		right.Normalize();

		return	right;
	}


	//	�U���ǉ����ʎ擾(��Ԉُ�)
	int		BaseChara::GetAttack_addParam(void)const
	{
		return	attackInfo.addParam;
	}

	//	�U�����蔼�a�擾
	float		BaseChara::GetAttack_R( void )const
	{
		return	attackInfo.r;
	}

	//	�U������p�����[�^�擾
	float		BaseChara::GetAttack_T( void )const
	{
		return	attackInfo.t;
	}

	//	�����擾
	float		BaseChara::GetAngle( void )const
	{
		return	angle.y;
	}

	float		BaseChara::GetAngle(Vector3 vec1, Vector3 vec2)const
	{
		float out;
		out = Vector3Dot(vec1, vec2) / (vec1.Length() * vec2.Length());
		out = acos(out);
		return out;
	}

	//	�X�P�[���擾
	float		BaseChara::GetScale( void )const
	{
		return	scale;
	}

	//	�U���͎擾
	int			BaseChara::GetPower( void )const
	{
		return	power;
	}

	int			BaseChara::GetTotalPower( void )const
	{
		return	totalPower;
	}

	float		BaseChara::GetTotalSpeed( void )const
	{
		return	totalSpeed;
	}

	////	���G��Ԏ擾
	//bool		BaseChara::GetUnrivaled( void )const
	//{
	//	return	unrivaled;
	//}

	//	�n�C�p�[�g�p�ێ擾
	bool		BaseChara::GetCanHyper( void )const
	{
		return	canHyper;
	}
	
	//	�p�����[�^�[��Ԏ擾
	bool		BaseChara::GetParameterState( int type )const
	{
		bool	out = false;

		switch ( type )
		{
		case PARAMETER_STATE::UNRIVALED:
			out = unrivaled.state;
			break;

		case PARAMETER_STATE::SLIP:
			out = slip.state;
			break;

		case PARAMETER_STATE::BOOST:
			out = boost.state;
			break;

		case PARAMETER_STATE::OUTRAGE:
			out = outrage.state;
			break;

		case PARAMETER_STATE::ATTACKUP:
			out = attackUp.state;
			break;

		case PARAMETER_STATE::SPEEDUP:
			out = speedUp.state;
			break;

		case PARAMETER_STATE::BOMB:
			out = bomb.state;
			break;

		case PARAMETER_STATE::JUMP:
			out = jump.state;

		case	PARAMETER_STATE::MAGNET:
			out = magnet.state;
			break;

		case	PARAMETER_STATE::CONFUSION:
			out = confusion.state;
			break;

		case	PARAMETER_STATE::UNRIVALEDITEM:
			out = unrivaledItem.state;
			break;
		}

		return	out;
	}

	//	�W�����v�t���O�擾
	bool		BaseChara::GetJumpFlag( void )const
	{
		return	jumpState;
	}

	//	�R�C�������Ԏ擾
	bool		BaseChara::GetCoinUnrivaled( void )const
	{
		return	coinUnrivaled;
	}

	//	���[�h�擾
	int		BaseChara::GetMode( void )const
	{
		return	mode;
	}

	//	
	int		BaseChara::GetAIMode( void )const
	{
		return	aiInfo.mode;
	}

	//	�v���C���[�ԍ��擾
	int		BaseChara::GetPlayerNum( void )const
	{
		return	playerNum;
	}

	//	������t���[���擾
	int		BaseChara::GetLeanFrame( void )const
	{
		return	leanFrame;
	}

	//	�U���^�C�v�擾
	int		BaseChara::GetAttackParam( void )const
	{
		return	attackInfo.type;
	}

	//	�m�b�N�o�b�N�^�C�v�擾
	int		BaseChara::GetKnockBackType( void )const
	{
		return	knockBackInfo.type;
	}

	//�@���ʎ擾
	int		BaseChara::GetRank( void )const
	{
		return	rank;
	}

	//	���C�t�擾
	int		BaseChara::GetLife( void )const
	{
		return	life;
	}

//----------------------------------------------------------------------------
//	���ݒ�
//----------------------------------------------------------------------------

	//	���[�V�����ݒ�
	void	BaseChara::SetMotion( int motion )
	{
		if ( obj->GetMotion() != motion )
		{
			obj->SetMotion(motion);
		}
	}

	//	���W�ݒ�
	void	BaseChara::SetPos( Vector3 pos )
	{
		this->pos = pos;
	}

	//	�ړ��l�ݒ�
	void	BaseChara::SetMove( Vector3 move )
	{
		this->move = move;
	}
	
	//	�����ݒ�
	void	BaseChara::SetAngle( Vector3 angle )
	{
		this->angle = angle;
	}

	//	�����ݒ�
	void	BaseChara::SetAngle( float angle )
	{
		this->angle.y = angle;
	}

	//	�X�P�[���ݒ�
	void	BaseChara::SetScale( float scale )
	{
		this->scale = scale;
	}

	//	���[�h�ݒ�
	void	BaseChara::SetMode( int mode )
	{
		if ( GetMode() != mode )
		{
			this->mode = mode;
		}
	}

	//	AI���[�h�ݒ�
	void	BaseChara::SetAIMode(int mode)
	{
		if (GetAIMode() != mode)
		{
			this->aiInfo.mode = mode;
		}
	}

	//	�R�͐ݒ�
	void	BaseChara::SetDrag( float param )
	{
		drag = param;
	}

	//	�_���[�W���F�ݒ�
	void	BaseChara::SetDamageColor( Vector3 color )
	{
		damageColor.param = color;
	}

	//	�n���F�ݒ�
	void	BaseChara::SetPassColor( Vector3 color )
	{
		damageColor.catchColor = color;
	}

	//	�����莞�Ԑݒ�
	void	BaseChara::SetLeanFrame( int frame )
	{
		leanFrame = frame;
	}

	//	�u�[�X�g��Ԑݒ�
	/*void	BaseChara::SetBoosting( bool boosting )
	{
	this->boosting = boosting;
	}*/

	//	�m�b�N�o�b�N�����ݒ�
	void	BaseChara::SetKnockBackVec( Vector3 vec )
	{
		knockBackInfo.vec = vec;
	}

	//	���G��Ԑݒ�
	void	BaseChara::SetUnrivaled( bool state )
	{
		unrivaled.state = state;
	}

	//	�R�C������ݒ�
	void	BaseChara::SetCoinUnrivaled( bool state )
	{
		coinUnrivaled = state;
	}

	//	�p�����[�^��Ԑݒ�
	void	BaseChara::SetParameterState( int parameterState )
	{
		switch ( parameterState )
		{
		case	PARAMETER_STATE::UNRIVALED:
			SetParameterState(unrivaled, 2 * SECOND);
			break;

		case PARAMETER_STATE::SLIP:
			SetParameterState( slip, 1 * SECOND );
			break;

		case PARAMETER_STATE::BOOST:
			SetParameterState( boost, 30 * SECOND );
			break;

		case PARAMETER_STATE::OUTRAGE:
			SetParameterState( outrage, 11 * SECOND );
			break;

		case PARAMETER_STATE::ATTACKUP:
			SetParameterState( attackUp, 11 * SECOND );
			break;

		case PARAMETER_STATE::SPEEDUP:
			SetParameterState( speedUp, 11 * SECOND );
			break;

		case PARAMETER_STATE::BOMB:
			SetParameterState( speedUp, 5 * SECOND );
			break;

		case PARAMETER_STATE::JUMP:
			SetParameterState( jump, 11 * SECOND );
			break;

		case PARAMETER_STATE::CONFUSION:
			SetParameterState( confusion, 3 * SECOND );
			break;

		case PARAMETER_STATE::RESPAWN:
			SetParameterState( respawn, 3 * SECOND );
			break;

		case	PARAMETER_STATE::MAGNET:
			SetParameterState( magnet, 5 * SECOND );
			break;

		case	PARAMETER_STATE::UNRIVALEDITEM:
			m_Effect->SetShield( playerNum, true );
			SetParameterState( unrivaledItem, 5 * SECOND );
			break;
		}
	}

	//�@���ʐݒ�
	void	BaseChara::SetRank( int rank )
	{
		this->rank = rank;
	}

	//	�p�����[�^��Ԑݒ�
	void	BaseChara::SetParameterState( PARAMETER_INFO& parameterState, int time )
	{
		parameterState.state = true;
		parameterState.timer = time;
	}

	//	�^�͐ݒ�
	void BaseChara::SetForce( float force )
	{
		this->force = force;
	}

	//	���C�t�ݒ�
	void	BaseChara::SetLife( int life )
	{
		this->life = life;
	}
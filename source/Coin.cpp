
#include	"iextreme.h"
#include	"Random.h"
#include	"system/System.h"
#include	"Collision.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"Stage.h"
#include	"Sound.h"
#include	"Particle.h"
#include	"Event_Coin.h"

#include	"Coin.h"

//******************************************************************************
//
//	Coin�N���X
//
//******************************************************************************

//-------------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------------

#define	GETAWAY_LENGTH		5.0f	//	�����锻�苗��
#define	MAX_HEIGHT					50.0f	//	�z�肵�Ă��鍂���ő�l
#define	MASS							10.0f;	//	����
//#define	MAX_SHADOW_SIZE		1.5f	//	�e�ő�T�C�Y

//-------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Coin::Coin( void ) : obj( nullptr ), moveCheck( true )
	{
		shadow.obj = new iex2DObj( "DATA/Effect/shadow.png" );	
	}

	//	�f�X�g���N�^
	Coin::~Coin( void )
	{
		SafeDelete( obj );
		SafeDelete( shadow.obj );
	}

	//	������
	bool	Coin::Initialize( void )
	{
		obj = nullptr;
		angle = 0.0f;
		pos = Vector3( 0.0f, 0.0f, 0.0f );
		move = Vector3( 0.0f, 0.0f, 0.0f );
		scale = 3.0f;
		judgeTimer = 0;
		type = 0;
		activate = false;
		state = false;
		getAwayflag = false;
		absorbedflg = false;

		//	�����p�p�����[�^
		mass = MASS;
		position = Vector3( 0.0f, 0.0f, 0.0f );
		velocity = Vector3( 0.0f, 0.0f, 0.0f );
		acceleration = Vector3( 0.0f, 0.0f, 0.0f );
		resultant = Vector3( 0.0f, 0.0f, 0.0f );

		//	�e�\���̏�����
		{
			shadow.pos = pos;
			shadow.scale = 1.0f;
			SetVertex( shadow.v[0], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 0.0f, 0.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[1], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 1.0f, 0.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[2], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 0.0f, 1.0f, 0xFFFFFFFF );
			SetVertex( shadow.v[3], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 1.0f, 1.0f, 0xFFFFFFFF );
		}

		return	true;
	}

//-------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------

	//	�X�V
	void	Coin::Update( void )
	{
		//	���薳�����Ԍ��Z
		if ( judgeTimer > 0 )		judgeTimer--;
		else							activate = true;

		//	����
		Move();

		//	������
		if ( getAwayflag )	GetAway();

		//	�}�O�l�b�g
		Magnet();

		//	�ړ��l���Z
		pos += move;

		//	�e���W�ݒ�
		shadow.pos = pos;

		//	�[������
		if ( !absorbedflg )	//�|�����̋z�����݂��󂯂Ă���Ƃ��͖���
		{
			move.x *= 0.97f;
			move.z *= 0.97f;
		}

		//	�����蔻��
		StageCollisionCheck();
		PlayerCollisionCheck();
		
		//	�e���X�V
		ShadowUpdate();

		//	���X�V
		obj->SetAngle( angle );
		obj->SetPos( pos );
		obj->SetScale( scale );
		obj->Update();
	}
	
	//	�e���X�V
	void	Coin::ShadowUpdate( void )
	{
		//	�X�P�[���v�Z( �����ɉ����ĉe�̃X�P�[���𒲐��A�e�̑傫���̍ő�l�̓��f���̑傫����2.5�{�ɐݒ� )
		float	t = pos.y / MAX_HEIGHT;
		static	float	maxScale = scale * 1.2f;
		shadow.scale = maxScale - ( maxScale * t );

		//	���_�Z�b�g
		static	DWORD	vertexColor = 0xFFFFFFFF;
		SetVertex( shadow.v[0], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 0.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[1], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z + shadow.scale / 2, 1.0f, 0.0f, vertexColor );
		SetVertex( shadow.v[2], shadow.pos.x - shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 0.0f, 1.0f, vertexColor );
		SetVertex( shadow.v[3], shadow.pos.x + shadow.scale / 2, shadow.pos.y, shadow.pos.z - shadow.scale / 2, 1.0f, 1.0f, vertexColor );
	}

	//	�`��i �����̂ǂ��炩����ł��ݒ肵�Ȃ�������ʏ�`�� �j
	void	Coin::Render( iexShader* shader, LPSTR technique )
	{
		//	�e�`��
		iexPolygon::Render3D( shadow.v, 2, shadow.obj, shader3D, "alpha" );

		//	���f���`��
		if ( shader == nullptr || technique == nullptr )
			obj->Render();
		else
			obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------

	//	�X�e�[�W�����蔻��`�F�b�N
	void	Coin::StageCollisionCheck( void )
	{
		//	�z�肵�Ă�����������Ƃ�ł�����X�L�b�v�@
		int	outId;
		float	height = 0.0f;
		Vector3	tempPos;
		//	�������C����
		float work = stage->GetHeight( pos );
		float objectWork = stage->GetHeightToObject( pos, tempPos, outId );

		//	�e�ݒ�
		if ( work < objectWork )	height = objectWork;
		else									height = work;

		//	�ڒn����
		if ( pos.y < work || pos.y < objectWork )
		{
			if ( pos.y < objectWork )
			{
				pos.y = height = objectWork;
				pos += tempPos;
			}
			if ( pos.y < work )					pos.y = height = work;
			move.y = 0.0f;
		}

		//	�e�����ݒ�
		shadow.pos.y = height + 0.1f;
	}

	//	�v���C���[�Ƃ̂�����`�F�b�N
	void	Coin::PlayerCollisionCheck( void )
	{
		//	�R�C���̃R���W�����̗L���������擾
		if ( !gameManager->GetCoinCollision() )	return;

		Vector3	p_pos;
		float r = 0.0f;
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( !activate )	continue;
			if ( characterManager->GetCoinUnrivaled( i ) )	continue;
			//if ( characterManager->GetUnrivaled( i ) )	continue;
			p_pos = characterManager->GetPos( i );
			switch ( type )
			{
			case COIN:
				r = 1.5f;
				break;
			case COIN_BAG_5:
			case COIN_BAG_10:
				r = 5.0f;
				break;
			}
			bool isHit = Collision::CapsuleVSSphere( p_pos, Vector3( p_pos.x, p_pos.y + 3.0f, p_pos.z ), 2.0f, Vector3( pos.x, pos.y + ( scale * 0.5f ), pos.z ), r );

			if ( isHit )
			{
				Hitduringtheoperation(p_pos,i);
			}
		}
	}

	//	����
	void	Coin::Move( void )
	{
		//	�d�͉��Z
		if ( moveCheck )
		move.y += GRAVITY;
		
		//	��]
		angle += 0.05f;

		// ����( �X�e�[�W )	
		static float rate = 0.4f;
		stage->GetReflect( pos, move, rate );

		//	����������Ĕz�u
		if ( GetPos().y <= -3.0f )
		{
			SetPos( Vector3( Random::GetFloat( -10.0f, 10.0f ), Random::GetFloat( 0.0f, MAX_HEIGHT ), Random::GetFloat( -10.0f, 10.0f ) ) );
		}
	}

	//	�q�b�g������
	void	Coin::Hitduringtheoperation( const Vector3& pos, const int& Num )
	{
		state = false;
		float	effectScale = 0.4f;
		particle->Spark( pos, effectScale );
		switch ( type )
		{
		case COIN:
			gameManager->AddCoin( Num );
			if (event_coin->GetDubbleInst().eventflag) gameManager->AddCoin( Num );
			break;

		case COIN_BAG_5:
			FOR( 0, 5 )
			{
				gameManager->AddCoin(Num);
				if (event_coin->GetDubbleInst().eventflag) gameManager->AddCoin(Num);
			}
			break;

		case COIN_BAG_10:
			FOR( 0, 10 )
			{
				gameManager->AddCoin( Num );
				if ( event_coin->GetDubbleInst().eventflag )	gameManager->AddCoin( Num );
			}
			break;
		}
		sound->PlaySE( SE::COIN_SE );
	}

	//	����
	void	Coin::GetAway( void )
	{
		Vector3	p_pos[4];
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( !activate )	continue;
			p_pos[i] = characterManager->GetPos( i );
			//p_pos[i].y = pos.y;

			//	�v���C���[�ւ̃x�N�g���擾
			Vector3	vec = p_pos[i] - pos;
			float	length = vec.Length();

			//	�߂���Γ�����
			if ( length <= GETAWAY_LENGTH )
			{
				vec.Normalize();
				move = -vec * 0.1f;
			}
		}
	}

	//	�}�O�l�b�g
	void	Coin::Magnet( void )
	{
		Vector3	p_pos;
		for ( int i = 0; i < PLAYER_MAX; i++ )
		{
			if ( !activate )	continue;
			if ( !characterManager->GetParameterState( i, PARAMETER_STATE::MAGNET ) )	continue;
			p_pos = characterManager->GetPos( i );
			//p_pos.y = pos.y;

			//	�v���C���[�ւ̃x�N�g���擾
			Vector3	vec = p_pos - pos;
			float	length = vec.Length();

			//	�߂���΋z���񂹂���
			if ( length <= 6.0f )
			{
				vec.Normalize();
				move = vec * 0.1f;
			}
		}
	}

//-------------------------------------------------------------------------------
//	���ݒ�
//-------------------------------------------------------------------------------

	//	���W�ݒ�
	void	Coin::SetPos( const Vector3& pos )
	{
		this->pos = pos; 
	}

	//�ړ��ʐݒ�
	void	Coin::SetMove(const Vector3& move)
	{
		this->move = move;
	}
	
	//	�����ݒ�
	void	Coin::SetAngle( const float& angle )
	{
		this->angle = angle; 
	}

	//	�X�P�[���ݒ�
	void	Coin::SetScale( const float& scale )
	{
		this ->scale = scale; 
	}

	//	�����t���O�ݒ�
	void	Coin::SetGetAwayFlag( bool flag )
	{
		this->getAwayflag = flag;
	}

	//	�|�����ɋz������Ă����ԃt���O�ݒ�
	void	Coin::SetAbsorbedFlag(bool flag)
	{
		this->absorbedflg = flag;
	}
	
	//��Ԑݒ�
	void	Coin::SetState(bool state)
	{
		this->state = state;
	}

//-------------------------------------------------------------------------------
//	���擾
//-------------------------------------------------------------------------------

	//	���W�擾
	Vector3	Coin::GetPos( void )
	{
		Vector3	out = this->pos;
		return	out; 
	}

	//	�����擾
	float		Coin::GetAngle( void )
	{
		float	out = this->angle;
		return	out; 
	}
	
	//	��Ԏ擾
	bool		Coin::GetState( void )
	{
		return	state;
	}

//-------------------------------------------------------------------------------
//	�����p����֐�
//-------------------------------------------------------------------------------

	//	��񓝍�
	void	Coin::Integrate( float dt )
	{
		acceleration = ( resultant / mass );
		velocity += acceleration * dt;
		position = velocity * dt;

		resultant = Vector3( 0.0f, 0.0f ,0.0f );
	}

	//	�͂�������
	void	Coin::AddForce( const Vector3& force )
	{
		resultant += force;
	}

	//	�ʂ��w�肵�ă��C���Ƃ΂�
	bool	Coin::RayPick( LVERTEX v[3], Vector3& out_hitPos, Vector3& out_normal, Vector3 pos, Vector3 moveVec, float dist )
	{
		//	�p�����[�^����
		Vector3	p = pos;
		Vector3	vec = moveVec;
		Vector3	v1, v2, v3;
		Vector3	n;
		Vector3	l1, l2, l3;
		Vector3	temp;
		Vector3	cp;
		Vector3	p1, p2, p3;
		float neart = 0.0f;
		//float dist;

		//	���_�擾
		v1 = Vector3( v[0].x, v[0].y, v[0].z );
		v2 = Vector3( v[1].x, v[1].y, v[1].z );
		v3 = Vector3( v[2].x, v[2].y, v[2].z );

		//	��������
		l1 = v2 - v1;
		l2 = v3 - v2;

		//	�O�ςɂ��@���Z�o
		Vector3Cross( n, l1, l2 );
		out_normal = n;

		//	���ς̌��ʂ��v���X�Ȃ痠����
		float	dot = Vector3Dot( vec, n );
		if ( dot >= 0.0f )	return false;

		//	��_�Z�o
		p1 = v1 - p;
		float	t = Vector3Dot( n, p1 ) / dot;
		if ( t < 0.0f || t > neart )	return false;

		cp = vec * t + p;

		//	���_����
		p1 = v1 - cp;

		Vector3Cross( temp, p1, l1 );
		if ( Vector3Dot( temp, n ) < 0.0f )	return false;

		p2 = v2 - cp;
		Vector3Cross( temp, p2, l2 );
		if ( Vector3Dot( temp, n ) < 0.0f )	return false;

		l3 = v1 - v3;
		p3 = v3 - cp;
		Vector3Cross( temp, p3, l3 );
		if ( Vector3Dot( temp, n ) < 0.0f )	return false;

		out_hitPos = cp;
		return	true;
	}

	//	�|���S���Ƃ̓����蔻��
	void	Coin::Collide( void )
	{

	}
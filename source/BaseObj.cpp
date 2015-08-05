
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"

#include	"BaseObj.h"

//****************************************************************************************
//
//	BaseObj�N���X
//
//****************************************************************************************

//-------------------------------------------------------------------------------------
//	�������E���
//-------------------------------------------------------------------------------------
	
	//	�R���X�g���N�^
	BaseObj::BaseObj( void ) : obj( NULL ),
		pos( 0.0f, 0.0f, 0.0f ), move( 0.0f, 0.0f, 0.0f ),
		angle( 0.0f ), scale( 0.0f ), speed( 0.0f ), mode( 0 ),
		attackParam( 0 ), attackPos( 0.0f, 0.0f, 0.0f ), isGround( true), coinNum( 0 )
	{
		
	}

	//	�f�X�g���N�^
	BaseObj::~BaseObj( void )
	{
		SafeDelete( obj );
	}

	//	������
	bool	BaseObj::Initialize( int input, int type, Vector3 pos )
	{
		this->input = ::input[input];

		Load( type );

		this->pos = pos;

		if ( obj == NULL )	return	false;
		return	true;
	}

	//	���f���ǂݍ���
	bool	BaseObj::Load( int type )
	{
		switch ( type )
		{
		case 	PlayerData::PLAYER_TYPE::Y2009:
			obj = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
			break;

		case PlayerData::PLAYER_TYPE::ECCMAN:
			obj = new iex3DObj( "DATA/CHR/ECCMAN/ECCMAN.IEM" );
			break;

		default:
			obj = new iex3DObj( "DATA/CHR/Y2009/Y2009.IEM" );
			break;
		}

		if ( obj == NULL )	return	false;
		return	true;
	}

//-------------------------------------------------------------------------------------
//	�X�V�E�`��
//-------------------------------------------------------------------------------------

	//	�X�V
	void	BaseObj::Update( void )
	{
		//	�d�͉��Z
		move.y += GRAVITY;

		//	�X�e�[�W�����蔻��
		StageCollisionCheck();

		//	�ړ��l���Z
		pos += move;

		obj->SetPos( pos );
		obj->SetAngle( angle );
		obj->SetScale( scale );
		obj->Animation();
		obj->Update();
	}

	//	�`��
	void	BaseObj::Render( void )
	{
		obj->Render();
	}

	//	�V�F�[�_�[�t���`��
	void	BaseObj::Render( iexShader* shader, LPSTR technique )
	{
		obj->Render( shader, technique );
	}

//-------------------------------------------------------------------------------------
//	����֐�
//-------------------------------------------------------------------------------------

	//	�X�e�[�W�����蔻��`�F�b�N
	void	BaseObj::StageCollisionCheck( void )
	{
		//�@������
		float work = Collision::GetHeight( pos );
		if ( pos.y <= work )
		{
			pos.y = work;
			move.y = 0;
			isGround = true;
		}
		else
		{
			isGround = false;
		}

		//	�ǔ���
		Collision::CheckWall( pos, move );
	}
	
	//	�R�C���������Z
	void	BaseObj::AddCoin( void )
	{
		coinNum++;
	}

	//	�R�C���������Z
	void	BaseObj::SubCoin( void )
	{
		if ( coinNum > 0 )	coinNum--;
	}

//-------------------------------------------------------------------------------------
//	���ݒ�E�擾
//-------------------------------------------------------------------------------------

	//	�ݒ�
	void	BaseObj::SetMotion( int motion )
	{
		if ( obj->GetMotion() != motion )
		{
			obj->SetMotion( motion );
		}
	}
	void	BaseObj::SetMode( int mode )
	{
		if ( this->mode != mode )		this->mode = mode;
	}
	void	BaseObj::SetMotionData( MotionData& md, MotionType::Motion kind, int num )
	{
		switch ( kind )
		{
		case 	MotionType::STAND:					//	����
			md.STAND = num;
			break;
		case 	MotionType::POSTURE:				//	�\��
			md.POSTURE = num;
			break;
		case 	MotionType::RUN:						//	����
			md.RUN = num;
			break;
		case 	MotionType::JUMP:					//	�W�����v
			md.JUMP = num;
			break;
		case 	MotionType::LANDING:				//	���n
			md.LANDING = num;
			break;
		case 	MotionType::ATTACK1:				//	�U���P�i�K��
			md.ATTACK1 = num;
			break;
		case 	MotionType::ATTACK2:				//	�U���Q�i�K��
			md.ATTACK2 = num;
			break;
		case 	MotionType::ATTACK3:				//	�U���R�i�K��
			md.ATTACK3 = num;
			break;
		case 	MotionType::GUARD:					//	�K�[�h
			md.GUARD = num;
		}
	}

	void	BaseObj::SetPos( Vector3 pos ){ this->pos = pos; }
	void	BaseObj::SetPos( float x, float y, float z ){ this->pos = Vector3( x, y, z ); }
	void	BaseObj::SetAngle( float angle ){ this->angle = angle; }
	void	BaseObj::SetScale( float scale ){ this->scale = scale; }

	//	�擾
	Vector3		BaseObj::GetPos( void ){ return	pos; }
	Vector3		BaseObj::GetAttackPos( void ){ return attackPos; }
	Matrix		BaseObj::GetMatrix( void ){ return obj->TransMatrix; }
	float			BaseObj::GetAngle( void ){ return angle; }
	int				BaseObj::GetAttackParam( void ){ return attackParam; }
	int				BaseObj::GetCoinNum( void ){ return	coinNum; }

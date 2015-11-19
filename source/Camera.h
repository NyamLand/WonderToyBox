
#ifndef		__CAMERA_H__
#define		__CAMERA_H__

//****************************************************************************************
//
//	Camera�N���X
//
//****************************************************************************************

//	param
namespace
{
	namespace VIEW_MODE
	{
		enum
		{
			SETUP,
			FIX,
			SLERP,
			CHASE,		//	�����̃��[�h�̏ꍇ��SetPlayerInfo�Ő��Player�������Ȃ���΂����Ȃ�
			RESULT,
			TITLE,
			INDIVIDUAL,
		};
	}

	namespace TITLE_MOVE_INFO
	{
		float	height = 5.0f;
		Vector3	pos[] =
		{
			Vector3( 8.0f, height, 0.0f ),		//���
			Vector3( 45.0f, height, -14.0f ),	//	�I�v�V����
			Vector3( -41.0f, height, -20.0f ),	//	�N���W�b�g		
			Vector3( 46.0f, height, 200.0f ),	//	���C����
		};

		//	��Ɠ���
		Vector3	target[] =
		{
			Vector3( 8.0f, 3.0f, 13.0f ),
			Vector3( 53.0f, 3.0f, -5.0f ),
			Vector3( -50.0f, 3.0f, -14.0f ),
			Vector3( 46.0f, 4.0f, 200.0f ),
		};
	}
	
	namespace TITLE_TARGET
	{
		enum
		{
			PLAY,
			OPTION,
			CREDIT,
			MOVE_MAIN,
			END,
		};
	}
}

//	�N���X�O���錾
class Rubber;

//	class
class Camera : public iexView
{
private:
	//	�萔
	static	const		int		MAX = 10;		//	�J��������^�[�Q�b�g�̋����̍ő�
	static	const		int		MIN = 4;		//								�ŏ�
	static	const		int		PLAYER_NUM = 4;	//	�v���C���[�̐�

private:
	Vector3	playerPos[PLAYER_NUM];
	Vector3 target;
	Vector3	nextPoint;
	Vector3	nextTarget;
	Vector3	startPos;
	Vector3 pos;
	float		length;
	float		speed;
	float		t;
	float		playerAngle[PLAYER_NUM];
	bool		moveState;
	
	//	�S���p�����[�^
	Rubber*		q;
	
	//	�U���p�p�����[�^
	Vector3 adjust;
	bool		shakeflag;
	int			shakeTimer;
	float		wide;

	//	���ʐ��`��ԗp
	D3DXQUATERNION	orientation;

public:
	//	�������E���
	Camera( void );
	~Camera( void );

	//	�X�V�E�`��
	void	Update( int viewmode, Vector3 target = Vector3( 0.0f, 0.0f, 0.0f ) );
	void	Render( void );

	//	�e���[�h����
	void	ModeFix( Vector3 target );
	void	ModeSlerp( Vector3 target );
	void	ModeIndividualSurveillance( Vector3 target );
	void	ModeChase( void );
	void	ModeResult( void );
	void	ModeTitle();

	//	����֐�
	void	Slerp( Vector3 target, float speed ) ;
	void	Shake( void );
	void	ShakeSet( float wide, int timer );
	void	SetNextPoint( int num, float speed );
	void	SpringMove( Vector3 position );

	//	���擾
	Vector3	GetPos( void ){ return pos; }
	Vector3	GetTarget( void ){ return target; }
	Matrix	GetMatrix( void ){ return matView; }
	bool	GetMoveState( void )const{ return moveState; }
	float	GetPlayerAngle( int p_num ){ return playerAngle[p_num]; }
	
	//	���l�v�Z
	void	CalcCameraPos( void );
	Vector3	CalcCenterPos( void );
	float	CalcMaxDist( void );
	Vector3	CheckPosLength( Vector3 position );
	void	CheckViewAngle( void );

	
	//	���ݒ�
	void	SetPos( Vector3 pos ){ this->pos = pos; }
	void	SetPos( float posx, float posy, float posz ){ this->pos = Vector3( posx, posy, posz); }
	void	SetPlayerInfo( const Vector3& p_1, const Vector3& p_2, const Vector3& p_3, const Vector3& p_4 );
};

//	�o�l
class Rubber
{
public:
	//	���_�p�����[�^
	float mass;					//	����
	Vector3 position;			//	�ʒu
	Vector3 velocity;			//	���x
	bool	init_flag;

private:
	Vector3 acceleration;		//	�����x
	Vector3 resultant;			//	��

public:
	//	�R���X�g���N�^�E�f�X�g���N�^
	Rubber( void );
	~Rubber( void );

	//	�X�V
	void	Update( void );
	void	Integrate( float dt );

	//	���ݒ�E���擾
	void	AddForce( const Vector3 &force ){	resultant += force;	}
	const	Vector3 &GetAcceleration() const{	return acceleration; }
};

//	���C���J����
extern	Camera*		mainView;

//****************************************************************************************
#endif // !__CAMERA_H__


#ifndef		__OBJECT_H__
#define	__OBJECT_H__

//*******************************************************************************
//
//	Object�N���X
//
//*******************************************************************************

//	include

//	define
namespace
{
	namespace MOVE_TYPE
	{
		enum 
		{
			FIX_BOX,
			BREAK_OBJECT,
			MOVE_BOX_SIDE,
			MOVE_BOX_HIEGHT,
		};
	}

	namespace OBJECT_TYPE
	{
		enum 
		{
			BASE,
			BLUE_BLOCK,
			GREEN_BLOCK,
			RED_BLOCK,
			YELLOW_BLOCK,
			DESK_BASE,
			FOREST_BASE,
			END,
		};
	}

}

//	class
class Object
{
public:

private:
	int			id;
	int			moveType;
	int			objectType;
	int			durableValue;	//	�ϋv�l
	int			unrivaledTime;	//	���G����
	bool			state;
	bool			unrivaled;

	float			moveHeight;	//	�㉺�ړ��l
	float			originHeight;	//	���̍���
	float			flashParam;		//	�_��
	iexMesh*	obj;
	iexMesh*	collisionObj;
	Vector3	pos;
	Vector3	angle;
	Vector3	scale;
	Vector3	move;		//	�ړ��l
	Vector3	tempPos;	//	�ړ��ʕۑ��p

public:
	//	�������E���
	Object( void );
	~Object( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( iexShader*	shader = nullptr, LPSTR technique = nullptr );

	//	����֐�
	void	Move( void );
	void	MoveHeight( void );
	void	MoveSide( void );
	void	StageCollisionCheck( void );
	void	HitCheck( void );
	void	SubDurableValue( void );

	//	���ݒ�
	void	SetPos( Vector3 Pos );
	void	SetAngle( Vector3 Angle );
	void	SetAngle( float Angle );
	void	SetScale( Vector3 Scale );
	void	SetScale( float Scale );
	void	SetMesh( iexMesh* org );
	void	SetCollisionModel( iexMesh* org );
	void	SetMoveType( int moveType );
	void	SetObjectType( int objectType );
	void	SetOriginHeight( float height );
	void	SetId( int ID );
	void	SetUnrivaled( bool state );
	void	InitTempPos( void );

	//	���擾
	iexMesh*	GetMesh( void )const;
	Vector3	GetPos( void )const;
	Vector3	GetScale( void )const;
	Vector3	GetTempPos( void )const;
	Vector3	GetMove( void )const;
	float			GetAngle( void )const;
	void			GetAngle( Vector3& out )const;
	bool			GetState( void )const;
	bool			GetUnrivaled( void )const;
	int			GetDurable( void )const;
	int			GetObjectType( void )const;
	int			GetMoveType( void )const;
	int			GetID( void )const;
};

//*******************************************************************************
#endif // !__OBJECT_H__

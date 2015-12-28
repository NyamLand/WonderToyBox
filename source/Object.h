
#ifndef __OBJECT_H__
#define	__OBJECT_H__

//*******************************************************************************
//
//	Object�N���X
//
//*******************************************************************************

//	include

//	class
class Object
{
private:
	int			type;
	int			durableValue;	//	�ϋv�l
	bool			state;
	iexMesh*	obj;
	Vector3	pos;
	Vector3	angle;
	Vector3	scale;

public:
	enum OBJECT_TYPE
	{
		FIX_BOX,
		BREAK_OBJECT,
		MOVE_BOX_SIDE,
		MOVE_BOX_HIEGHT,
	};
	
public:
	//	�������E���
	Object( void );
	~Object( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( iexShader*	shader = nullptr, LPSTR technique = nullptr );

	//	����֐�
	void	Move( void );
	void	HitCheck( void );

	//	���ݒ�
	void	SetPos( Vector3 Pos );
	void	SetAngle( Vector3 Angle );
	void	SetAngle( float Angle );
	void	SetScale( Vector3 Scale );
	void	SetScale( float Scale );
	void	SetMesh( iexMesh* org );
	void	SetType( int objType );

	//	���擾
	Vector3	GetPos( void )const;
	Vector3	GetScale( void )const;
	float			GetAngle( void )const;
	void			GetAngle( Vector3& out )const;
	bool			GetState( void )const;
	int			GetType( void )const;
	int			GetDurable( void )const;
};

//*******************************************************************************
#endif // !__OBJECT_H__

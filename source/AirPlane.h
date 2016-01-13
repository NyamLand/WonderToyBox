
#pragma	once

//**********************************************************************
//
//	AirPlane�N���X
//
//**********************************************************************

//	include

//	class
class AirPlane
{
public:
	enum
	{
		FLYING_IN,
		FLYING_OUT,
		STAY,
	};

	enum 
	{
		TYPE1,
		TYPE2,
		TYPE3,
	};

private:
	ImageObj	obj;
	Vector3	pos;
	Vector3	scale;
	Vector3	startPos;
	Vector3	endPos;
	POINT		center;
	float			angle;
	float			heightParam;
	float			sideParam;
	float			param;
	bool			renderflag;
	int			mode;
	int			type;	//	�e�L�X�g�^�C�v

public:
	//	�������E���
	AirPlane( void );
	~AirPlane( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	bool	FlyingIn( void );
	bool	FlyingOut( void );
	void	FlyRoll( void );
	void	Stay( void );

	//	���ݒ�
	void	SetScale( float w, float h );
	void	SetType( int type );
	void	SetNext( Vector3	startPos, Vector3 nextPoint, int mode );
	void	SetRenderFlag( bool state );

	//	���擾
	Vector3	GetPos( void )const;
	Vector3	GetScale( void )const;
};

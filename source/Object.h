
#ifndef __OBJECT_H__
#define	__OBJECT_H__

//*******************************************************************************
//
//	Objectクラス
//
//*******************************************************************************

//	include

//	class
class Object
{
private:
	int			type;
	int			durableValue;	//	耐久値
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
	//	初期化・解放
	Object( void );
	~Object( void );

	//	更新・描画
	void	Update( void );
	void	Render( iexShader*	shader = nullptr, LPSTR technique = nullptr );

	//	動作関数
	void	Move( void );
	void	HitCheck( void );

	//	情報設定
	void	SetPos( Vector3 Pos );
	void	SetAngle( Vector3 Angle );
	void	SetAngle( float Angle );
	void	SetScale( Vector3 Scale );
	void	SetScale( float Scale );
	void	SetMesh( iexMesh* org );
	void	SetType( int objType );

	//	情報取得
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

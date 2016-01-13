
#pragma	once

//**********************************************************************
//
//	AirPlaneクラス
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
	int			type;	//	テキストタイプ

public:
	//	初期化・解放
	AirPlane( void );
	~AirPlane( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	bool	FlyingIn( void );
	bool	FlyingOut( void );
	void	FlyRoll( void );
	void	Stay( void );

	//	情報設定
	void	SetScale( float w, float h );
	void	SetType( int type );
	void	SetNext( Vector3	startPos, Vector3 nextPoint, int mode );
	void	SetRenderFlag( bool state );

	//	情報取得
	Vector3	GetPos( void )const;
	Vector3	GetScale( void )const;
};

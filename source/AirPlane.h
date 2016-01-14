
#pragma	once

//**********************************************************************
//
//	AirPlaneクラス
//
//**********************************************************************

//	include

namespace AIRPLANE_POS_INFO
{
	
}

//	class
class AirPlane
{

private:
	

public:
	const int IN_START_POS_X = static_cast<int>(iexSystem::ScreenWidth * 1.01f);
	const int IN_START_POS_Y = static_cast<int>(iexSystem::ScreenHeight * -0.13f);
	const int STAY_POS_X = static_cast<int>(iexSystem::ScreenWidth * 0.5f);
	const int STAY_POS_Y = static_cast<int>(iexSystem::ScreenHeight * 0.2f);
	const int OUT_END_POS_X = static_cast<int>(iexSystem::ScreenWidth * -0.55f);
	const int OUT_END_POS_Y = IN_START_POS_Y;
	const int ROLL_POINT_ADJUST_X = 200;

	const Vector3 IN_START_POS = Vector3(IN_START_POS_X, IN_START_POS_Y, 0.0f);
	const Vector3 STAY_POS = Vector3(STAY_POS_X, STAY_POS_Y, 0.0f);
	const Vector3 OUT_END_POS = Vector3(OUT_END_POS_X, OUT_END_POS_Y, 0.0f);

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

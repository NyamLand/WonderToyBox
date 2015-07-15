
#ifndef		__BASEOBJ_H__
#define		__BASEOBJ_H__

//****************************************************************************************
//
//	BaseObjクラス
//
//****************************************************************************************
class BaseObj
{
protected:
	//	定数
	const float GRAVITY = -0.01f;
	enum STATE
	{
		MOVE,
		ATTACK,
		DAMAGE,
	};

protected:
	//	モデル
	iex3DObj*	obj;

	//	コントローラ
	iexInput*		input;

	//	パラメータ
	Vector3		pos;
	Vector3		attackPos;
	Vector3		move;
	float			angle;
	float			scale;
	float			speed;
	int				mode;
	int				attackParam;
	bool			isGround;

protected:
	//	関数
	void	SetMotion( int motion );
	void	SetMode( int mode );

public:
	//	初期化・解放
	BaseObj( void );
	~BaseObj( void );
	bool	Initialize( int input, int type, Vector3 pos );

	//	更新・描画
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	動作関数
	void	StageCollisionCheck( void );

	//	情報設定
	void	SetPos( Vector3 pos );
	void	SetPos( float x, float y, float z );
	void	SetAngle( float angle );
	void	SetScale( float scale );

	//	情報取得
	Vector3	GetPos( void );
	Vector3	GetAttackPos( void );
	Matrix	GetMatrix( void );
	float		GetAngle( void );
	int			GetAttackParam( void );
	
};

//****************************************************************************************
#endif // !__BASEOBJ_H__


#ifndef __OBJECT_H__
#define	__OBJECT_H__

//*******************************************************************************
//
//	Objectクラス
//
//*******************************************************************************

class Object
{
protected:
	iexMesh*	obj;
	Vector3		pos;
	Vector3		move;
	float			scale;
	float			angle;
	bool			state;
	bool			activate;
	int				judgeTimer;	//	判定有効時間

public:
	//	初期化・解放
	Object( void );
	~Object( void );
	virtual	bool	Initialize( void );
	virtual	void	Release( void );

	//	更新・描画
	virtual	void	Update( void );
	virtual	void	Render( iexShader* shader = nullptr, LPSTR technique = nullptr );

	//	動作関数
	virtual	void	StageCollisionCheck( void );
	virtual	void	PlayerCollisionCheck( void );
	virtual	void	Move( void );
	virtual	void	HitDuaringTheOperation( const Vector3& pos, const int& num );	//	ヒット時動作

	//	情報設定
	void	SetObj( iexMesh* obj );
	void	SetPos( Vector3 pos );
	void	SetScale( float scale );
	void	SetAngle( float angle );

	//	情報取得
	Vector3	GetPos( void )const;
	float		GetAngle( void )const;
	bool		GetState( void )const;
};

//*******************************************************************************
#endif // !__OBJECT_H__

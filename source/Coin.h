
#ifndef __COIN_H__
#define	__COIN_H__

//*****************************************************************************
//
//	Coinクラス
//
//*****************************************************************************
class Coin
{
private:
	const float GRAVITY = -0.01f;

private:
	iexMesh*	obj;
	Vector3		pos;
	Vector3		move;
	float			scale;
	float			angle;
	bool			activate;

public:
	//	初期化・解放
	Coin( void );
	~Coin( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	StageCollisionCheck( void );
	void	Move( void );

	//	情報取得・設定
	void	SetPos( Vector3 pos );
	void	SetAngle( float angle );
	void	SetScale( float angle );
	Vector3	GetPos( void );
	float		GetAngle( void );
};

//*****************************************************************************
#endif // !__COIN_H__

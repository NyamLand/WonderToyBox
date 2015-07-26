
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
	bool			state;
	bool			activate;
	int				judgeTimer;	//	判定有効時間

public:
	//	初期化・解放
	Coin( void );
	~Coin( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	動作関数
	void	StageCollisionCheck( void );
	void	PlayerCollisionCheck( void );
	void	Move( void );

	//	情報取得・設定
	void	SetPos( Vector3 pos );
	void	SetAngle( float angle );
	void	SetScale( float angle );
	Vector3	GetPos( void );
	float		GetAngle( void );

private:
	friend	class CoinManager;
};

//*****************************************************************************
#endif // !__COIN_H__

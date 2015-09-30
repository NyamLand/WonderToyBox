
#ifndef __COIN_H__
#define	__COIN_H__

//*****************************************************************************
//
//	Coinクラス
//
//*****************************************************************************
class Coin
{
protected:


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
	void	Hitduringtheoperation( const Vector3& pos, const int& Num );	//ヒット時動作
	//	情報取得・設定
	void	SetPos( const Vector3& pos );
	void	SetAngle( const float& angle );
	void	SetScale( const float& angle );
	Vector3	GetPos( void );
	float		GetAngle( void );
	bool		GetState( void );

private:
	friend	class CoinManager;
};

//*****************************************************************************
#endif // !__COIN_H__

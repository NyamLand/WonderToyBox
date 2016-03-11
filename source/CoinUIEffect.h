
#pragma once

//**********************************************************************
//
//	CoinUIEffectクラス
//
//**********************************************************************

//	include

//	クラス前方宣言
class Camera;

//	class
class CoinUIEffect
{
private:
	//	パーティクル用
	Vector3	parPos;
	bool		coinFlag[PLAYER_MAX];
	int			coinTimer[PLAYER_MAX];
	int			posX[PLAYER_MAX];
	int			posY;
	int			scale;

	//	パーティクル用バックバッファ
	iex2DObj*	targetPar;

	//	パーティクル用カメラ
	Camera*		camera;

public:
	//	初期化・解放
	CoinUIEffect( void );
	~CoinUIEffect( void );
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( int player );
	void	RenderToBackBaffer( void );
	
	//	情報設定
	void	SetEffectFlag( int playerNum, bool state );
	void	SetPos( int playerNum, int x, int y );
	void	SetScale( int scale );
};

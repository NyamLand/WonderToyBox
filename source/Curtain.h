
#pragma once

//**********************************************************************
//
//	Curtainクラス
//
//**********************************************************************

//	class
class Curtain
{
private:	//	構造体
	struct CurtainInfo
	{
		iex2DObj*	obj;
		TLVERTEX	tlv[4];
		float			t;
	};

public:		//	定数
	enum CURTAIN_MODE
	{
		OPEN,
		CLOSE,
	};

private:	//	パラメータ
	CURTAIN_MODE	mode;
	CurtainInfo	curtainR, curtainL;
	float startPosL[2][4];		//	４頂点２パターン左カーテン
	float	startPosR[2][4];		//	４頂点２パターン右カーテン
	float	endPosL[2][4];		//	４頂点２パターン左カーテン
	float	endPosR[2][4];		//	４頂点２パターン右カーテン
	float speed;
	bool	isEnd;

public:		//	関数
	//	初期化・開放
	Curtain( void );
	~Curtain( void );
	void	StartPosInitialize( void );
	void	EndPosInitialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	Open( void );
	void	Close( void );

	//	情報設定
	void	SetMode( CURTAIN_MODE mode, float speed = 0.01f );

	//	情報取得
	bool	GetIsEnd( void )const;
};

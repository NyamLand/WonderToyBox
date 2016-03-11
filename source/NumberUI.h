
#pragma once

//**********************************************************************
//
//	NumberUIクラス
//
//**********************************************************************

//	include

//-------------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------------

#define	NUMBER_SCALE	100

//	class
class NumberUI
{
private:
	//	数値画像構造体
	struct NumberImageInfo
	{
		int				scale;
		POINT			pos;
		ImageObj		hundred;			//	三桁目
		ImageObj		ten;					//	二桁目
		ImageObj		one;					//	一桁目
		bool				hundredRenderFlag;	//	百の位レンダー用フラグ
	};

	//	数字情報
	struct NumberInfo
	{
		int		hundred;	//コイン三桁目
		int		ten;			//コイン二桁目
		int		one;			//コイン一桁目
		bool		H_flg;		//百の位レンダー用フラグ
	};

private:
	iex2DObj*	orgNumber;
	NumberImageInfo	numImageInfo;
	NumberInfo				numInfo;
	bool				secretflag;
	int				number;	//	数値

public:
	//	初期化・解放
	NumberUI( void );
	~NumberUI( void );
	bool	Initialize( int x, int y, int scale, int num );
	void	Release( void );
	void	Load( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	SetNumberInfo( NumberInfo& numberInfo, int num );
	void	SetNumberImageInfo( NumberImageInfo& numImageInfo, NumberInfo& numInfo, const int& num );

	//	情報設定
	void	SetPos( int x, int y );
	void	SetScale( int scale );
	void	SetColor( const Vector3& color );
	void	SetNumber( int num );
	void	SetSecretFlag( bool state );
	void	SetImage( void );

	//	情報取得
	int	GetPosX( void )const;
	int	GetPosY( void )const;
};

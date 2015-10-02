
#pragma	once

//*******************************************************************************
//
//	Screenクラス
//
//*******************************************************************************

namespace
{
	namespace SCREEN_MODE
	{
		enum
		{
			FADE_IN,
			FADE_OUT,
			WHITE_IN,
			WHITE_OUT,
			END,
		};
	}
}

//	class
class Screen
{
private:
	int			mode;
	float		alpha;
	bool		screenState;
	Vector3	color;

public:
	//	初期化・解放
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	bool	Update( void );
	void	Render( void );

	//	動作関数
	bool	FadeIn( void );
	bool	FadeOut( void );

	//	情報設定
	void	SetScreenMode( int mode );

	//	情報取得
	bool	GetScreenState( void )const;
	static	Screen*	GetInstance( void );
};

#define	screen ( Screen::GetInstance() )
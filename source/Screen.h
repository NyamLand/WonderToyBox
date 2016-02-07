
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
			WIPE_IN,
			WIPE_OUT,
			END,
		};
	}
}

//	include
#include	"iextreme.h"

//	class
class Screen
{
private:
	int		mode;
	float		alpha;
	float		speed;
	bool		screenState;
	int			size;
	float		wipeSize;
	Vector3	color;
	float		param;

private:
	//	初期化・解放
	Screen( void );
	~Screen( void );

public:
	//	更新・描画
	bool	Update( void );
	void	Render( void );

	//	動作関数
	bool	FadeIn( void );
	bool	FadeOut( void );
	bool	WipeIn( void );
	bool	WipeOut( void );

	//	情報設定
	void	SetScreenMode( int mode, float speed );

	//	情報取得
	bool	GetScreenState( void )const;
	static	Screen*	GetInstance( void );
};

#define	screen ( Screen::GetInstance() )
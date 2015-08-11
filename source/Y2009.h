
#ifndef __Y2009_H__
#define	__Y2009_H__

//*******************************************************************************
//
//	Player_Y2009クラス
//
//*******************************************************************************

class Y2009	:	public	Player
{
private:


public:
	//	初期化・解放
	Y2009( void );
	~Y2009( void );
	void	SetMotionData( void );
	
	//	更新・描画
	void	Update( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	動作関数
	void	ModeManagement( void );
	void	Move( void );
	void	Jump( void );
	void	Attack( void );
};

//*******************************************************************************
#endif // !__Y2009_H__

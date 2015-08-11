
#ifndef		__PLAYER_H__
#define		__PLAYER_H__

//****************************************************************************************
//
//	Player関連クラス
//
//****************************************************************************************
class Player : public	BaseObj
{
private:

	//	変数

public:
	//	初期化・解放
	Player( void );
	~Player( void );
	virtual	void	SetMotionData( void ) = 0;

	//	更新・描画
	virtual	void	Update( void ) = 0;
	virtual	void	Render( iexShader* shader, LPSTR technique ) = 0;
	void				Render( void );
	//	動作関数
	virtual	void	ModeManagement( void ) = 0;

};

//****************************************************************************************
#endif // !__PLAYER_H__

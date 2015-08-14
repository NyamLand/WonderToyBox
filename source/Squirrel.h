
#ifndef __SQUIRREL_H__
#define	__SQUIRREL_H__

//*********************************************************************************
//
//	Squirrelクラス
//
//*********************************************************************************
class Squirrel : public		Player
{
private:

public:
	//	初期化・解放
	Squirrel( void );
	~Squirrel( void );
	void	SetMotionData( void );

	//	更新・描画
	void	Update( void );
	void	Render( iexShader* shader, LPSTR technique );

	//	動作関数
	void	ModeManagement( void );
	void	Move( void );
	void	Jump( void );
	void	Attack( int attackKind );
	void	Damage( int type );
	bool	QuickArts( void );
	bool	PowerArts( void );
	bool	HyperArts( void );
	void	Guard( void );
};

//*********************************************************************************
#endif // !__SQUIRREL_H__


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
	bool	QuickArts( void );
	bool	PowerArts( void );
	bool	HyperArts( void );

	//	情報設定
	void	SetAttackParam( int attackKind );
};

//*********************************************************************************
#endif // !__SQUIRREL_H__

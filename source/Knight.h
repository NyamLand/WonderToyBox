
#ifndef __KNIGHT_H__
#define	__KNIGHT_H__

//*********************************************************************************
//
//	Knightクラス
//
//*********************************************************************************
class Knight : public	Player
{
private:

private:
	float	lance_r;

public:
	//	初期化・解放
	Knight( void );
	~Knight( void );
	void	SetMotionData( void );

	//	更新・描画
	void	Render( iexShader* shader, LPSTR technique );

	//	動作関数
	bool	QuickArts( void );
	bool	PowerArts( void );
	bool	HyperArts( void );

	//	情報設定
	void	SetAttackParam( int attackKind );
};

//*********************************************************************************
#endif // !__KNIGHT_H__

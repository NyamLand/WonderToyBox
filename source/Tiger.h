
#ifndef __TIGER_H__
#define	__TIGER_H__

//*******************************************************************************
//
//	Tigerクラス
//
//*******************************************************************************

class Tiger	: public	Player
{
private:
	int		attackCount;
	bool	attackFlag;


public:
	//	初期化・解放
	Tiger( void );
	~Tiger( void );
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


//*******************************************************************************
#endif // !__Tiger_H__

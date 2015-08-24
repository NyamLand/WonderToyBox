
#ifndef __PRINCESS_H__
#define	__PRINCESS_H__

//*********************************************************************************
//
//	Princessクラス
//
//*********************************************************************************
class Princess : public		Player
{
private:


public:
	//	初期化・解放
	Princess( void );
	~Princess( void );
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
#endif // !__PRINCESS_H__

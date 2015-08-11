
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
#endif // !__PRINCESS_H__

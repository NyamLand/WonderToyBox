
#ifndef __ECCMAN_H__
#define	__ECCMAN_H__

//********************************************************************************
//
//	Player_ECCMANクラス
//
//********************************************************************************
class ECCMAN : public	Player
{
private:


public:
	//	初期化・解放
	ECCMAN( void );
	~ECCMAN( void );
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

//********************************************************************************
#endif // !__ECCMAN_H__

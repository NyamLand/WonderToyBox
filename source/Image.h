
#ifndef __IMAGE_H__
#define	__IMAGE_H__

//********************************************************************************
//
//	Imageクラス
//
//********************************************************************************
class Image
{
private:
	iex2DObj*	obj;
	float	 x, y, w, h;
	int		sx, sy, sw, sh;
	float	angle;
	float	alpha;
	float		plusScale_x, plusScale_y;
	POINT	p;
	bool	renderflag;
	int		timer;

public:
	//	初期化・解放
	Image( LPSTR filename );
	~Image( void );
	bool	Initialize( float x, float y, float w, float h, int sx, int sy, int sw, int sh, float angle = 0.0f, float alpha = 1.0f, int timer = 0 );

	//	更新
	void	Update( void );
	void	WaveUpdate( void );

	//	描画
	void	Render( void );	//	個々のパラメータで描画
	void	NormalRender( void );	//	個々のパラメータで通常描画
	void	Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, u32 dwFlags = RS_COPY, DWORD color = 0xFFFFFFFF );
	void	Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, POINT p, float angle, iexShader* shader, LPSTR technique );
	void	Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, POINT p, float ange, u32 dwFlags = RS_COPY, DWORD color = 0xFFFFFFFF );
	void	Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, iexShader* shader, LPSTR technique, DWORD color = 0xFFFFFFFF );

	//	情報設定・取得
	void	SetPos( float x, float y );
	void	SetSize( float w, float h );
	void	SetWave( void );
	void	SetSearchPos( int sx, int sy );
	void	SetSearchSize( int sw, int sh );
};
//********************************************************************************
#endif // !__IMAGE_H__

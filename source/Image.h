
#ifndef __IMAGE_H__
#define	__IMAGE_H__

//********************************************************************************
//
//	Image�N���X
//
//********************************************************************************
class Image
{
private:
	iex2DObj*	obj;
	int	 x, y, w, h, sx, sy, sw, sh;
	float	angle;
	float alpha;
	POINT	p;
	bool	renderflag;
	int		timer;

public:
	//	�������E���
	Image( LPSTR filename );
	~Image( void );
	bool	Initialize( int x, int y, int w, int h, int sx, int sy, int sw, int sh, float angle = 0.0f, float alpha = 1.0f, int timer = 30.0f );

	//	�X�V
	void	Update( void );
	void	WaveUpdate( void );

	//	�`��
	void	Render( void );	//	�X�̃p�����[�^�ŕ`��
	void	Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, u32 dwFlags = RS_COPY, DWORD color = 0xFFFFFFFF );
	void	Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, POINT p, float angle, iexShader* shader, LPSTR technique );
	void	Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, POINT p, float ange, u32 dwFlags = RS_COPY, DWORD color = 0xFFFFFFFF );
	void	Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, iexShader* shader, LPSTR technique, DWORD color = 0xFFFFFFFF );

	//	���ݒ�E�擾
	void	SetPos( int x, int y );
	void	SetWave( void );
};
//********************************************************************************
#endif // !__IMAGE_H__

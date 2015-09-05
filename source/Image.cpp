
#include	"iextreme.h"
#include	"system/System.h"
#include	"GlobalFunction.h"

#include	"Image.h"

//********************************************************************************
//
//	Image�N���X
//
//********************************************************************************

//-----------------------------------------------------------------------------
//	�������E���
//-----------------------------------------------------------------------------

	//	�R���X�g���N�^
	Image::Image( LPSTR filename ) : obj( NULL ), x( 0 ), y( 0 ), w( 0 ), h( 0 ), sx( 0 ), sy( 0 ), sw( 0 ), sh( 0 ),angle( 0.0f ), alpha( 0.0f ), renderflag( false )
	{
		obj = new iex2DObj( filename );
		alpha = 1.0f;
	}

	//	�f�X�g���N�^
	Image::~Image( void )
	{
		SafeDelete( obj );
	}

	//	������
	bool	Image::Initialize( int x, int y, int w, int h, int sx, int sy, int sw, int sh, float angle, float alpha, int timer )
	{
		this->x = x;
		this->y = y;
		this->sx = sx;
		this->sy = sy;
		this->angle = angle;
		this->alpha = alpha;
		this->timer = timer;
		this->plusScale_x = 0;
		this->plusScale_y = 0;
		renderflag = true;

		return	true;
	}

//-----------------------------------------------------------------------------
//	�X�V
//-----------------------------------------------------------------------------

	//	�X�V
	void	Image::Update( void )
	{
		//	�g��X�V
		WaveUpdate();
	}

	//	�g��X�V
	void	Image::WaveUpdate( void )
	{
		static float maxFrame = 30.0f;

		//	�X�V
		timer++;
		alpha -= 1.0f / maxFrame;

		plusScale_x += 140 / ( int )maxFrame;
		plusScale_y += 140 / ( int )maxFrame;

		if ( timer >= ( int )maxFrame )
		{
			alpha = 0.0f;
		}

		if ( timer >= maxFrame * 2 )
		{
			renderflag = false;
		}
	}

//-----------------------------------------------------------------------------
//	�X�V
//-----------------------------------------------------------------------------

	//	�X�̃p�����[�^�ŕ`��
	void	Image::Render( void )
	{
		int		width = w + plusScale_x;
		int		height = h + plusScale_y;
		int		posx = x - w / 2;
		int		posy = y - h / 2;

		if ( renderflag )
		obj->Render( posx, posy, width, height, sx, sy, sw, sh, p, angle, RS_COPY, GetColor( 1.0f, 1.0f, 1.0f, alpha ) );
	}

	//	�X�̃p�����[�^�ŕ`��i �g��E�����x��̗p �j
	void	Image::NormalRender( void )
	{
		int		width = w;
		int		height = h;
		int		posx = x - w / 2;
		int		posy = y - h / 2;

		obj->Render( posx, posy, width, height, sx, sy, sw, sh, p, angle, RS_COPY );
	}

	//	�ʏ�`��
	void	Image::Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, u32 dwFlags, DWORD color )
	{
		obj->Render( x, y, w, h, sx, sy, sw, sh, dwFlags, color );
	}

	//	��]�A�V�F�[�_�[�Ή�
	void	Image::Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, POINT p, float angle, iexShader* shader, LPSTR technique )
	{
		obj->Render( x, y, w, h, sx, sy, sw, sh, p, angle, shader, technique );
	}

	//	��]�Ή�
	void	Image::Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, POINT p, float ange, u32 dwFlags, DWORD color )
	{
		obj->Render( x, y, w, h, sx, sy, sw, sh, p, angle, dwFlags, color );
	}

	//	�V�F�[�_�[�Ή�
	void	Image::Render( int x, int y, int w, int h, int sx, int sy, int sw, int sh, iexShader* shader, LPSTR technique, DWORD color )
	{
		obj->Render( x, y, w, h, sx, sy, sw, sh, shader, technique, color );
	}

//-----------------------------------------------------------------------------
//	���ݒ�
//-----------------------------------------------------------------------------

	//	�g�䔭��
	void	Image::SetWave( void )
	{
		plusScale_x = plusScale_y = 0;
		alpha = 1.0f;
		timer = 0;
		renderflag = true;
	}

	//	���W�ݒ�(�摜�̒��S��ݒ�)
	void	Image::SetPos( int x, int y )
	{
		this->x = x;
		this->y = y;
	}

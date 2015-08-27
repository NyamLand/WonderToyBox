
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
		renderflag = true;

		return	true;
	}

//-----------------------------------------------------------------------------
//	�X�V
//-----------------------------------------------------------------------------

	//	�X�V
	void	Image::Update( void )
	{

	}

	//	�g��X�V
	void	Image::WaveUpdate( void )
	{

	}

//-----------------------------------------------------------------------------
//	�X�V
//-----------------------------------------------------------------------------

	//	�X�̃p�����[�^�ŕ`��
	void	Image::Render( void )
	{
		if ( renderflag )
		obj->Render( x, y, w, h, sx, sy, sw, sh, p, angle, RS_COPY, GetColor( 1.0f, 1.0f, 1.0f, alpha ) );
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
		
	}

	//	���W�ݒ�(�摜�̒��S��ݒ�)
	void	Image::SetPos( int x, int y )
	{
		this->x = x;
		this->y = y;
	}

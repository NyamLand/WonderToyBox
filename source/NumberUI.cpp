
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"NumberUI.h"

//**********************************************************************
//
//	NumberUI�N���X
//
//**********************************************************************

//--------------------------------------------------------------------------------------
//	�O���[�o��
//--------------------------------------------------------------------------------------

#define	SECRET_SRCPOS_X		12 * 64

//--------------------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	NumberUI::NumberUI( void ) : orgNumber( nullptr ),
		secretflag( false )
	{
		numImageInfo.one.obj = nullptr;
		numImageInfo.ten.obj = nullptr;
		numImageInfo.hundred.obj = nullptr;
	}

	//	�f�X�g���N�^
	NumberUI::~NumberUI( void )
	{
		Release();
	}

	//	������
	bool	NumberUI::Initialize( int x, int y, int scale, int num )
	{
		//	���l�ۑ�
		number = num;

		//	�摜�ǂݍ���
		Load();

		//	�摜�ݒ�
		SetImage();

		//	���W������
		SetPos( x, y );

		//	�X�P�[��������
		SetScale( scale );
		
		//	�F�������i���j
		SetColor( Vector3( 0.0f, 0.0f, 0.0f ) );
		
		//	���l�ݒ�
		SetNumber( num );

		//	�B�؂��ݒ�
		SetSecretFlag( false );

		//	�\���̂ɏ��ݒ�
		SetNumberImageInfo( numImageInfo, numInfo, number );

		return	true;
	}

	//	���
	void	NumberUI::Release( void )
	{
		SafeDelete( orgNumber );
	}

	//	�ǂݍ���
	void	NumberUI::Load( void )
	{
		orgNumber = new iex2DObj( "DATA/UI/number.png" );
	}

//--------------------------------------------------------------------------------------
//	�X�V�E�`��
//--------------------------------------------------------------------------------------

	//	�X�V
	void	NumberUI::Update( void )
	{
		//	���l�\���̍X�V
		SetNumberImageInfo( numImageInfo, numInfo, number );

		//	���l�B��
		if ( secretflag )
		{
			SetNumberImageInfo( numImageInfo, numInfo, 10 );
			numImageInfo.hundred.sx = SECRET_SRCPOS_X;
			numImageInfo.ten.sx = SECRET_SRCPOS_X;
			numImageInfo.one.sx = SECRET_SRCPOS_X;
		}
	}
	
	//	�`��
	void	NumberUI::Render( void )
	{
		//	�S�̈ʕ`��
		if ( numImageInfo.hundredRenderFlag )
		{
			RenderImage( numImageInfo.hundred, 
			numImageInfo.hundred.sx,	numImageInfo.hundred.sy, 
			numImageInfo.hundred.sw, numImageInfo.hundred.sh, 
			IMAGE_MODE::ADOPTPARAM );
		}
		
		//	�\�̈ʕ`��
		RenderImage( numImageInfo.ten,
			numImageInfo.ten.sx, numImageInfo.ten.sy,
			numImageInfo.ten.sw, numImageInfo.ten.sh,
			IMAGE_MODE::ADOPTPARAM );

		//	��̈ʕ`��
		RenderImage( numImageInfo.one,
			numImageInfo.one.sx, numImageInfo.one.sy,
			numImageInfo.one.sw, numImageInfo.one.sh,
			IMAGE_MODE::ADOPTPARAM );
	}

//--------------------------------------------------------------------------------------
//	����֐�
//--------------------------------------------------------------------------------------

	//	���l�\���̐ݒ�( ���l�\���́A���l )
	void	NumberUI::SetNumberInfo( NumberInfo& numberInfo, int num )
	{
		numberInfo.hundred = num / 100 % 10;
		numberInfo.ten = num / 10 % 10;
		numberInfo.one = num % 10;
	}

	//	�ݒ肵�����l�ɂ��킹�č\���̏���ݒ�A�P�O�O�ȏォ�Ŕz�u���ύX( �����摜���A�R�C���摜���A�R�C������ )
	void	NumberUI::SetNumberImageInfo( NumberImageInfo& numImageInfo, NumberInfo& numInfo, const int& num )
	{
		//	�����m�F
		if ( num >= 100 )		numImageInfo.hundredRenderFlag = true;
		else							numImageInfo.hundredRenderFlag = false;

		//	�����\���̐ݒ�
		SetNumberInfo( numInfo, num );

		//	�e�ʉ摜�\���̏�����
		int		x, y, w, h, sx, sy, sw, sh;
		//------------------------------------------------------------------------------------------------
		//	�����p�\���̐ݒ�
		//------------------------------------------------------------------------------------------------
		if ( numImageInfo.hundredRenderFlag )
		{
			//	�P�O�̈ʐݒ�
			x = numImageInfo.pos.x;
			y = numImageInfo.pos.y;
			w = h = numImageInfo.scale;
			sx = numInfo.ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( numImageInfo.ten, x, y, w, h, sx, sy, sw, sh );

			//	�P�O�O�̈ʐݒ�
			x = numImageInfo.pos.x - static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx = numInfo.hundred * 64;
			ImageInitialize( numImageInfo.hundred, x, y, w, h, sx, sy, sw, sh );

			//	�P�̈ʐݒ�
			x = numImageInfo.pos.x + static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx = numInfo.one * 64;
			ImageInitialize( numImageInfo.one, x, y, w, h, sx, sy, sw, sh );
		}
		else
		{
			//	�P�O�̈ʐݒ�
			w = h = numImageInfo.scale;
			x = numImageInfo.pos.x - w / 3;
			y = numImageInfo.pos.y;
			sx = numInfo.ten * 64;
			sy = 0;
			sw = sh = 64;
			ImageInitialize( numImageInfo.ten, x, y, w, h, sx, sy, sw, sh );

			//	�P�̈ʐݒ�
			x = numImageInfo.pos.x + w / 3;
			sx = numInfo.one * 64;
			ImageInitialize( numImageInfo.one, x, y, w, h, sx, sy, sw, sh );

			//	�P�O�O�̈ʐݒ�( �������̎��̂݃Z�b�g���邽�� �j
			x = numImageInfo.pos.x - static_cast<int>( numImageInfo.ten.w / 1.5f );
			sx = numInfo.hundred * 64;
			ImageInitialize( numImageInfo.hundred, x, y, w, h, sx, sy, sw, sh );
		}
	}

//--------------------------------------------------------------------------------------
//	���ݒ�
//--------------------------------------------------------------------------------------

	//	���W�ݒ�
	void	NumberUI::SetPos( int x, int y )
	{
		numImageInfo.pos.x = x;
		numImageInfo.pos.y = y;
	}

	//	�X�P�[���ݒ�
	void	NumberUI::SetScale( int scale )
	{
		numImageInfo.scale = scale;
	}

	//	���l�F�ݒ�
	void	NumberUI::SetColor( const Vector3& color )
	{
		numImageInfo.one.color = color;
		numImageInfo.ten.color = color;
		numImageInfo.hundred.color = color;
	}

	//	�B�؂��t���O�ݒ�( true�ŉB�؂� )
	void	NumberUI::SetSecretFlag( bool state )
	{
		secretflag = state;
	}

	//	�摜�ݒ�
	void	NumberUI::SetImage( void )
	{
		numImageInfo.hundred.obj = orgNumber;
		numImageInfo.ten.obj = orgNumber;
		numImageInfo.one.obj = orgNumber;
	}

	//	���l�ݒ�
	void	NumberUI::SetNumber( int num )
	{
		number = num;
	}

//--------------------------------------------------------------------------------------
//	���擾
//--------------------------------------------------------------------------------------

	//	X���W�擾
	int	NumberUI::GetPosX( void )const
	{
		return			numImageInfo.pos.x;
	}

	//	Y���W�擾
	int	NumberUI::GetPosY( void )const
	{
		return	numImageInfo.pos.y;
	}

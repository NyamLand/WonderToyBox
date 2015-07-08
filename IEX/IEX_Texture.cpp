#include	"iextreme.h"

//*****************************************************************************
//
//		�e�N�X�`���֘A
//
//*****************************************************************************

Texture2D*	lpLastTexture;
struct iexTexture::TEXINFO iexTexture::TexInfo[MAX_TEXTURE];

//------------------------------------------------------
//	������
//------------------------------------------------------
void	iexTexture::Initialize()
{
	for( int i=0 ; i<MAX_TEXTURE ; i++ ){
		if( TexInfo[i].lpTexture ) TexInfo[i].lpTexture->Release();
		TexInfo[i].lpTexture = NULL;
		TexInfo[i].UseCount  = 0;
	}
}

//------------------------------------------------------
//	�ǂݍ���
//------------------------------------------------------
Texture2D* iexTexture::Load( char* filename, int flag )
{
	int			i, no;
	HRESULT		hr;
	D3DFORMAT	fmt = D3DFMT_UNKNOWN;

	no = -1;
	//	�e�N�X�`���̌���
	for( i=0 ; i<MAX_TEXTURE ; i++ ){
		if( !TexInfo[i].lpTexture ) continue;
		//	�t�@�C�����̔�r
		if( lstrcmpi( TexInfo[i].filename, filename ) != 0 ) continue; 
		no = i;
		break;
	}
	//	�V�K�ǂݍ���
	if( no == -1 ){
		if( flag == 1 ) fmt = D3DFMT_R8G8B8;

		//	�󂫌���
		for( no=0 ; no<MAX_TEXTURE ; no++ ) if( !TexInfo[no].lpTexture ) break;
		//	�t�@�C�����ۑ�
		CopyMemory( TexInfo[no].filename, filename, strlen(filename)+1 );
		//	�e�N�X�`���ǂݍ���
		hr = D3DXCreateTextureFromFileEx( iexSystem::GetDevice(), filename, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
										fmt, D3DPOOL_DEFAULT,
										D3DX_FILTER_POINT, D3DX_FILTER_POINT,
										0x00000000, NULL, NULL, &TexInfo[no].lpTexture );

		if( FAILED(hr) ) return null;
	}
	TexInfo[no].UseCount++;
	return TexInfo[no].lpTexture;	
}

//------------------------------------------------------
//	���
//------------------------------------------------------
void iexTexture::Release( Texture2D* lpTexture )
{
	int			i, no;

	no = -1;
	//	�e�N�X�`���̌��� 
	for( i=0 ; i<MAX_TEXTURE ; i++ ){
		if( !TexInfo[i].lpTexture ) continue;
		if( TexInfo[i].lpTexture != lpTexture ) continue;
		no = i;
		break;
	}
	if( no != -1 ){
		TexInfo[no].UseCount--;
		if( TexInfo[no].UseCount > 0 ) return;
		//	�e�N�X�`������
		iexSystem::GetDevice()->SetTexture( 0, NULL );
		lpLastTexture = NULL;
		//	�e�N�X�`�����
		if( TexInfo[no].lpTexture->Release() != D3D_OK )
		{
			//	������s
			iexSystem::printf( "*�G���[[iexTexture] ---> ������s: \"%s\"\n", TexInfo[no].filename );
		}
		TexInfo[no].lpTexture = NULL;
		TexInfo[no].UseCount  = 0;
	} else {
		lpTexture->Release();
	}
}


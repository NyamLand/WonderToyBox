#include	"iExtreme.h"

//*****************************************************************************
//
//		シェーダー関連
//
//*****************************************************************************

//*****************************************************************************
//	初期化・解放
//*****************************************************************************
//------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------
iexShader::iexShader( char* filename )
{
	LPDEVICE	lpDevice = iexSystem::GetDevice();
	//	ファイル名チェック
	char fname[256];
	if( filename[ strlen(filename)-3 ] != '.' ) sprintf( fname, "%s.fx", filename );
	else sprintf( fname, "%s", filename );

	// シェーダの読み込み
	HRESULT	hr;
	LPD3DXBUFFER pErr=NULL;
	hr = D3DXCreateEffectFromFile( lpDevice, fname, NULL, NULL, 0 , NULL, &m_pShader, &pErr );
	if( FAILED(hr) )
	{
		char	szBuffer[2000];
		sprintf(szBuffer,"\terrors: %s\n", (char*)pErr->GetBufferPointer() );
		OutputDebugString(szBuffer);
		return;
	}

	//	基本変数の読み込み
	m_hmWVP      = m_pShader->GetParameterByName( NULL, "Projection" );
	m_htexDecale = m_pShader->GetParameterByName( NULL, "Texture" );
}

//------------------------------------------------------
//	デストラクタ
//------------------------------------------------------
iexShader::~iexShader()
{
	m_pShader->Release();
}

//*****************************************************************************
//	パラメータ設定
//*****************************************************************************

void iexShader::SetMatrix( Matrix* mat ){ m_pShader->SetMatrix( m_hmWVP, mat ); }
void iexShader::SetMatrix( Matrix& mat ){ m_pShader->SetMatrix( m_hmWVP, &mat ); }

void iexShader::SetTexture( Texture2D* tex ){ m_pShader->SetTexture( m_htexDecale, tex ); }
void iexShader::SetTexture( Texture2D& tex ){ m_pShader->SetTexture( m_htexDecale, &tex ); }

void iexShader::SetValue( char* name, iex2DObj* obj ){ m_pShader->SetTexture( name, obj->GetTexture() ); }
void iexShader::SetValue( char* name, iex2DObj& obj ){ m_pShader->SetTexture( name, obj.GetTexture() ); }

void iexShader::SetValue( char* name, Texture2D* tex ){ m_pShader->SetTexture( name, tex ); }
void iexShader::SetValue( char* name, Texture2D& tex ){ m_pShader->SetTexture( name, &tex ); }

void iexShader::SetValue( char* name, Matrix* mat ){ m_pShader->SetMatrix( name, mat ); }
void iexShader::SetValue( char* name, Matrix& mat ){ m_pShader->SetMatrix( name, &mat ); }

void iexShader::SetValue( char* name, D3DXVECTOR4* v ){ m_pShader->SetVector( name, v ); }
void iexShader::SetValue( char* name, D3DXVECTOR4& v ){ m_pShader->SetVector( name, &v ); }

void iexShader::SetValue( char* name, Vector3* v ){ m_pShader->SetFloatArray( name, (float*)v, 3 ); }
void iexShader::SetValue( char* name, Vector3& v ){ m_pShader->SetFloatArray( name, (float*)&v, 3 ); }


void iexShader::SetValue( char* name, float f ){ m_pShader->SetFloat( name, f ); }
void iexShader::SetValue( char* name, int d ){ m_pShader->SetInt( name, d ); }
void iexShader::SetValue( char* name, DWORD d ){ m_pShader->SetValue( name, &d, 4 ); }






#include	"iExtreme.h"

//*****************************************************************************
//
//		ライト関連
//
//*****************************************************************************

//*****************************************************************************
//	環境光
//*****************************************************************************
void	iexLight::SetAmbient( DWORD ambient )
{
	iexSystem::Device->SetRenderState( D3DRS_AMBIENT, ambient );
}

//*****************************************************************************
//	平行光源
//*****************************************************************************
void iexLight::DirLight( iexShader* shader, int index, Vector3* dir, float r, float g, float b )
{
	dir->Normalize();
	DirLight( index, dir, r, g, b );
	//	シェーダー設定
	Vector3	work = *dir;
	Vector3 color( r, g, b );
	shader->SetValue( "LightDir", work );
	shader->SetValue( "DirLightColor", &color );
}

void iexLight::DirLight( int index, Vector3* dir, float r, float g, float b )
{
	D3DLIGHT9 Light;

	ZeroMemory( &Light, sizeof(D3DLIGHT9) );
	Light.Type			= D3DLIGHT_DIRECTIONAL;
	Light.Diffuse.r		= r;
	Light.Diffuse.g		= g;
	Light.Diffuse.b		= b;
	Light.Diffuse.a		= 1.0f;
	Light.Specular.r	= r;
	Light.Specular.g	= g;
	Light.Specular.b	= b;
	Light.Specular.a	= 1.0f;
	Light.Ambient.r		= .0f;
	Light.Ambient.g		= .0f;
	Light.Ambient.b		= .0f;
	Light.Ambient.a		= 1.0f;
	Light.Direction.x	= dir->x;
	Light.Direction.y	= dir->y;
	Light.Direction.z	= dir->z;
	iexSystem::Device->SetLight( index, &Light );
	iexSystem::Device->LightEnable( index, TRUE );
}

//*****************************************************************************
//	点光源
//*****************************************************************************
void iexLight::PointLight( int index, Vector3* Pos, float r, float g, float b, float range )
{
	D3DLIGHT9 Light;

	ZeroMemory( &Light, sizeof(D3DLIGHT9) );
	Light.Type			= D3DLIGHT_POINT;
	Light.Diffuse.r		= r;
	Light.Diffuse.g		= g;
	Light.Diffuse.b		= b;
	Light.Diffuse.a		= 1.0f;
	Light.Specular.r	= r;
	Light.Specular.g	= g;
	Light.Specular.b	= b;
	Light.Ambient.r		= .0f;
	Light.Ambient.g		= .0f;
	Light.Ambient.b		= .0f;
	Light.Ambient.a		= .0f;
	Light.Direction.x	= .0f;
	Light.Direction.y	= .0f;
	Light.Direction.z	= .0f;
	Light.Position.x	= Pos->x;
	Light.Position.y	= Pos->y;
	Light.Position.z	= Pos->z;
	Light.Range         = range;
	Light.Attenuation0	= 1.0f;
	Light.Attenuation1	= 0.8f / range;
	Light.Attenuation2	= .0f;

	iexSystem::Device->SetLight( index, &Light );
	iexSystem::Device->LightEnable( index, TRUE );
}

//*****************************************************************************
//	フォグ
//*****************************************************************************
void	iexLight::SetFog( float Param1, float Param2, DWORD Color )
{
	iexSystem::Device->SetRenderState(D3DRS_FOGENABLE, TRUE);
	//	フォグカラー設定
	iexSystem::Device->SetRenderState(D3DRS_FOGCOLOR, Color);
	//	フォグモード設定
	iexSystem::Device->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_NONE );
	iexSystem::Device->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_LINEAR );
	//	パラメータ設定
	iexSystem::Device->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Param1));
	iexSystem::Device->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&Param2));
}


//*****************************************************************************************************************************
//
//	過去のＩＥＸとの互換用
//
//*****************************************************************************************************************************

void	IEX_ResetLight( int index )
{
	if( index != -1 ) iexSystem::Device->LightEnable( index, FALSE );
	 else {
		int		i;
        for( i=0 ; i<16 ; i++ ) iexSystem::Device->LightEnable( i, FALSE );
	}
}

//		環境光設定
void	IEX_SetAmbient( DWORD Color ){ iexLight::SetAmbient(Color); }

//		平行光作成
void	IEX_SetDirLight( int index, Vector3* dir, float r, float g, float b ){ iexLight::DirLight( index, dir, r, g, b ); }

//		点光源作成
void	IEX_SetPointLight( int index, Vector3* Pos, float r, float g, float b, float range ){ iexLight::PointLight( index, Pos, r, g, b, range ); }

//		フォグ設定
void	IEX_SetFog( DWORD Mode, float Param1, float Param2, DWORD Color ){ iexLight::SetFog(Param1, Param2, Color ); }


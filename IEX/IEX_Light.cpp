#include	"iExtreme.h"

//*****************************************************************************
//
//		���C�g�֘A
//
//*****************************************************************************

//*****************************************************************************
//	����
//*****************************************************************************
void	iexLight::SetAmbient( DWORD ambient )
{
	iexSystem::Device->SetRenderState( D3DRS_AMBIENT, ambient );
}

//*****************************************************************************
//	���s����
//*****************************************************************************
void iexLight::DirLight( iexShader* shader, int index, Vector3* dir, float r, float g, float b )
{
	dir->Normalize();
	DirLight( index, dir, r, g, b );
	//	�V�F�[�_�[�ݒ�
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
//	�_����
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
//	�t�H�O
//*****************************************************************************
void	iexLight::SetFog( float Param1, float Param2, DWORD Color )
{
	iexSystem::Device->SetRenderState(D3DRS_FOGENABLE, TRUE);
	//	�t�H�O�J���[�ݒ�
	iexSystem::Device->SetRenderState(D3DRS_FOGCOLOR, Color);
	//	�t�H�O���[�h�ݒ�
	iexSystem::Device->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_NONE );
	iexSystem::Device->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_LINEAR );
	//	�p�����[�^�ݒ�
	iexSystem::Device->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Param1));
	iexSystem::Device->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&Param2));
}


//*****************************************************************************************************************************
//
//	�ߋ��̂h�d�w�Ƃ̌݊��p
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

//		�����ݒ�
void	IEX_SetAmbient( DWORD Color ){ iexLight::SetAmbient(Color); }

//		���s���쐬
void	IEX_SetDirLight( int index, Vector3* dir, float r, float g, float b ){ iexLight::DirLight( index, dir, r, g, b ); }

//		�_�����쐬
void	IEX_SetPointLight( int index, Vector3* Pos, float r, float g, float b, float range ){ iexLight::PointLight( index, Pos, r, g, b, range ); }

//		�t�H�O�ݒ�
void	IEX_SetFog( DWORD Mode, float Param1, float Param2, DWORD Color ){ iexLight::SetFog(Param1, Param2, Color ); }


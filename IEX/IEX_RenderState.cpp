#include	"iExtreme.h"

//**************************************************************************************************
//
//	�����_�[�X�e�[�g�ݒ�
//
//**************************************************************************************************

//------------------------------------------------------
//	�����_�[�X�e�[�g������
//------------------------------------------------------
void iexRenderState::Initialize()
{
	//	�|���S���`��ݒ�
	iexSystem::Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	iexSystem::Device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	iexSystem::Device->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );

	//	�A���t�@�u�����h�ݒ�
	iexSystem::Device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	iexSystem::Device->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	iexSystem::Device->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_ALWAYS );
	iexSystem::Device->SetRenderState( D3DRS_ALPHAREF, 0 );
	//	�u�����h�ݒ� 
	iexSystem::Device->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	iexSystem::Device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_CURRENT );
	iexSystem::Device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE );
	iexSystem::Device->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	iexSystem::Device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_CURRENT );
	iexSystem::Device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );

	//	�e�N�X�`���ݒ�
	iexSystem::Device->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE  );
	iexSystem::Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	iexSystem::Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	iexSystem::Device->SetSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_WRAP );
	iexSystem::Device->SetSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_WRAP );
	
	//	���C�e�B���O�ݒ�
	iexLight::SetAmbient( 0x20202020 );
	iexSystem::Device->SetRenderState( D3DRS_LIGHTING, TRUE );
	
	//	�X�N���[���ݒ�
	iexSystem::Device->SetRenderState( D3DRS_ZENABLE, TRUE );
	iexSystem::Device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );

	iexSystem::Device->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );

	IEX_SetProjection( D3DX_PI/4, 10.0f, 1500.0f );
}

//------------------------------------------------------
//	�����_�[�X�e�[�g�ݒ�
//------------------------------------------------------
void iexRenderState::Set( DWORD state, D3DMATERIAL9* lpMaterial, Texture2D* lpTexture )
{
	switch( state ){
	case RS_COPY:
			iexSystem::Device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
			iexSystem::Device->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			iexSystem::Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			iexSystem::Device->SetRenderState( D3DRS_FOGENABLE, TRUE );
			iexSystem::Device->SetRenderState( D3DRS_LIGHTING, TRUE );
			iexSystem::Device->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
			break;
	case RS_LCOPY:
			iexSystem::Device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
			iexSystem::Device->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			iexSystem::Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			iexSystem::Device->SetRenderState( D3DRS_FOGENABLE, TRUE );
			iexSystem::Device->SetRenderState( D3DRS_LIGHTING, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
			break;

	case RS_ADD:	//	���Z����
			iexSystem::Device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
			iexSystem::Device->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			iexSystem::Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			iexSystem::Device->SetRenderState( D3DRS_FOGENABLE, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_LIGHTING, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
			break;

	case RS_SUB:
			iexSystem::Device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT );
			iexSystem::Device->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
			iexSystem::Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			iexSystem::Device->SetRenderState( D3DRS_FOGENABLE, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_LIGHTING, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
			break;

	case RS_MUL:
			iexSystem::Device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
			iexSystem::Device->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO     );
			iexSystem::Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
			iexSystem::Device->SetRenderState( D3DRS_FOGENABLE, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_LIGHTING, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
			break;

	case RS_NEGA:
			iexSystem::Device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
			iexSystem::Device->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR );
			iexSystem::Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
			iexSystem::Device->SetRenderState( D3DRS_FOGENABLE, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_LIGHTING, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
			break;

	case RS_INVERT:	/*	���]		*/ 		
			iexSystem::Device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT );
			iexSystem::Device->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			iexSystem::Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			iexSystem::Device->SetRenderState( D3DRS_FOGENABLE, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_LIGHTING, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
			break;
	}

	//	�}�e���A���ݒ�
	if( lpMaterial ) iexSystem::Device->SetMaterial( lpMaterial );
	//	�e�N�X�`���ݒ�
	iexSystem::Device->SetTexture( 0, lpTexture );
}

//*****************************************************************************
//	
//*****************************************************************************

//------------------------------------------------------
//	�t�B���^�n�m�E�n�e�e
//------------------------------------------------------
void	iexRenderState::Filter( BOOL bFilter )
{
	if( bFilter ){
		iexSystem::Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		iexSystem::Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	} else {
		iexSystem::Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		iexSystem::Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	}
}








//*****************************************************************************************************************************
//
//	�ߋ��̂h�d�w�Ƃ̌݊��p
//
//*****************************************************************************************************************************
//		�����_�[�X�e�[�g�ݒ�
void	IEX_InitRenderState( void ){ iexRenderState::Initialize(); }
//		�����_�[�X�e�[�g�ݒ�
void IEX_SetRenderState( DWORD state, D3DMATERIAL9* lpMaterial, LPDIRECT3DTEXTURE9 lpTexture ){ iexRenderState::Set( state, lpMaterial, lpTexture ); }
//		�t�B���^�[�ݒ�
void	IEX_UseFilter( BOOL bFilter ){ iexRenderState::Filter(bFilter); }


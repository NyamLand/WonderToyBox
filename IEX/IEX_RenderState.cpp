#include	"iExtreme.h"

//**************************************************************************************************
//
//	レンダーステート設定
//
//**************************************************************************************************

//------------------------------------------------------
//	レンダーステート初期化
//------------------------------------------------------
void iexRenderState::Initialize()
{
	//	ポリゴン描画設定
	iexSystem::Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	iexSystem::Device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	iexSystem::Device->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );

	//	アルファブレンド設定
	iexSystem::Device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	iexSystem::Device->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	iexSystem::Device->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_ALWAYS );
	iexSystem::Device->SetRenderState( D3DRS_ALPHAREF, 0 );
	//	ブレンド設定 
	iexSystem::Device->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	iexSystem::Device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_CURRENT );
	iexSystem::Device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE );
	iexSystem::Device->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	iexSystem::Device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_CURRENT );
	iexSystem::Device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );

	//	テクスチャ設定
	iexSystem::Device->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE  );
	iexSystem::Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	iexSystem::Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	iexSystem::Device->SetSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_WRAP );
	iexSystem::Device->SetSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_WRAP );
	
	//	ライティング設定
	iexLight::SetAmbient( 0x20202020 );
	iexSystem::Device->SetRenderState( D3DRS_LIGHTING, TRUE );
	
	//	スクリーン設定
	iexSystem::Device->SetRenderState( D3DRS_ZENABLE, TRUE );
	iexSystem::Device->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );

	iexSystem::Device->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );

	IEX_SetProjection( D3DX_PI/4, 10.0f, 1500.0f );
}

//------------------------------------------------------
//	レンダーステート設定
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

	case RS_ADD:	//	加算合成
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

	case RS_INVERT:	/*	反転		*/ 		
			iexSystem::Device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT );
			iexSystem::Device->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
			iexSystem::Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			iexSystem::Device->SetRenderState( D3DRS_FOGENABLE, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_LIGHTING, FALSE );
			iexSystem::Device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
			break;
	}

	//	マテリアル設定
	if( lpMaterial ) iexSystem::Device->SetMaterial( lpMaterial );
	//	テクスチャ設定
	iexSystem::Device->SetTexture( 0, lpTexture );
}

//*****************************************************************************
//	
//*****************************************************************************

//------------------------------------------------------
//	フィルタＯＮ・ＯＦＦ
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
//	過去のＩＥＸとの互換用
//
//*****************************************************************************************************************************
//		レンダーステート設定
void	IEX_InitRenderState( void ){ iexRenderState::Initialize(); }
//		レンダーステート設定
void IEX_SetRenderState( DWORD state, D3DMATERIAL9* lpMaterial, LPDIRECT3DTEXTURE9 lpTexture ){ iexRenderState::Set( state, lpMaterial, lpTexture ); }
//		フィルター設定
void	IEX_UseFilter( BOOL bFilter ){ iexRenderState::Filter(bFilter); }


#include	"iExtreme.h"

//*****************************************************************************
//
//		２Ｄオブジェクト
//
//*****************************************************************************
//------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------
iex2DObj::iex2DObj( char* filename )
{
	//	情報初期化
	width = height = 0;
	lpSurface = NULL;

	//	テクスチャ読み込み
	lpTexture = iexTexture::Load(filename);
	if( lpTexture == NULL ) return;

	//	サイズ保存
	D3DSURFACE_DESC	sd;
	lpTexture->GetLevelDesc( 0, &sd );
	width  = sd.Width;
	height = sd.Height;
	//	メインサーフェス取得
	lpTexture->GetSurfaceLevel( 0, &lpSurface );
}

iex2DObj::iex2DObj( u32 width, u32 height, u8 flag )
{
	//	情報初期化
	this->width = this->height = 0;
	lpSurface = NULL;

	D3DFORMAT	fmt = D3DFMT_R8G8B8;
	u32			usage = 0;
	D3DPOOL		pool = D3DPOOL_MANAGED;

	switch( flag ){
	case IEX2D_RENDERTARGET:	//	レンダー可能
					usage = D3DUSAGE_RENDERTARGET;
					fmt = iexSystem::ScreenFormat;
					pool = D3DPOOL_DEFAULT;
					break;
	case IEX2D_FLOAT2:	//	レンダー可能
					usage = D3DUSAGE_RENDERTARGET;
					fmt = D3DFMT_G32R32F;
					pool = D3DPOOL_DEFAULT;
					break;
	case IEX2D_FLOAT:	//	レンダー可能
					usage = D3DUSAGE_RENDERTARGET;
					fmt = D3DFMT_R32F;
					pool = D3DPOOL_DEFAULT;
					break;

	case IEX2D_USEALPHA:		//	透明度付
					fmt = D3DFMT_A8R8G8B8;
					break;
	case IEX2D_SYSTEMMEM:
					fmt = iexSystem::ScreenFormat;
					pool = D3DPOOL_SYSTEMMEM;
					break;
	}
	dwFlags= 0;
	//	テクスチャ作成
	D3DXCreateTexture( iexSystem::Device, width, height, 0, usage, fmt, pool, &lpTexture );
	if( lpTexture == NULL ) return;

	//	サイズ保存
	D3DSURFACE_DESC	sd;
	lpTexture->GetLevelDesc( 0, &sd );
	this->width  = sd.Width;
	this->height = sd.Height;
	//	メインサーフェス取得
	lpTexture->GetSurfaceLevel( 0, &lpSurface );
}

//*****************************************************************************
//	レンダーターゲット設定
//*****************************************************************************
void	iex2DObj::RenderTarget( int index )
{
	iexSystem::Device->SetRenderTarget( index,lpSurface );
}


//*****************************************************************************
//	レンダリング
//*****************************************************************************

void iex2DObj::Render()
{
	TLVERTEX	v[4];
	//	全画面設定
	v[0].sx = v[2].sx = 0;
	v[1].sx = v[3].sx = iexSystem::ScreenWidth - 0.5f;
	v[0].sy = v[1].sy = 0;
	v[2].sy = v[3].sy = iexSystem::ScreenHeight - 0.5f;

	v[0].tu = v[2].tu = 0;
	v[1].tu = v[3].tu = 1;
	v[0].tv = v[1].tv = 0;
	v[2].tv = v[3].tv = 1;

	v[0].color = v[1].color = v[2].color = v[3].color = 0xFFFFFFFF;
	v[0].sz    = v[1].sz    = v[2].sz    = v[3].sz    = 0;
	v[0].rhw   = v[1].rhw   = v[2].rhw   = v[3].rhw   = 1.0f;

	iexPolygon::Render2D( v, 2, this, RS_COPY );
}

void iex2DObj::Render( iexShader* shader, char* tech )
{
	TLVERTEX	v[4];
	//	全画面設定
	v[0].sx = v[2].sx = 0;
	v[1].sx = v[3].sx = iexSystem::ScreenWidth - 0.5f;
	v[0].sy = v[1].sy = 0;
	v[2].sy = v[3].sy = iexSystem::ScreenHeight - 0.5f;

	v[0].tu = v[2].tu = 0;
	v[1].tu = v[3].tu = 1;
	v[0].tv = v[1].tv = 0;
	v[2].tv = v[3].tv = 1;

	v[0].color = v[1].color = v[2].color = v[3].color = 0xFFFFFFFF;
	v[0].sz    = v[1].sz    = v[2].sz    = v[3].sz    = 0;
	v[0].rhw   = v[1].rhw   = v[2].rhw   = v[3].rhw   = 1.0f;

	iexPolygon::Render2D( v, 2, this, shader, tech );
}

void iex2DObj::Render( s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, u32 dwFlags, COLOR color, float z )
{
	TLVERTEX	v[4];
	v[0].sx = v[2].sx = (float)DstX;
	v[1].sx = v[3].sx = (float)(DstX+DstW)-0.5f;
	v[0].sy = v[1].sy = (float)DstY;
	v[2].sy = v[3].sy = (float)(DstY+DstH)-0.5f;

	v[0].tu = v[2].tu = (float)(SrcX+0.5f)/(float)this->width;
	v[1].tu = v[3].tu = (float)(SrcX+width)/(float)this->width;
	v[0].tv = v[1].tv = (float)(SrcY+0.5f)/(float)this->height;
	v[2].tv = v[3].tv = (float)(SrcY+height)/(float)this->height;

	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].sz    = v[1].sz    = v[2].sz    = v[3].sz    = z;
	v[0].rhw   = v[1].rhw   = v[2].rhw   = v[3].rhw   = 1.0f;

	iexPolygon::Render2D( v, 2, this, dwFlags );
}

void iex2DObj::Render( s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, iexShader* shader, char* tech, COLOR color, float z )
{
	TLVERTEX	v[4];
	v[0].sx = v[2].sx = (float)DstX;
	v[1].sx = v[3].sx = (float)(DstX+DstW)-0.5f;
	v[0].sy = v[1].sy = (float)DstY;
	v[2].sy = v[3].sy = (float)(DstY+DstH)-0.5f;

	v[0].tu = v[2].tu = (float)(SrcX+0.5f)/(float)this->width;
	v[1].tu = v[3].tu = (float)(SrcX+width)/(float)this->width;
	v[0].tv = v[1].tv = (float)(SrcY+0.5f)/(float)this->height;
	v[2].tv = v[3].tv = (float)(SrcY+height)/(float)this->height;

	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].sz    = v[1].sz    = v[2].sz    = v[3].sz    = z;
	v[0].rhw   = v[1].rhw   = v[2].rhw   = v[3].rhw   = 1.0f;

	iexPolygon::Render2D( v, 2, this, shader, tech );
}

//	回転対応
void	iex2DObj::Render( s32 x, s32 y, s32 w, s32 h, s32 sx, s32 sy, s32 sw, s32 sh, POINT p,
			float angle, u32 dwFlags, COLOR color, float z )
{
	TLVERTEX	v2[4];
	v2[0].sx = v2[2].sx = (float)x;
	v2[1].sx = v2[3].sx = (float)(x + w) - 0.5f;
	v2[0].sy = v2[1].sy = (float)y;
	v2[2].sy = v2[3].sy = (float)(y + h) - 0.5f;

	TLVERTEX	v[4];
	v[0].sx = v[2].sx = (float)x;
	v[1].sx = v[3].sx = (float)(x + w) - 0.5f;
	v[0].sy = v[1].sy = (float)y;
	v[2].sy = v[3].sy = (float)(y + h) - 0.5f;

	for (int i = 0; i < 4; i++)
	{
		v[i].sx = cosf(angle) * (v2[i].sx - p.x) - sinf(angle) * (v2[i].sy - p.y) + p.x;
		v[i].sy = sinf(angle) * (v2[i].sx - p.x) + cosf(angle) * (v2[i].sy - p.y) + p.y;
	}

	v[0].tu = v[2].tu = (float)(sx + 0.5f) / (float)this->width;
	v[1].tu = v[3].tu = (float)(sx + sw) / (float)this->width;
	v[0].tv = v[1].tv = (float)(sy + 0.5f) / (float)this->height;
	v[2].tv = v[3].tv = (float)(sy + sh) / (float)this->height;


	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].sz = v[1].sz = v[2].sz = v[3].sz = z;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;

	iexPolygon::Render2D( v, 2, this, dwFlags );
}

//	回転とシェーダー対応
void	iex2DObj::Render( s32 x, s32 y, s32 w, s32 h, s32 sx, s32 sy, s32 sw, s32 sh, POINT p,
	float angle, iexShader* shader, char* tech, u32 dwFlags, COLOR color, float z )
{
	TLVERTEX	v2[4];
	v2[0].sx = v2[2].sx = (float)x;
	v2[1].sx = v2[3].sx = (float)(x + w) - 0.5f;
	v2[0].sy = v2[1].sy = (float)y;
	v2[2].sy = v2[3].sy = (float)(y + h) - 0.5f;

	TLVERTEX	v[4];
	v[0].sx = v[2].sx = (float)x;
	v[1].sx = v[3].sx = (float)(x + w) - 0.5f;
	v[0].sy = v[1].sy = (float)y;
	v[2].sy = v[3].sy = (float)(y + h) - 0.5f;

	for (int i = 0; i < 4; i++)
	{
		v[i].sx = cosf(angle) * (v2[i].sx - p.x) - sinf(angle) * (v2[i].sy - p.y) + p.x;
		v[i].sy = sinf(angle) * (v2[i].sx - p.x) + cosf(angle) * (v2[i].sy - p.y) + p.y;
	}

	v[0].tu = v[2].tu = (float)(sx + 0.5f) / (float)this->width;
	v[1].tu = v[3].tu = (float)(sx + sw) / (float)this->width;
	v[0].tv = v[1].tv = (float)(sy + 0.5f) / (float)this->height;
	v[2].tv = v[3].tv = (float)(sy + sh) / (float)this->height;


	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].sz = v[1].sz = v[2].sz = v[3].sz = z;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;

	iexPolygon::Render2D(v, 2, this, shader, tech);

}

//**************************************************************************************************
//
//		過去のIEXとの互換用
//
//**************************************************************************************************

void	IEX_Render2DObject( int DstX, int DstY, int DstW, int DstH, LPIEX2DOBJ lpObj, int SrcX, int SrcY, int width, int height, u32 dwFlags, u32 color )
{
	if( !lpObj ) return;
	lpObj->Render(DstX, DstY, DstW, DstH, SrcX, SrcY, width, height, dwFlags, color );
}


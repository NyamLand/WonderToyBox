#include	"iExtreme.h"

//*****************************************************************************
//
//		パーティクル単体
//
//*****************************************************************************

//*****************************************************************************
//		セット
//*****************************************************************************
void iexParticleData::Set( LPPARTICLE pd )
{
	memcpy( &pdata, pd, sizeof(PARTICLE) );
	CurFrame = 0;
	CurAlpha = 0;
	bFlags = 1;
}

//*****************************************************************************
//		更新
//*****************************************************************************
void iexParticleData::Update()
{
	float	work;
	int	a1, r1, g1, b1, a2, r2, g2, b2;

	if(	bFlags == 0 ) return;

	//	情報更新
	pdata.Pos   += pdata.Move;
	pdata.Move  += pdata.Power;
	pdata.angle += pdata.rotate;
	pdata.scale *= pdata.stretch;

	//	カラー計算
	if( CurFrame < pdata.aFrame ) CurAlpha = .0f;
	 else {
		if( CurFrame < pdata.mFrame ){
			work = (CurFrame - pdata.aFrame) / (float)(pdata.mFrame - pdata.aFrame);
			a1 = (  pdata.aColor>>24)         ;
			r1 = ( (pdata.aColor>>16) & 0xFF );
			g1 = ( (pdata.aColor>> 8) & 0xFF );
			b1 = ( (pdata.aColor    ) & 0xFF );
		} else {
			work = (pdata.eFrame - CurFrame) / (float)(pdata.eFrame - pdata.mFrame);
			a1 = (  pdata.eColor>>24)         ;
			r1 = ( (pdata.eColor>>16) & 0xFF );
			g1 = ( (pdata.eColor>> 8) & 0xFF );
			b1 = ( (pdata.eColor    ) & 0xFF );
		}

		a2 = (  pdata.mColor>>24)         ;
		r2 = ( (pdata.mColor>>16) & 0xFF );
		g2 = ( (pdata.mColor>> 8) & 0xFF );
		b2 = ( (pdata.mColor    ) & 0xFF );
		//	カラー設定
		CurAlpha = ((a2 - a1)*work + a1) / 255.0f;
		r        = ((r2 - r1)*work + r1) / 255.0f;
		g        = ((g2 - g1)*work + g1) / 255.0f;
		b        = ((b2 - b1)*work + b1) / 255.0f;
	}

	//	フレーム進行
	CurFrame ++;
	if( CurFrame >= pdata.eFrame ) bFlags = 0;
}

//*****************************************************************************
//		描画
//*****************************************************************************
//------------------------------------------------------
//		ポリゴン生成
//------------------------------------------------------
inline bool iexParticleData::SetVertex( TLVERTEX* v )
{
	D3DVIEWPORT9	vp;
	iexSystem::Device->GetViewport(&vp);

	Matrix mat2 = iexParticle::CurViewProjection;

	float z1 = pdata.Pos.x * matView._13 + pdata.Pos.y * matView._23 + pdata.Pos.z * matView._33 + matView._43;

	//	位置変換
	Vector3	pos;
	pos.z = pdata.Pos.x * mat2._13 + pdata.Pos.y * mat2._23 + pdata.Pos.z * mat2._33 + mat2._43;
	if( pos.z <= 0 ) return false;

	pos.x = pdata.Pos.x * mat2._11 + pdata.Pos.y * mat2._21 + pdata.Pos.z * mat2._31 + mat2._41;
	pos.y = pdata.Pos.x * mat2._12 + pdata.Pos.y * mat2._22 + pdata.Pos.z * mat2._32 + mat2._42;

	float pw = pdata.Pos.x * mat2._14 + pdata.Pos.y * mat2._24 + pdata.Pos.z * mat2._34 + mat2._44;

	float	x1, x2, y1, y2, s, c;
	s = sinf(pdata.angle) * pdata.scale;
	c = cosf(pdata.angle) * pdata.scale;
	//	頂点設定
	x1 = s - c;
	x2 = c + s;
	y1 = c + s;
	y2 = c - s;
	//	投影サイズ計算
	float px, py, px2, py2;
	px  = x1*matProjection._11 + y1*matProjection._21 + z1*matProjection._31 + matProjection._41;
	py  = x1*matProjection._12 + y1*matProjection._22 + z1*matProjection._32 + matProjection._42;
	px2 = x2*matProjection._11 + y2*matProjection._21 + z1*matProjection._31 + matProjection._41;
	py2 = x2*matProjection._12 + y2*matProjection._22 + z1*matProjection._32 + matProjection._42;

	v[0].sx = (pos.x + px)  / pw;
	v[0].sy = (pos.y + py)  / pw;
	v[1].sx = (pos.x + px2) / pw;
	v[1].sy = (pos.y + py2) / pw;
	v[2].sx = (pos.x - px2) / pw;
	v[2].sy = (pos.y - py2) / pw;
	v[3].sx = (pos.x - px)  / pw;
	v[3].sy = (pos.y - py)  / pw;

	v[0].sz = v[1].sz = v[2].sz = v[3].sz = pos.z / pw;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;

	int w = vp.Width;
	int h = vp.Height;

	bool bRender = false;
	for( int i=0 ; i<4 ; i++ )
	{
		if( v[i].sx > -1.0f && v[i].sx < 1.0f ) bRender = true; 
		if( v[i].sy > -1.0f && v[i].sy < 1.0f ) bRender = true; 
		v[i].sx = (v[i].sx+1.0f) * w/2 + vp.X;
		v[i].sy = (-v[i].sy+1.0f) * h/2 + vp.Y;
	}

	if( bRender == false ) return false;
	//	タイプ設定
	v[0].tu = v[2].tu = (float)(pdata.type%4) * 0.25f + 0.001f;
	v[1].tu = v[3].tu = v[0].tu + 0.248f;
	v[0].tv = v[1].tv = (float)(pdata.type/4) * 0.25f + 0.001f;
	v[3].tv = v[2].tv = v[0].tv + 0.248f;

	//	色設定
	v[0].color = v[1].color = v[2].color = v[3].color = ((DWORD)(CurAlpha*255.0f)<<24) | ((DWORD)(r*255.0f)<<16) | ((DWORD)(g*255.0f)<<8) | ((DWORD)(b*255.0f));

	return true;
}

//------------------------------------------------------
//		固定機能描画
//------------------------------------------------------
void	iexParticleData::Render()
{
	TLVERTEX	v[4];

	if( SetVertex(v) == false ) return;
	//	合成設定
	if( pdata.flag == RS_SUB ) iexSystem::Device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT );
	 else                      iexSystem::Device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	//	レンダリング
	iexSystem::Device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(TLVERTEX) );
}

//------------------------------------------------------
//		シェーダー描画
//------------------------------------------------------
void	iexParticleData::Render(iexShader* shader, char* name )
{
	if( pdata.flag != RS_SHADER ){ Render(); return; }

	TLVERTEX	v[4];
	SetVertex(v);

	//	シェーダーの適用
	u32 pass = shader->Begine(name);

	for( u32 p=0 ; p<pass ; p++ )
	{
		shader->BeginePass(p);
		shader->CommitChanges();
		//	レンダリング
		iexSystem::Device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(TLVERTEX) );
		shader->EndPass();
	}
	shader->End();
}




//**************************************************************************************************
//
//		パーティクル管理クラス
//
//**************************************************************************************************

int				 iexParticle::nParticles;	//	パーティクル最大数
iexParticleData* iexParticle::ParticleData;	//	パーティクルデータ
iex2DObj*        iexParticle::lpTexture;	//	テクスチャ

Matrix			 iexParticle::CurViewProjection;

//*****************************************************************************
//	初期化・解放
//*****************************************************************************
//------------------------------------------------------
//		初期化
//------------------------------------------------------
void iexParticle::Initialize( char* filename, int nParticles )
{
	//	パーティクルバッファ確保
	iexParticle::nParticles = nParticles;
	ParticleData = new iexParticleData[nParticles];
	//	全リセット
	Reset();
	//	テクスチャ設定
	lpTexture = new iex2DObj(filename);
}

//------------------------------------------------------
//		解放
//------------------------------------------------------
void iexParticle::Release()
{
	delete[] ParticleData;
	delete lpTexture;
}

//*****************************************************************************
//		データ設定
//*****************************************************************************
//------------------------------------------------------
//		構造体指定
//------------------------------------------------------
void iexParticle::Set( LPPARTICLE pd )
{
	for( int i=0 ; i<nParticles ; i++ ){
		if( ParticleData[i].bActive() ) continue;
		ParticleData[i].Set(pd);
		break;
	}
}

//------------------------------------------------------
//		データ個別指定
//------------------------------------------------------
void iexParticle::Set( int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor, 
						LPVECTOR3 Pos, LPVECTOR3 Move, LPVECTOR3 Power, float rotate, float stretch, float scale, u8 flag )
{
	PARTICLE pd;

	pd.type = type;
	pd.aFrame = aFrame;
	pd.aColor = aColor;
	pd.eFrame = eFrame;
	pd.eColor = eColor;
	pd.mFrame = mFrame;
	pd.mColor = mColor;

	pd.Pos   = *Pos;
	pd.Move  = *Move;
	pd.Power = *Power;
	pd.rotate = rotate;
	pd.stretch = stretch;

	pd.scale = scale;
	pd.angle = .0f;

	pd.flag = flag;

	Set(&pd);
}

//------------------------------------------------------
//		データ個別指定
//------------------------------------------------------
void	iexParticle::Set( int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha, 
						LPVECTOR3 Pos, LPVECTOR3 Move, LPVECTOR3 Power,
						float r, float g, float b, float scale, u8 flag )
{
	COLOR	color;
	DWORD	aa, ea, ma;

	aa = ((DWORD)(aAlpha*255.0f) << 24);
	ea = ((DWORD)(eAlpha*255.0f) << 24);
	ma = ((DWORD)(mAlpha*255.0f) << 24);
	color = ((DWORD)(r*255.0f) << 16) | ((DWORD)(g*255.0f) << 8) | (DWORD)(b*255.0f);
	Set( type, aFrame, aa|color, eFrame, ea|color, mFrame, ma|color, Pos, Move, Power, .0f, 1.0f, scale, flag );
}

//*****************************************************************************
//		更新
//*****************************************************************************
void iexParticle::Update()
{
	for( int i=0 ; i<nParticles ; i++ ){
		if(	ParticleData[i].bActive() == FALSE ) continue;
		ParticleData[i].Update();
	}
}

//*****************************************************************************
//		リセット
//*****************************************************************************
void iexParticle::Reset()
{
	for( int i=0 ; i<nParticles ; i++ ){
		if(	ParticleData[i].bActive() == FALSE ) continue;
		ParticleData[i].SetFlag(0);
	}
}

//*****************************************************************************
//		描画
//*****************************************************************************
//------------------------------------------------------
//		固定機能描画
//------------------------------------------------------
void iexParticle::Render()
{
	int		i;
	float	scale = -1.0f;

	CurViewProjection = matView * matProjection;

	iexRenderState::Set( RS_ADD, NULL, lpTexture->GetTexture() );
	iexSystem::Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	iexSystem::Device->SetFVF(D3DFVF_TLVERTEX);
	//	アルファブレンド設定
	iexSystem::Device->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	iexSystem::Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	iexSystem::Device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	for( i=0 ; i<nParticles ; i++ ){
		//	パーティクルレンダリング
		if( ParticleData[i].bActive() ) ParticleData[i].Render();
	}

	iexSystem::Device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	iexSystem::Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
}

//------------------------------------------------------
//		シェーダー描画
//------------------------------------------------------
void iexParticle::Render( iexShader* shader, char* name )
{
	int		i;
	float	scale = -1.0f;

	CurViewProjection = matView * matProjection;

	shader->SetTexture( lpTexture->GetTexture() );
	iexRenderState::Set( RS_ADD, NULL, lpTexture->GetTexture() );

	iexSystem::Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	iexSystem::Device->SetFVF(D3DFVF_TLVERTEX);
	//	アルファブレンド設定
	iexSystem::Device->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	iexSystem::Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	iexSystem::Device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	for( i=0 ; i<nParticles ; i++ ){
		//	パーティクルレンダリング
		if( ParticleData[i].bActive() ) ParticleData[i].Render( shader, name );
	}

	iexSystem::Device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	iexSystem::Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
}






//**************************************************************************************************
//
//		過去のIEXとの互換用
//
//**************************************************************************************************

void	IEX_InitParticle( LPSTR filename ){ iexParticle::Initialize( filename, 4096 ); }
void	IEX_ReleaseParticle(){ iexParticle::Release(); }
void	IEX_ExecuteParticles(){ iexParticle::Update(); }
void	IEX_DrawParticles(){ iexParticle::Render(); }


void	IEX_SetParticle( int type, int aFrame, COLOR aColor, int eFrame, COLOR eColor, int mFrame, COLOR mColor, 
						LPVECTOR3 Pos, LPVECTOR3 Move, LPVECTOR3 Power, float rotate, float stretch, float scale, u8 flag )
{
	iexParticle::Set( type, aFrame, aColor, eFrame, eColor, mFrame, mColor, Pos, Move, Power, rotate, stretch, scale, flag );
}

void	IEX_SetParticle( LPPARTICLE p )
{
	iexParticle::Set( p->type, p->aFrame, p->aColor, p->eFrame, p->eColor, p->mFrame, p->mColor, 
						&p->Pos, &p->Move, &p->Power, p->rotate, p->stretch, p->scale, p->flag );
}

void	IEX_SetParticle( int type, int aFrame, float aAlpha, int eFrame, float eAlpha, int mFrame, float mAlpha, 
						LPVECTOR3 Pos, LPVECTOR3 Move, LPVECTOR3 Power,
						float r, float g, float b, float scale, u8 flag )
{
	iexParticle::Set( type, aFrame, aAlpha, eFrame, eAlpha, mFrame, mAlpha, Pos, Move, Power, r, g, b, scale, flag );
}





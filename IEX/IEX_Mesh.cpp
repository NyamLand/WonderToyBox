#include	"iExtreme.h"

//**************************************************************************************************
//
//
//
//**************************************************************************************************

//------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------
iexMesh::iexMesh( char* filename )
{
	lpMesh = NULL;
	lpTexture = NULL;
	lpNormal = NULL;
	lpSpecular = NULL;
	lpHeight = NULL;

	//	ファイル読み込み
	char*	ext = &filename[ lstrlen(filename)-4 ];
	if( lstrcmpi( ext, ".imo" ) == 0 ) LoadIMO(filename);
	 else LoadX(filename);

	if( lpMesh == NULL )
	{
		//	読み込み失敗
		iexSystem::printf( "*エラー[iexMesh] ---> ロード失敗: \"%s\"\n", filename );
		bLoad = FALSE;
	} else {
		DWORD* pAdjacency = new DWORD [ lpMesh->GetNumFaces() * 3 ];
		// 最適化
		lpMesh->GenerateAdjacency( 1e-6f, pAdjacency );
		lpMesh->OptimizeInplace( D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, pAdjacency, NULL, NULL, NULL );
		delete[] pAdjacency;

		bLoad = TRUE;
	}
	SetPos(0,0,0);
	SetScale(1.0f);
	SetAngle(0);
	Update();
}

//------------------------------------------------------
//	デストラクタ
//------------------------------------------------------
iexMesh::~iexMesh()
{
	if( bLoad )
	{
		if( lpMesh ) lpMesh->Release();

		//	テクスチャ解放
		for( DWORD i=0 ; i<MaterialCount ; i++ ){
			if( lpTexture[i] != NULL ) iexTexture::Release(lpTexture[i]);
			if( lpNormal[i] != NULL ) iexTexture::Release(lpNormal[i]);
			if( lpSpecular[i] != NULL ) iexTexture::Release(lpSpecular[i]);
			if( lpHeight[i] != NULL ) iexTexture::Release(lpHeight[i]);
		}
		delete[] lpMaterial;
		delete[] lpTexture;
		delete[] lpNormal;
		delete[] lpSpecular;
		delete[] lpHeight;
	}
}

//------------------------------------------------------
//	クローンの作成
//------------------------------------------------------
iexMesh*	iexMesh::Clone()
{
	iexMesh* obj = new iexMesh(*this);
	obj->bLoad = FALSE;
	return obj;
}

//*****************************************************************************
//	情報設定
//*****************************************************************************
//------------------------------------------------------
//	位置設定
//------------------------------------------------------
void iexMesh::SetPos( float x, float y, float z )
{
	Pos.x = x;
	Pos.y = y;
	Pos.z = z;
	bChanged = TRUE;
}

void iexMesh::SetPos( Vector3& pos )
{
	Pos = pos;
	bChanged = TRUE;
}

//------------------------------------------------------
//	回転設定
//------------------------------------------------------
void iexMesh::SetAngle( float x, float y, float z )
{
	Angle.x = x;
	Angle.y = y;
	Angle.z = z;
	bChanged = TRUE;
}

void iexMesh::SetAngle( float angle )
{
	Angle.x = .0f;
	Angle.y = angle;
	Angle.z = .0f;
	bChanged = TRUE;
}

void iexMesh::SetAngle( Vector3& angle )
{
	Angle = angle;
	bChanged = TRUE;
}

//------------------------------------------------------
//	スケール設定
//------------------------------------------------------
void iexMesh::SetScale( float x, float y, float z )
{
	Scale.x = x;
	Scale.y = y;
	Scale.z = z;
	bChanged = TRUE;
}

void iexMesh::SetScale( float scale )
{
	Scale.x = scale;
	Scale.y = scale;
	Scale.z = scale;
	bChanged = TRUE;
}
void iexMesh::SetScale( Vector3& scale )
{
	Scale = scale;
	bChanged = TRUE;
}

//**************************************************************************************************
//
//		更新処理
//
//**************************************************************************************************
void	iexMesh::Update()
{
	Matrix MatScale;

	if( !lpMesh ) return;
	//	スケーリング
	D3DXMatrixScaling( &MatScale, Scale.x, Scale.y, Scale.z ); 
	//	座標系設定 
	SetTransformMatrixXYZ( &TransMatrix, Pos.x, Pos.y, Pos.z, Angle.x, Angle.y, Angle.z );
	//	転送行列作成 
	TransMatrix = MatScale * TransMatrix;
}

//**************************************************************************************************
//
//		描画
//
//**************************************************************************************************

//------------------------------------------------------
//	通常描画
//------------------------------------------------------
void iexMesh::Render(){ Render( RS_COPY, -1.0f ); }

//------------------------------------------------------
//	フラグ指定描画
//------------------------------------------------------
void iexMesh::Render( u32 dwFlags, float param )
{
	if( !lpMesh ) return;

    //	転送行列設定
	iexSystem::Device->SetTransform( D3DTS_WORLD, &TransMatrix );
	//	メッシュのレンダリング
	for( u32 i=0 ; i<MaterialCount ; i++ ){
		//	透明度設定
		if( param != -1.0f ) lpMaterial[i].Diffuse.a = lpMaterial[i].Ambient.a = param;
		//	材質設定
		iexRenderState::Set( dwFlags, &lpMaterial[i], lpTexture[i] );
		//	材質グループ描画
		lpMesh->DrawSubset( i );
	}
}

//------------------------------------------------------
//	シェーダー描画
//------------------------------------------------------
void iexMesh::Render( iexShader* shader, char* name )
{
	//	シェーダーの適用
	u32 pass = shader->Begine(name);

	// ローカル-射影変換行列
	Matrix m = TransMatrix * matView * matProjection;
	shader->SetValue("TransMatrix", TransMatrix );
	shader->SetMatrix( &m );

	for( u32 p=0 ; p<pass ; p++ )
	{
		//	パスのレンダリング
		shader->BeginePass(p);
		for( u32 i=0 ; i<MaterialCount ; i++ )
		{
			//	テクスチャ指定
			shader->SetTexture( lpTexture[i] );
			shader->SetValue( "NormalMap", lpNormal[i] );
			shader->SetValue( "SpecularMap", lpSpecular[i] );
			shader->SetValue( "HeightMap", lpHeight[i] );
			shader->CommitChanges();
			//	材質グループ描画
			lpMesh->DrawSubset( i );
		}
		shader->EndPass();
	}

	shader->End();
}

//**************************************************************************************************
//
//		レイピック
//
//**************************************************************************************************
//------------------------------------------------------
//		レイピック
//------------------------------------------------------
int	iexMesh::RayPick( Vector3* out, Vector3* pos, Vector3* vec, float *Dist )
{
	int		ret = -1;

	if( vec->x == .0f && vec->z == .0f ) return RayPickUD( out, pos, vec, Dist );

	Vector3 p = *pos;
	Vector3 vv = *vec;

	float neart = *Dist;
	float dist = *Dist;
	dist = dist*dist;
	*out = p;
	//	情報取得	
	u32 fvf = lpMesh->GetFVF();
	//	頂点サイズ計算
	int VertexSize = D3DXGetFVFVertexSize(fvf) / sizeof(float);

	//	バッファロック
	float	*pVertices;
	u16		*pIndices;
	u32 NumIndices = lpMesh->GetNumFaces();
	lpMesh->LockVertexBuffer( D3DLOCK_READONLY, (void**)&pVertices );
	lpMesh->LockIndexBuffer( D3DLOCK_READONLY, (void**)&pIndices );

	Vector3 v1, v2, v3;
	Vector3	n;
	Vector3	l1, l2, l3;
	Vector3	temp;
	Vector3	cp;

	Vector3 p1, p2, p3;

	for( u32 j=0 ; j<NumIndices ; j++ )
	{
		//	面頂点取得
		int a = pIndices[j*3+0] * VertexSize;
		v1.x = pVertices[a];	v1.y = pVertices[a+1];	v1.z = pVertices[a+2];

		int b = pIndices[j*3+1] * VertexSize;
		v2.x = pVertices[b];	v2.y = pVertices[b+1];	v2.z = pVertices[b+2];

		int c = pIndices[j*3+2] * VertexSize;
		v3.x = pVertices[c];	v3.y = pVertices[c+1];	v3.z = pVertices[c+2];
		
		//	距離判定
		//Vector3	ss = (v1 + v2 + v3) / 3.0f - p;
		//if( ss.LengthSq() > dist ) continue;
		l1.x = v2.x - v1.x;
		l1.y = v2.y - v1.y;
		l1.z = v2.z - v1.z;
		l2.x = v3.x - v2.x;
		l2.y = v3.y - v2.y;
		l2.z = v3.z - v2.z;
		
		//	外積による法線算出		
		Vector3Cross( n, l1, l2 );
		//	内積の結果がプラスならば裏向き
		float dot = Vector3Dot( vv, n );
		if( dot >= 0 ) continue;
		//	交点算出
		p1.x = v1.x - p.x;
		p1.y = v1.y - p.y;
		p1.z = v1.z - p.z;
		float t = Vector3Dot( n, p1 ) / dot;
		if( t < .0f || t > neart ) continue;

		cp.x = vv.x*t + p.x;
		cp.y = vv.y*t + p.y;
		cp.z = vv.z*t + p.z;
		//	内点判定
		p1.x = v1.x - cp.x;
		p1.y = v1.y - cp.y;
		p1.z = v1.z - cp.z;
		
		Vector3Cross( temp, p1, l1 );
		if( Vector3Dot(temp, n) < .0f ) continue;

		p2.x = v2.x - cp.x;
		p2.y = v2.y - cp.y;
		p2.z = v2.z - cp.z;
		Vector3Cross( temp, p2, l2 );
		if( Vector3Dot(temp, n) < .0f ) continue;

		l3.x = v1.x - v3.x;
		l3.y = v1.y - v3.y;
		l3.z = v1.z - v3.z;
		p3.x = v3.x - cp.x;
		p3.y = v3.y - cp.y;
		p3.z = v3.z - cp.z;
		Vector3Cross( temp, p3, l3 );
		if( Vector3Dot(temp, n) < .0f ) continue;

		*out = cp;
		*vec = n;
		ret = j;
		neart = t;
	}
	lpMesh->UnlockVertexBuffer();
	lpMesh->UnlockIndexBuffer();
	*Dist = neart;

	return	ret;
}

//------------------------------------------------------
//		上下最適化
//------------------------------------------------------
int	iexMesh::RayPickUD( Vector3* out, Vector3* pos, Vector3* vec, float *Dist )
{
	float	t, neart;
	float	vy;
	int		ret = -1;

	int		VertexSize;

	Vector3	p = *pos;
	vy = vec->y;

	neart = *Dist;

	//	情報取得	
	int fvf = lpMesh->GetFVF();
	//	頂点サイズ計算
	VertexSize = D3DXGetFVFVertexSize(fvf) / sizeof(float);

	//	バッファロック
	float	*pVertices;
	u16		*pIndices;
	int NumIndices = lpMesh->GetNumFaces();
	lpMesh->LockVertexBuffer( D3DLOCK_READONLY , (void**)&pVertices );
	lpMesh->LockIndexBuffer( D3DLOCK_READONLY , (void**)&pIndices );

	Vector	l1, l2, l3;
	Vector	p1, p2, p3;
	Vector v[3];
	Vector n;

	for( int j=0 ; j<NumIndices ; j++ )
	{
		//	面頂点取得
		int a = pIndices[j*3+0] * VertexSize;
		int b = pIndices[j*3+1] * VertexSize;
		int c = pIndices[j*3+2] * VertexSize;

		v[0].x = pVertices[a];	v[1].x = pVertices[b];	v[2].x = pVertices[c];
		if( v[0].x > p.x && v[1].x > p.x && v[2].x > p.x ) continue;

		v[0].z = pVertices[a+2];	v[1].z = pVertices[b+2];	v[2].z = pVertices[c+2];
		if( v[0].z > p.z && v[1].z > p.z && v[2].z > p.z ) continue;

		v[0].y = pVertices[a+1];	v[1].y = pVertices[b+1];	v[2].y = pVertices[c+1];

		//	内点判定（全外積がマイナス）		
		l1.x = v[1].x - v[0].x;
		l1.z = v[1].z - v[0].z;
		p1.x = v[0].x - p.x;
		p1.z = v[0].z - p.z;
		if( (p1.x*l1.z - p1.z*l1.x)*vy < 0 ) continue;

		l2.x = v[2].x - v[1].x;
		l2.z = v[2].z - v[1].z;
		p2.x = v[1].x - p.x;
		p2.z = v[1].z - p.z;
		if( (p2.x*l2.z - p2.z*l2.x)*vy < 0 ) continue;

		l3.x = v[0].x - v[2].x;
		l3.z = v[0].z - v[2].z;
		p3.x = v[2].x - p.x;
		p3.z = v[2].z - p.z;
		if( (p3.x*l3.z - p3.z*l3.x)*vy < 0 ) continue;

		//	外積による法線算出		
		l1.y = v[1].y - v[0].y;
		l2.y = v[2].y - v[1].y;
		Vector3Cross( n, l1, l2 );
		//	表裏判定
		if( vy*n.y >= 0 ) continue;

		//	交点算出
		p1.y = v[0].y - p.y;
		t = Vector3Dot( n, p1 ) / (n.y*vy);
		if( t < .0f || t > neart ) continue;

		*vec = n;
		ret = j;
		neart = t;
	}
	lpMesh->UnlockVertexBuffer();
	lpMesh->UnlockIndexBuffer();

	out->y = neart*vy + p.y;
	out->x = pos->x;
	out->z = pos->z;
	*Dist = neart;

	return	ret;
}

//**************************************************************************************************
//
//	ファイル読み込み
//
//**************************************************************************************************

//	IMOオブジェクト
typedef struct tagIMOOBJ {
	DWORD		id;	
	int			type;
	DWORD		NumVertex;
	LPLVERTEX	lpVertex;

	DWORD		NumFace;
	LPWORD		lpFace;
	LPDWORD		lpAtr;

	DWORD			NumMaterial;	//	マテリアル数
	D3DMATERIAL9	Material[32];	//	マテリアル
	char			Texture[32][32];//	テクスチャファイル

} IMOOBJ, *LPIMOOBJ;

//------------------------------------------------------
//	ＩＭＯ読み込み
//------------------------------------------------------
BOOL iexMesh::LoadIMO( LPSTR filename )
{
	IMOOBJ		imo;
	DWORD		i;
	//	ファイル読み込み
	HANDLE	hfile;
	DWORD	dum;

	int		version = 1;

	hfile = CreateFile( filename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL );
	if( hfile == INVALID_HANDLE_VALUE ) return FALSE;

	ReadFile( hfile, &imo, sizeof(IMOOBJ), &dum, NULL );
	if( imo.id == '2OMI' ) version = 2;

	//	メッシュ作成
	BYTE	*pVertex, *pFace;
	DWORD	*pData;

	LVERTEX*	workV = new LVERTEX[imo.NumVertex];
	MESHVERTEX2*	workV2 = new MESHVERTEX2[imo.NumVertex];
	WORD*		workF = new WORD[imo.NumFace*3];

	if( version == 1 )
	{
		//	頂点読み込み
		ReadFile( hfile, workV, sizeof(LVERTEX)*imo.NumVertex, &dum, NULL );
		//	インデックス読み込み
		ReadFile( hfile, workF, sizeof(WORD)*imo.NumFace*3, &dum, NULL );
		
		for (DWORD i = 0; i < imo.NumVertex; i++)
		{
			workV2[i].x = workV[i].x;
			workV2[i].y = workV[i].y;
			workV2[i].z = workV[i].z;
			workV2[i].tu = workV[i].tu;
			workV2[i].tv = workV[i].tv;
			workV2[i].color = workV[i].color;
			workV2[i].nx = .0f;
			workV2[i].ny = .0f;
			workV2[i].nz = .0f;
		}

		for (DWORD i = 0; i < imo.NumFace; i++)
		{
			//	法線計算
			Vector3 v1, v2, n;
			v1.x = workV[workF[i * 3 + 0]].x - workV[workF[i * 3 + 1]].x;
			v1.y = workV[workF[i * 3 + 0]].y - workV[workF[i * 3 + 1]].y;
			v1.z = workV[workF[i * 3 + 0]].z - workV[workF[i * 3 + 1]].z;
			
			v2.x = workV[workF[i * 3 + 1]].x - workV[workF[i * 3 + 2]].x;
			v2.y = workV[workF[i * 3 + 1]].y - workV[workF[i * 3 + 2]].y;
			v2.z = workV[workF[i * 3 + 1]].z - workV[workF[i * 3 + 2]].z;

			n.x = (v1.y*v2.z - v1.z*v2.y);
			n.y = (v1.z*v2.x - v1.x*v2.z);
			n.z = (v1.x*v2.y - v1.y*v2.x);
			float d = sqrtf( n.x*n.x + n.y*n.y + n.z*n.z );
			n.x /= d;
			n.y /= d;
			n.z /= d;

			workV2[workF[i * 3 + 0]].nx += n.x;
			workV2[workF[i * 3 + 0]].ny += n.y;
			workV2[workF[i * 3 + 0]].nz += n.z;
			workV2[workF[i * 3 + 1]].nx += n.x;
			workV2[workF[i * 3 + 1]].ny += n.y;
			workV2[workF[i * 3 + 1]].nz += n.z;
			workV2[workF[i * 3 + 2]].nx += n.x;
			workV2[workF[i * 3 + 2]].ny += n.y;
			workV2[workF[i * 3 + 2]].nz += n.z;
		}
		for (DWORD i = 0; i < imo.NumVertex; i++)
		{
			float d = sqrtf( workV2[i].nx*workV2[i].nx + workV2[i].ny*workV2[i].ny + workV2[i].nz*workV2[i].nz );
			workV2[i].nx /= d;
			workV2[i].ny /= d;
			workV2[i].nz /= d;
		}
	} else {
		//	頂点読み込み
		ReadFile( hfile, workV2, sizeof(MESHVERTEX2)*imo.NumVertex, &dum, NULL );
		//	インデックス読み込み
		ReadFile( hfile, workF, sizeof(WORD)*imo.NumFace*3, &dum, NULL );
	}

	DWORD	Declaration = D3DFVF_MESHVERTEX2;
	D3DXCreateMeshFVF( imo.NumFace, imo.NumVertex, D3DXMESH_MANAGED, Declaration, iexSystem::GetDevice(), &lpMesh );
	//	頂点設定
	lpMesh->LockVertexBuffer( 0, (void**)&pVertex );
	memcpy( pVertex, workV2, sizeof(MESHVERTEX2)*imo.NumVertex );
	lpMesh->UnlockVertexBuffer();

	//	面設定
	lpMesh->LockIndexBuffer( 0, (void**)&pFace );
	memcpy( pFace, workF, sizeof(WORD)*imo.NumFace*3 );
	lpMesh->UnlockIndexBuffer();

	//	属性設定
	lpMesh->LockAttributeBuffer( 0, &pData );
	ReadFile( hfile, pData, sizeof(DWORD)*imo.NumFace, &dum, NULL );
	lpMesh->UnlockAttributeBuffer();

	//	パス分割
	char	workpath[MAX_PATH];
	CopyMemory( workpath, filename, lstrlen(filename)+1 );
	for( i=lstrlen(filename) ; i>0 ; i-- ){
		if( IsDBCSLeadByte(workpath[i-2]) ){
			i--;
			continue;
		}
		if( workpath[i-1] == '\\' || workpath[i-1] == '/' ){
			workpath[i] = '\0';
			break;
		}
	}

	//	マテリアル設定
	MaterialCount = imo.NumMaterial;
	lpMaterial = new D3DMATERIAL9[ MaterialCount ];
	CopyMemory( lpMaterial, imo.Material, sizeof(D3DMATERIAL9)*imo.NumMaterial );
	//	テクスチャ設定
	lpTexture  = new Texture2D* [ imo.NumMaterial ];
	lpNormal   = new Texture2D* [ imo.NumMaterial ];
	lpSpecular = new Texture2D* [ imo.NumMaterial ];
	lpHeight   = new Texture2D* [ imo.NumMaterial ];

	char temp[256];
	for( i=0 ; i<MaterialCount ; i++ ){
		lpTexture[i]  = NULL;
		lpNormal[i]   = NULL;
		lpSpecular[i] = NULL;
		lpHeight[i]   = NULL;

		if( imo.Texture[i][0] == '\0' ) continue;
		//	テクスチャ読み込み
		sprintf( temp, "%s%s", workpath, imo.Texture[i] );
		lpTexture[i] = iexTexture::Load( temp );

		sprintf( temp, "%sN%s", workpath, imo.Texture[i] );
		lpNormal[i] = iexTexture::Load( temp );

		sprintf( temp, "%sS%s", workpath, imo.Texture[i] );
		lpSpecular[i] = iexTexture::Load( temp );

		sprintf( temp, "%sH%s", workpath, imo.Texture[i] );
		lpHeight[i] = iexTexture::Load( temp, 1 );
	}
	CloseHandle(hfile);

	delete[] workV;
	delete[] workV2;
	delete[] workF;

	//	初期設定
	SetPos(0,0,0);
	SetAngle(0.0f);
	SetScale(1.0f);
	dwFlags = 0;

	return TRUE;
}

//------------------------------------------------------
//	Ｘファイル読み込み
//------------------------------------------------------
BOOL	iexMesh::LoadX( LPSTR filename )
{
	HRESULT hr;
	LPD3DXBUFFER	lpD3DXMtrlBuffer;

	//	パス分割
	char	workpath[MAX_PATH];
	CopyMemory( workpath, filename, lstrlen(filename)+1 );
	for( int i=lstrlen(filename) ; i>0 ; i-- ){
		if( IsDBCSLeadByte(workpath[i-2]) ){
			i--;
			continue;
		}
		if( workpath[i-1] == '\\' || workpath[i-1] == '/' ){
			workpath[i] = '\0';
			break;
		}
	}

	// X ファイルのロード
	hr = D3DXLoadMeshFromX( filename, D3DXMESH_MANAGED, iexSystem::GetDevice(),NULL, &lpD3DXMtrlBuffer, NULL, &MaterialCount, &lpMesh );
 	if( FAILED( hr ) ) return FALSE;

	// lpD3DXMtrlBufferから、質感やテクスチャーの情報を取得する
	D3DXMATERIAL* d3dxMaterial = (D3DXMATERIAL *)lpD3DXMtrlBuffer->GetBufferPointer();

	// テクスチャ＆マテリアル用バッファ生成
	lpMaterial = new D3DMATERIAL9 [ MaterialCount ];
	lpTexture  = new LPDIRECT3DTEXTURE9 [ MaterialCount ];
	lpNormal   = new Texture2D* [ MaterialCount ];
	lpSpecular = new Texture2D* [ MaterialCount ];
	lpHeight   = new Texture2D* [ MaterialCount ];

	// データ読み込み
	for( u32 i=0 ; i<MaterialCount ; i++ ){
		// マテリアル
		lpMaterial[i] = d3dxMaterial[i].MatD3D;
		lpMaterial[i].Ambient.r = lpMaterial[i].Diffuse.r;
		lpMaterial[i].Ambient.g = lpMaterial[i].Diffuse.g;
		lpMaterial[i].Ambient.b = lpMaterial[i].Diffuse.b;
		// テクスチャ
		char temp[256];
		lpTexture[i]  = NULL;
		lpNormal[i]   = NULL;
		lpSpecular[i] = NULL;
		lpHeight[i]   = NULL;

		if( d3dxMaterial[i].pTextureFilename != NULL ){
			//	テクスチャ読み込み
			sprintf( temp, "%s%s", workpath, d3dxMaterial[i].pTextureFilename );
			lpTexture[i] = iexTexture::Load( temp );

			sprintf( temp, "%sN%s", workpath, d3dxMaterial[i].pTextureFilename );
			lpNormal[i] = iexTexture::Load( temp );

			sprintf( temp, "%sS%s", workpath, d3dxMaterial[i].pTextureFilename );
			lpSpecular[i] = iexTexture::Load( temp );

			sprintf( temp, "%sH%s", workpath, d3dxMaterial[i].pTextureFilename );
			lpHeight[i] = iexTexture::Load( temp, 1 );
		}
	}
	lpD3DXMtrlBuffer->Release();

	//	初期設定
	SetPos(0,0,0);
	SetAngle(0.0f);
	SetScale(1.0f);
	dwFlags = 0;

	return TRUE;
}








//*****************************************************************************
//
//	過去のＩＥＸとの互換用
//
//*****************************************************************************
//------------------------------------------------------
//		情報設定
//------------------------------------------------------
void	IEX_SetMeshPos( iexMesh* lpMesh, float x, float y, float z )
{
	if( !lpMesh ) return;
	lpMesh->SetPos( x, y, z );
	lpMesh->Update();
}

void	IEX_SetMeshAngle( iexMesh* lpMesh, float x, float y, float z )
{
	if( !lpMesh ) return;
	lpMesh->SetAngle( x, y, z );
	lpMesh->Update();
}

void	IEX_SetMeshScale( iexMesh* lpMesh, float x, float y, float z )
{
	if( !lpMesh ) return;
	lpMesh->SetScale( x, y, z );
	lpMesh->Update();
}

void	IEX_SetMeshScale( iexMesh* lpMesh, float scale )
{
	if( !lpMesh ) return;
	lpMesh->SetScale( scale );
	lpMesh->Update();
}

//*****************************************************************************
//
//		メッシュのレンダリング
//
//*****************************************************************************

void	IEX_RenderMesh( iexMesh* lpMesh, u32 dwFlags, float param )
{
	if( !lpMesh ) return;
	lpMesh->Render(dwFlags, param);
}


void	IEX_RenderMesh( iexMesh* lpMesh, iexShader* shader, char* name )
{
	if( !lpMesh ) return;
	lpMesh->Render( shader, name);
}

void	IEX_RenderMesh( iexMesh* lpMesh )
{
	IEX_RenderMesh( lpMesh, RS_COPY, -1.0f );
}

//*****************************************************************************
//
//
//
//*****************************************************************************

//------------------------------------------------------
//		Ｘファイルメッシュ作成
//------------------------------------------------------

iexMesh*	IEX_LoadMeshFromX( LPSTR filename )
{
	iexMesh*	mesh = new iexMesh(filename);
	return mesh;
}

//------------------------------------------------------
//		iMoファイル
//------------------------------------------------------
iexMesh*	IEX_LoadIMO( LPSTR filename )
{
	iexMesh*	mesh = new iexMesh(filename);
	return mesh;
}

//*****************************************************************************
//
//
//
//*****************************************************************************

//
//		メッシュの解放
//

void	IEX_ReleaseMesh( iexMesh* lpMesh )
{
	if( !lpMesh ) return;
	delete	lpMesh;
}

//*****************************************************************************
//
//		RayPick
//
//*****************************************************************************
 
//
//		レイピック（真上・真下）
//

int	IEX_RayPickMeshUD( iexMesh* lpMesh, Vector3* out, Vector3* pos, Vector3* vec, float *Dist )
{
	int ret = lpMesh->RayPickUD( out, pos, vec, Dist );
	return	ret;
}

//
//		レイピック（通常）
//

int	IEX_RayPickMesh( iexMesh* lpMesh, Vector3* out, Vector3* pos, Vector3* vec, float *Dist )
{
	int ret = lpMesh->RayPick( out, pos, vec, Dist );
	return	ret;
}


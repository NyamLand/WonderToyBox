#include	"iExtreme.h"

//*****************************************************************************
//
//		３Ｄオブジェクト
//
//*****************************************************************************

//*****************************************************************************
//		初期化・解放
//*****************************************************************************
//------------------------------------------------------
//	コンストラクタ
//------------------------------------------------------
iex3DObj::iex3DObj( char* filename )
{
	if( LoadObject(filename) == FALSE ) return;
	for( int i=0 ; i<16 ; i++ ) Param[i] = 0;

	//	変数初期化
	motionSpeed = 1.0f;

	bLoad = TRUE;
}

//------------------------------------------------------
//	デストラクタ
//------------------------------------------------------
iex3DObj::~iex3DObj()
{
	if( bLoad )
	{
		if( lpSkinInfo ) lpSkinInfo->Release();
		delete[] BoneParent;
		delete[] lpBoneMatrix;
		delete[] lpOffsetMatrix;
		delete[] lpMatrix;

		delete[] orgPose;
		delete[] orgPos;
		delete[] CurPose;
		delete[] CurPos;

		delete[] lpVertex;

		//	アニメーションデータ解放
		for( DWORD i=0 ; i<NumBone ; i++ ){
			if( lpAnime[i].rotNum > 0 )
			{
				delete[] lpAnime[i].rotFrame;
				delete[] lpAnime[i].rot;
			}
			if( lpAnime[i].posNum > 0 )
			{
				delete[] lpAnime[i].posFrame;
				delete[] lpAnime[i].pos;
			}
		}
		delete[] dwFrameFlag;
		delete[] lpAnime;
	}
}

//------------------------------------------------------
//	クローンの作成
//------------------------------------------------------
iex3DObj*	iex3DObj::Clone()
{
	iex3DObj*	obj = new iex3DObj(*this);
	obj->SetLoadFlag(FALSE);
	return obj;
}

//*****************************************************************************
//	情報設定
//*****************************************************************************
//------------------------------------------------------
//	モーション設定
//------------------------------------------------------
void iex3DObj::SetMotion( int motion )
{
	int		param;

	if( M_Offset[motion] == 65535 ) return;
	Motion  = motion;
	dwFrame = M_Offset[motion];
	bChanged = TRUE;

	param = dwFrameFlag[( int )dwFrame];
	if( (param!=0xFFFF) && (param&0x4000) ) Param[(param&0x0F00)>>8] = (u8)(param&0x00FF);
}

//*****************************************************************************
//		更新処理
//*****************************************************************************
void iex3DObj::Update()
{
	/*	スキンメッシュ更新	*/ 
	UpdateSkinMeshFrame( (float)dwFrame );
	UpdateBoneMatrix();
	UpdateSkinMesh();

	iexMesh::Update();
	RenderFrame = ( unsigned long )dwFrame;
	bChanged = FALSE;
}

//------------------------------------------------------
//	モーション
//------------------------------------------------------
void iex3DObj::Animation()
{
	int		param;
	u32	work;

	work = ( unsigned long )dwFrame;
	param = dwFrameFlag[(int)dwFrame];
	if (param & 0x4000) param = 0xFFFF;
	if (param != 0xFFFF){
		//	アニメーションジャンプ
		if (param & 0x8000){
			SetMotion(param & 0xFF);
		}
		else dwFrame = ( float )param;
	}
	else {
		dwFrame += motionSpeed;
		if (dwFrame >= NumFrame) dwFrame = 0;
	}

	if (dwFrame != work) bChanged = TRUE;

	param = dwFrameFlag[(int)dwFrame];
	if ((param != 0xFFFF) && (param & 0x4000)) Param[(param & 0x0F00) >> 8] = (u8)(param & 0x00FF);

}

//**************************************************************************************************
//		描画
//**************************************************************************************************

//------------------------------------------------------
//		固定機能通常描画
//------------------------------------------------------
void iex3DObj::Render()
{
	//	情報更新
	if( RenderFrame != dwFrame ) Update();
	//	メイン行列設定
	iexMesh::Render();
}

//------------------------------------------------------
//		固定機能フラグ指定
//------------------------------------------------------
void iex3DObj::Render( DWORD flag, float alpha )
{
	//	情報更新
	if( RenderFrame != dwFrame ) Update();
	//	メイン行列設定
	iexMesh::Render( flag, alpha );
}

//------------------------------------------------------
//		シェーダー描画
//------------------------------------------------------
void iex3DObj::Render( iexShader* shader, char* name )
{
	//	情報更新
	if( RenderFrame != dwFrame ) Update();
	//	メイン行列設定
	iexMesh::Render( shader, name );
}

//*****************************************************************************
//
//		オブジェクト作成
//
//*****************************************************************************

typedef struct tagIEMBONE1 {
	Matrix			BoneMatrix;			//	ボーン行列
	u16				parent;				//	親ボーン

	Quaternion		orgPose;			//	基本姿勢
	Vector3			orgPos;				//	基本座標

	u16				IndexNum;			//	影響頂点数
	u32				Index[1000];		//	影響頂点Index
	float			Influence[1000];	//	影響力
} IEMBONE1, *LPIEMBONE1;

typedef struct tagIEMMOTION1 {
	u16				NumRotate;			//	回転キーフレーム数
	u16				RotateFrame[512];	//	回転キーフレーム
	Quaternion		Rotate[512];		//	ボーンの状態クォータニオン

	u16				NumPosition;		//	座標キーフレーム数
	u16				PositionFrame[512];	//	座標キーフレーム
	Vector3			Position[512];		//	座標
} IEMMOTION1, *LPIEMMOTION1;

typedef struct tagIEMBONE {
	Matrix			BoneMatrix;			//	ボーン行列
	u16				parent;				//	親ボーン

	Quaternion		orgPose;			//	基本姿勢
	Vector3			orgPos;				//	基本座標

	u16				IndexNum;			//	影響頂点数
	u32*			Index;				//	影響頂点Index
	float*			Influence;			//	影響力
} IEMBONE, *LPIEMBONE;

typedef struct tagIEMMOTION {
	u16				NumRotate;			//	回転キーフレーム数
	u16*			RotateFrame;		//	回転キーフレーム
	Quaternion*		Rotate;				//	ボーンの状態クォータニオン

	u16				NumPosition;		//	座標キーフレーム数
	u16*			PositionFrame;		//	座標キーフレーム
	LPVECTOR3		Position;			//	座標
} IEMMOTION, *LPIEMMOTION;

struct tagIEMFILE
{
	int				version;			//	バージョン	
	//	メッシュ情報
	u16				NumVertex;			//	頂点数
	void*			lpVertex;			//	頂点バッファ

	u16				NumFace;			//	ポリゴン数
	u16*			lpFace;				//	ポリゴンインデックス
	u32*			lpAtr;				//	ポリゴン材質

	u16				NumMaterial;		//	マテリアル数
	D3DMATERIAL9	Material[32];		//	マテリアル
	char			Texture[32][64];	//	テクスチャファイル

	//	ボーン情報
	u16				NumBone;
	LPIEMBONE		lpBone;

	//	モーション情報
	u16				MaxFrame;
	u16				NumMotion;
	u16				M_Offset[256];
	u16				FrameFlag[65535];

	LPIEMMOTION		lpMotion;
};


//------------------------------------------------------
//		スキン情報作成
//------------------------------------------------------
LPD3DXSKININFO	iex3DObj::CreateSkinInfo( LPIEMFILE lpIem )
{
	int				i;
	LPD3DXSKININFO	lpInfo;
	u32				Declaration = D3DFVF_MESHVERTEX;
	if( lpIem->version >= 4 ) Declaration = D3DFVF_MESHVERTEX2;

	//	スキン情報作成
	D3DXCreateSkinInfoFVF( lpIem->NumVertex, Declaration, lpIem->NumBone, &lpInfo );
	//	ボーン設定
	for( i=0 ; i<lpIem->NumBone ; i++ ){
		lpInfo->SetBoneInfluence( i, lpIem->lpBone[i].IndexNum, lpIem->lpBone[i].Index, lpIem->lpBone[i].Influence );
	}
	return lpInfo;
}

//------------------------------------------------------
//		DirectXメッシュの作成
//------------------------------------------------------
LPD3DXMESH	iex3DObj::CreateMesh( LPIEMFILE lpIem )
{
	LPD3DXMESH	lpMesh;
	u8			*pVertex, *pFace;
	u32			*pData;
	
	if( lpIem->version < 4 )
	{
		u32	Declaration = D3DFVF_MESHVERTEX;
		//	メッシュ作成
		D3DXCreateMeshFVF( lpIem->NumFace, lpIem->NumVertex, D3DXMESH_MANAGED, Declaration, iexSystem::Device, &lpMesh );
		//	頂点設定
		lpMesh->LockVertexBuffer( 0, (void**)&pVertex );
		CopyMemory( pVertex, lpIem->lpVertex, sizeof(MESHVERTEX)*lpIem->NumVertex );
	} else {
		u32	Declaration = D3DFVF_MESHVERTEX2;
		//	メッシュ作成
		D3DXCreateMeshFVF( lpIem->NumFace, lpIem->NumVertex, D3DXMESH_MANAGED, Declaration, iexSystem::Device, &lpMesh );
		//	頂点設定
		lpMesh->LockVertexBuffer( 0, (void**)&pVertex );
		CopyMemory( pVertex, lpIem->lpVertex, sizeof(MESHVERTEX2)*lpIem->NumVertex );
	}

	lpMesh->UnlockVertexBuffer();


	//	面設定
	lpMesh->LockIndexBuffer( 0, (void**)&pFace );
	CopyMemory( pFace, lpIem->lpFace, sizeof(u16)*lpIem->NumFace*3 );
	lpMesh->UnlockIndexBuffer();

	//	属性設定
	lpMesh->LockAttributeBuffer( 0, &pData );
	CopyMemory( pData, lpIem->lpAtr, sizeof(u32)*lpIem->NumFace );
	lpMesh->UnlockAttributeBuffer();

	return lpMesh;
}

//------------------------------------------------------
//		ＩＥＭから３Dオブジェクト作成
//------------------------------------------------------
BOOL iex3DObj::CreateFromIEM( char* path, LPIEMFILE lpIem )
{
	u32		i, j;

	dwFrame = 0;

	//	メッシュ作成
	lpSkinInfo = CreateSkinInfo( lpIem );
	lpMesh    = CreateMesh( lpIem );
	//	頂点情報コピー
	NumVertex = lpIem->NumVertex;

	if( lpIem->version < 4 )
	{
		lpVertex  = new MESHVERTEX[NumVertex];
		CopyMemory( lpVertex, lpIem->lpVertex, sizeof(MESHVERTEX)*NumVertex );
	} else {
		lpVertex  = new MESHVERTEX2[NumVertex];
		CopyMemory( lpVertex, lpIem->lpVertex, sizeof(MESHVERTEX2)*NumVertex );
	}

	//	マテリアル設定
	MaterialCount = lpIem->NumMaterial;
	lpMaterial = new D3DMATERIAL9[ MaterialCount ];
	CopyMemory( lpMaterial, lpIem->Material, sizeof(D3DMATERIAL9)*MaterialCount );
	//	テクスチャ設定
	lpTexture  = new Texture2D* [ MaterialCount ];
	lpNormal   = new Texture2D* [ MaterialCount ];
	lpSpecular = new Texture2D* [ MaterialCount ];
	lpHeight   = new Texture2D* [ MaterialCount ];
	ZeroMemory( lpTexture,  sizeof(Texture2D*)*MaterialCount );
	ZeroMemory( lpNormal,   sizeof(Texture2D*)*MaterialCount );
	ZeroMemory( lpSpecular, sizeof(Texture2D*)*MaterialCount );
	ZeroMemory( lpHeight,   sizeof(Texture2D*)*MaterialCount );

	for( i=0 ; i<MaterialCount ; i++ ){
		if( lpIem->Texture[i][0] == '\0' ) continue;
		//	テクスチャ読み込み
		char	temp[256];
		sprintf( temp, "%s%s", path, lpIem->Texture[i] );
		lpTexture[i] = iexTexture::Load( temp );

		sprintf( temp, "%sN%s", path, lpIem->Texture[i] );
		lpNormal[i] = iexTexture::Load( temp );

		sprintf( temp, "%sS%s", path, lpIem->Texture[i] );
		lpSpecular[i] = iexTexture::Load( temp );

		sprintf( temp, "%sH%s", path, lpIem->Texture[i] );
		lpHeight[i] = iexTexture::Load( temp );
	}

	//
	//	ボーン情報
	//
	NumBone = lpIem->NumBone;
	BoneParent     = new WORD[ NumBone ];
	lpBoneMatrix   = new Matrix[ NumBone ];
	lpOffsetMatrix = new Matrix[ NumBone ];
	lpMatrix       = new Matrix[ NumBone ];

	orgPose		= new Quaternion[ NumBone ];
	orgPos		= new Vector3[ NumBone ];
	CurPose		= new Quaternion[ NumBone ];
	CurPos		= new Vector3[ NumBone ];

	//
	NumFrame = lpIem->MaxFrame;
	CopyMemory( M_Offset, lpIem->M_Offset, 2*256 );
	dwFrameFlag = new u16[NumFrame];
	CopyMemory( dwFrameFlag, lpIem->FrameFlag, 2*NumFrame );

	//	アニメーション設定
	lpAnime = new IEXANIME2[ NumBone ];

	for( i=0 ; i<lpIem->NumBone ; i++ ){
		BoneParent[i]     = lpIem->lpBone[i].parent;		//	親
		lpOffsetMatrix[i] = lpIem->lpBone[i].BoneMatrix;	//	基準化行列
		orgPos[i]         = lpIem->lpBone[i].orgPos;		//	標準位置
		orgPose[i]        = lpIem->lpBone[i].orgPose;		//	標準姿勢

		//	クォータニオンコピー
		lpAnime[i].rotNum = lpIem->lpMotion[i].NumRotate;
		lpAnime[i].rotFrame = new WORD[ lpAnime[i].rotNum ];
		lpAnime[i].rot      = new Quaternion[ lpAnime[i].rotNum ];
		for( j=0 ; j<lpAnime[i].rotNum ; j++ ){
			lpAnime[i].rotFrame[j] = lpIem->lpMotion[i].RotateFrame[j];
			lpAnime[i].rot[j] = lpIem->lpMotion[i].Rotate[j];
		}
		//	ポジションコピー
		lpAnime[i].posNum   = lpIem->lpMotion[i].NumPosition;
		if( lpAnime[i].posNum > 0 )
		{
			lpAnime[i].posFrame = new WORD[ lpAnime[i].posNum ];
			lpAnime[i].pos      = new Vector3[ lpAnime[i].posNum ];
		}
		for( j=0 ; j<lpAnime[i].posNum ; j++ ){
			lpAnime[i].posFrame[j] = lpIem->lpMotion[i].PositionFrame[j];
			lpAnime[i].pos[j] = lpIem->lpMotion[i].Position[j];
		}
	}
	//	初期設定
	SetPos( .0f, .0f, .0f );
	SetAngle( .0f, .0f, .0f );
	SetScale( 1.0f, 1.0f, 1.0f );
	dwFlags = 0;
	iexMesh::Update();
	
	return TRUE;
}

//*****************************************************************************************************************************
//	iEMファイル読み込み
//*****************************************************************************************************************************
int		iex3DObj::LoadiEM( LPIEMFILE lpIem, LPSTR filename )
{
	HANDLE	hfile;
	u32		dum, FileID;
	int		version, i;

	hfile = CreateFile( filename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL );
	if( hfile == INVALID_HANDLE_VALUE ) return 0;

	//	ID
	ReadFile( hfile, &FileID, 4, &dum, NULL );
	if( FileID == '1MEI' ) version = 1;
	 else if( FileID == '2MEI' ) version = 2;
	 else if( FileID == '3MEI' ) version = 3;
	 else if( FileID == '4MEI' ) version = 4;
	 else version = -1;

	 lpIem->version = version;
	//	頂点	
	ReadFile( hfile, &lpIem->NumVertex, sizeof(u16), &dum, NULL );
	if( version < 4 )
	{
		lpIem->lpVertex = new MESHVERTEX[ lpIem->NumVertex ];
		ReadFile( hfile, lpIem->lpVertex,   sizeof(MESHVERTEX)*lpIem->NumVertex, &dum, NULL );
	} else {
		lpIem->lpVertex = new MESHVERTEX2[ lpIem->NumVertex ];
		ReadFile( hfile, lpIem->lpVertex,   sizeof(MESHVERTEX2)*lpIem->NumVertex, &dum, NULL );
	}
	//	ポリゴン	
	ReadFile( hfile, &lpIem->NumFace, sizeof(u16), &dum, NULL );
	lpIem->lpFace = new u16[ lpIem->NumFace*3 ];
	lpIem->lpAtr  = new u32[ lpIem->NumFace ];
	ReadFile( hfile, lpIem->lpFace,   sizeof(u16)*lpIem->NumFace*3, &dum, NULL );
	ReadFile( hfile, lpIem->lpAtr,    sizeof(u32)*lpIem->NumFace, &dum, NULL );
	//	マテリアル	
	ReadFile( hfile, &lpIem->NumMaterial, sizeof(u16), &dum, NULL );
	ReadFile( hfile, lpIem->Material,     sizeof(D3DMATERIAL9)*lpIem->NumMaterial, &dum, NULL );
	ReadFile( hfile, lpIem->Texture,      sizeof(char)*lpIem->NumMaterial*64, &dum, NULL );

	//	ボーン
	ReadFile( hfile, &lpIem->NumBone, sizeof(u16), &dum, NULL );
	lpIem->lpBone = new IEMBONE[ lpIem->NumBone ];
	for( i=0 ; i<lpIem->NumBone ; i++ ){
		ReadFile( hfile, &lpIem->lpBone[i], sizeof(IEMBONE), &dum, NULL );

		lpIem->lpBone[i].Index = new DWORD[lpIem->lpBone[i].IndexNum];
		ReadFile( hfile, lpIem->lpBone[i].Index, sizeof(DWORD)*lpIem->lpBone[i].IndexNum, &dum, NULL );

		lpIem->lpBone[i].Influence = new float[lpIem->lpBone[i].IndexNum];
		ReadFile( hfile, lpIem->lpBone[i].Influence, sizeof(float)*lpIem->lpBone[i].IndexNum, &dum, NULL );
	}
	
	//	モーション
	ReadFile( hfile, &lpIem->NumMotion, sizeof(u16), &dum, NULL );
	ReadFile( hfile, &lpIem->MaxFrame,  sizeof(u16), &dum, NULL );
	ReadFile( hfile, &lpIem->M_Offset,  sizeof(u16)*256, &dum, NULL );
	ReadFile( hfile, &lpIem->FrameFlag, sizeof(u16)*lpIem->MaxFrame, &dum, NULL );
	lpIem->lpMotion = new IEMMOTION[ lpIem->NumBone ];

	for( i=0 ; i<lpIem->NumBone ; i++ ){
		ReadFile( hfile, &lpIem->lpMotion[i], sizeof(IEMMOTION), &dum, NULL );

		lpIem->lpMotion[i].Rotate       = new Quaternion[lpIem->lpMotion[i].NumRotate];
		lpIem->lpMotion[i].RotateFrame  = new WORD[lpIem->lpMotion[i].NumRotate];
		ReadFile( hfile, lpIem->lpMotion[i].Rotate, sizeof(Quaternion)*lpIem->lpMotion[i].NumRotate, &dum, NULL );
		ReadFile( hfile, lpIem->lpMotion[i].RotateFrame, sizeof(WORD)*lpIem->lpMotion[i].NumRotate, &dum, NULL );

		lpIem->lpMotion[i].Position      = new Vector3[lpIem->lpMotion[i].NumPosition];
		lpIem->lpMotion[i].PositionFrame = new WORD[lpIem->lpMotion[i].NumPosition];
		ReadFile( hfile, lpIem->lpMotion[i].Position, sizeof(Vector3)*lpIem->lpMotion[i].NumPosition, &dum, NULL );
		ReadFile( hfile, lpIem->lpMotion[i].PositionFrame, sizeof(WORD)*lpIem->lpMotion[i].NumPosition, &dum, NULL );
	}

	CloseHandle(hfile);

	return version;
}

//*****************************************************************************************************************************
//	オブジェクト読み込み
//*****************************************************************************************************************************
BOOL	iex3DObj::LoadObject( char* filename )
{
	IEMFILE		iem;
	char		workpath[MAX_PATH];

	version = LoadiEM( &iem, filename );
	if( version <= 0 )
	{
		return FALSE;
	}

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
	CreateFromIEM( workpath, &iem );

	for( int i=0 ; i<iem.NumBone ; i++ ){
		delete[]	iem.lpBone[i].Index;
		delete[]	iem.lpBone[i].Influence;
		delete[]	iem.lpMotion[i].Rotate;
		delete[]	iem.lpMotion[i].RotateFrame;
		delete[]	iem.lpMotion[i].Position;
		delete[]	iem.lpMotion[i].PositionFrame;
	}
	delete[]	iem.lpVertex;
	delete[]	iem.lpFace;
	delete[]	iem.lpAtr;
	delete[]	iem.lpBone;
	delete[]	iem.lpMotion;

	bLoad = TRUE;

	return TRUE;
}


//*****************************************************************************************************************************
//		３Ｄオブジェクト保存
//*****************************************************************************************************************************
BOOL iex3DObj::SaveObject( LPIEMFILE lpIem, LPSTR filename )
{
	HANDLE	hfile;
	int		i;
	u32		dum;
	u32		FileID = '3MEI';

	if( lpIem->version == 4 ) FileID = '4MEI';

	hfile = CreateFile( filename, GENERIC_WRITE, FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL );
	if( hfile == INVALID_HANDLE_VALUE ) return FALSE;
	//	ID
	WriteFile( hfile, &FileID, 4, &dum, NULL );
	//	頂点	
	WriteFile( hfile, &lpIem->NumVertex, sizeof(u16), &dum, NULL );
	if( lpIem->version < 4 ) WriteFile( hfile, (MESHVERTEX*)(lpIem->lpVertex), sizeof(MESHVERTEX)*lpIem->NumVertex, &dum, NULL );
	else WriteFile( hfile, (MESHVERTEX2*)(lpIem->lpVertex), sizeof(MESHVERTEX2)*lpIem->NumVertex, &dum, NULL );

	//	ポリゴン	
	WriteFile( hfile, &lpIem->NumFace, sizeof(u16), &dum, NULL );
	WriteFile( hfile, lpIem->lpFace,   sizeof(u16)*lpIem->NumFace*3, &dum, NULL );
	WriteFile( hfile, lpIem->lpAtr,    sizeof(u32)*lpIem->NumFace, &dum, NULL );
	//	マテリアル	
	WriteFile( hfile, &lpIem->NumMaterial, sizeof(u16), &dum, NULL );
	WriteFile( hfile, lpIem->Material,     sizeof(D3DMATERIAL9)*lpIem->NumMaterial, &dum, NULL );
	WriteFile( hfile, lpIem->Texture,      sizeof(char)*lpIem->NumMaterial*64, &dum, NULL );

	//	ボーン
	WriteFile( hfile, &lpIem->NumBone, sizeof(u16), &dum, NULL );
	for( i=0 ; i<lpIem->NumBone ; i++ ){
		WriteFile( hfile, &lpIem->lpBone[i],   sizeof(IEMBONE), &dum, NULL );
		WriteFile( hfile, lpIem->lpBone[i].Index,     sizeof(DWORD) *lpIem->lpBone[i].IndexNum, &dum, NULL );
		WriteFile( hfile, lpIem->lpBone[i].Influence, sizeof(float)*lpIem->lpBone[i].IndexNum, &dum, NULL );
	}

	//	モーション
	WriteFile( hfile, &lpIem->NumMotion, sizeof(u16), &dum, NULL );
	WriteFile( hfile, &lpIem->MaxFrame,  sizeof(u16), &dum, NULL );
	WriteFile( hfile, &lpIem->M_Offset,  sizeof(u16)*256, &dum, NULL );
	WriteFile( hfile, &lpIem->FrameFlag, sizeof(u16)*lpIem->MaxFrame, &dum, NULL );

	for( i=0 ; i<lpIem->NumBone ; i++ ){
		WriteFile( hfile, &lpIem->lpMotion[i],   sizeof(IEMMOTION), &dum, NULL );
		WriteFile( hfile, lpIem->lpMotion[i].Rotate,      sizeof(D3DXQUATERNION)*lpIem->lpMotion[i].NumRotate, &dum, NULL );
		WriteFile( hfile, lpIem->lpMotion[i].RotateFrame, sizeof(WORD)          *lpIem->lpMotion[i].NumRotate, &dum, NULL );
		WriteFile( hfile, lpIem->lpMotion[i].Position,      sizeof(D3DXVECTOR3)*lpIem->lpMotion[i].NumPosition, &dum, NULL );
		WriteFile( hfile, lpIem->lpMotion[i].PositionFrame, sizeof(WORD)       *lpIem->lpMotion[i].NumPosition, &dum, NULL );
	}

	CloseHandle(hfile);

	return TRUE;
}











//*****************************************************************************************************************************
//
//	過去のＩＥＸとの互換用
//
//*****************************************************************************************************************************

//		３Ｄオブジェクト読み込み
LPIEX3DOBJ	IEX_Load3DObject( LPSTR filename )
{
	LPIEX3DOBJ	lpObj = new iex3DObj(filename);
	return lpObj;
}

//		解放
void	IEX_Release3DObject( LPIEX3DOBJ lpObj )
{
	if( !lpObj ) return;
	delete lpObj;
}

//		レンダリング
void	IEX_NoRender3DObject( LPIEX3DOBJ lpObj )
{
	if( !lpObj ) return;
	lpObj->Update();
}

void	IEX_Render3DObject( LPIEX3DOBJ lpObj )
{
	if( !lpObj ) return;
	lpObj->Render();

}

void	IEX_Render3DObject( LPIEX3DOBJ lpObj, DWORD flag, float alpha )
{
	if( !lpObj ) return;
	lpObj->Render( flag, alpha );
}

void	IEX_Render3DObject( LPIEX3DOBJ lpObj, iexShader* shader, char* name )
{
	if( !lpObj ) return;
	lpObj->Render( shader, name );
}

//		フレーム進行
void	IEX_ObjectFrameNext( LPIEX3DOBJ lpObj )
{
	if( !lpObj ) return;
	lpObj->Animation();
}

//		モーション設定
void	IEX_SetObjectMotion( LPIEX3DOBJ lpObj, int motion )
{
	if( !lpObj ) return;
	lpObj->SetMotion(motion);
}

//		モーション取得
int		IEX_GetObjectMotion( LPIEX3DOBJ lpObj )
{
	if( !lpObj ) return -1;
	return	lpObj->GetMotion();
}

//		座標変更
void	IEX_SetObjectPos( LPIEX3DOBJ lpObj, float x, float y, float z  )
{
	if( !lpObj ) return;
	lpObj->SetPos( x, y, z );
}

//		回転変更
void	IEX_SetObjectAngle( LPIEX3DOBJ lpObj, float x, float y, float z  )
{
	if( !lpObj ) return;
	lpObj->SetAngle( x, y, z );
}

//		スケール変更
void	IEX_SetObjectScale( LPIEX3DOBJ lpObj, float scale )
{
	if( !lpObj ) return;
	lpObj->SetScale( scale );
}

void	IEX_SetObjectScale( LPIEX3DOBJ lpObj, float scaleX, float scaleY, float scaleZ )
{
	if( !lpObj ) return;
	lpObj->SetScale( scaleX, scaleY, scaleZ );
}

//		パラメータ取得
u8	IEX_GetObjectParam( LPIEX3DOBJ lpObj, int index )
{
	return	lpObj->GetParam(index);
}

void	IEX_SetObjectParam( LPIEX3DOBJ lpObj, int index, u8 param )
{
	lpObj->SetParam( index, param );
}


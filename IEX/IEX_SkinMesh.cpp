#include	"iextreme.h"

//**************************************************************************************************
//
//		スキンメッシュ関連
//
//**************************************************************************************************

//*****************************************************************************
//	フレーム更新
//*****************************************************************************
void	iex3DObj::UpdateSkinMeshFrame( float frame )
{
	u32			i, j;
	LPIEXANIME2	lpAnime;
	float		t;

	for( i=0 ; i<NumBone ; i++ ){
		lpAnime = &this->lpAnime[i];

		//	ポーズ設定
		if( lpAnime->rotNum == 0 ) CurPose[i] = orgPose[i];
		 else if( lpAnime->rotNum == 1 ) CurPose[i] = lpAnime->rot[0];
		 else {
			//	回転キー補間
			for( j=0 ; j<lpAnime->rotNum-1 ; j++ )
			{
				//	現在位置検索
				if( (frame>=lpAnime->rotFrame[j]) && (frame<lpAnime->rotFrame[j+1]) )
				{
					//	経過フレーム計算
					t = (float)(frame-lpAnime->rotFrame[j]) / (float)(lpAnime->rotFrame[j+1] - lpAnime->rotFrame[j]);
					//	補間
					CurPose[i] = QuaternionSlerp( lpAnime->rot[j], lpAnime->rot[j+1], t );
					break;
				}
			}
			if( j == lpAnime->rotNum-1 ) CurPose[i] = lpAnime->rot[lpAnime->rotNum-1];
		}
		//	座標設定
		if( lpAnime->posNum == 0 ) CurPos[i] = orgPos[i];
		 else {
			//	位置補間
			for( j=0 ; j<lpAnime->posNum-1 ; j++ )
			{
				//	現在位置検索
				if( (frame>=lpAnime->posFrame[j]) && (frame<lpAnime->posFrame[j+1]) )
				{
					//	経過フレーム計算
					t = (float)(frame-lpAnime->posFrame[j]) / (float)(lpAnime->posFrame[j+1] - lpAnime->posFrame[j]);
					//	補間
					CurPos[i] = lpAnime->pos[j] + (lpAnime->pos[j+1]-lpAnime->pos[j])*t;
					break;
				}
			}
			if( j == lpAnime->posNum-1 ) CurPos[i] = lpAnime->pos[lpAnime->posNum-1];
		}
	}
}

//*****************************************************************************
//	ボーン行列更新
//*****************************************************************************
void iex3DObj::UpdateBoneMatrix()
{
	u32			i;

	//	ボーン更新
	for( i=0 ; i<NumBone ; i++ )
	{
		CurPose[i].toMatrix( lpBoneMatrix[i] );
		//	位置情報コピー
		lpBoneMatrix[i]._41 = CurPos[i].x;
		lpBoneMatrix[i]._42 = CurPos[i].y;
		lpBoneMatrix[i]._43 = CurPos[i].z;

		if( BoneParent[i] != 0xFFFF ) lpBoneMatrix[i] *= lpBoneMatrix[ BoneParent[i] ];
	}
	for( i=0 ; i<NumBone ; i++ )
	{
		lpMatrix[i] = lpOffsetMatrix[i] * lpBoneMatrix[i];
	}
}


//*****************************************************************************
//	スキンメッシュ更新
//*****************************************************************************
void	iex3DObj::UpdateSkinMesh()
{
	//	メッシュ更新
	void*	pVertex;
	lpMesh->LockVertexBuffer( D3DLOCK_DISCARD, &pVertex );
	lpSkinInfo->UpdateSkinnedMesh( lpMatrix, NULL, lpVertex, pVertex );
	lpMesh->UnlockVertexBuffer();
}


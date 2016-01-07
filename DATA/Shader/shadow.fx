
//************************************************************************
//
//	Shadow
//
//************************************************************************

//-----------------------------------------------------------------------------------
//	環境関連
//-----------------------------------------------------------------------------------
	float4x4		ShadowProjection;

//-----------------------------------------------------------------------------------
//	テクスチャサンプラー
//-----------------------------------------------------------------------------------

	//	シャドウマップ
	texture	ShadowMap;
	sampler	ShadowSamp = sampler_state
	{
		Texture = <ShadowMap>;
		MinFilter = POINT;
		MagFilter = POINT;
		MipFilter = NONE;

		BorderColor = 0xFFFFFFFF;
		AddressU = BORDER;
		AddressV = BORDER;
	};

//-----------------------------------------------------------------------------------
//	頂点フォーマット
//-----------------------------------------------------------------------------------

	struct VS_SHADOW
	{
		float4	Pos	:	POSITION;
		float4	Color	:	TEXCOORD0;		//	頂点色
	};

	struct V_FULL_S
	{
		float4	Pos		:	POSITION;
		float4	Color		:	COLOR0;
		float2	Tex		:	TEXCOORD0;
		float4	Ambient	:	COLOR1;
		float4	light		:	COLOR2;
		float3	vLight	:	TEXCOORD1;
		float3	vE			:	TEXCOORD2;
		float3	pLight	:	TEXCOORD3;

		float3	vShadow	:	TEXCOORD4;
	};

//-----------------------------------------------------------------------------------
//	シャドウマップ
//-----------------------------------------------------------------------------------

	//	パラメータ
	float		adjustValue = -0.004f;
	float		Shadow = 0.4f;
	float		flashParam = 0.0f;

	//	影テクスチャ情報取得
	inline		float3	GetShadowTex( float3 Pos )
	{
		float3	Tex;

		//	テクスチャ座標計算
		float4	ppp;
		ppp.xyz = Pos;
		ppp.w = 1;
		Tex = mul( ppp, ShadowProjection );

		Tex.y = -Tex.y;
		Tex.xy = 0.5f * Tex.xy + 0.5f;

		return	Tex;
	}

	//	影情報取得
	inline		float		GetShadow( float3	 Tex )
	{
		float	d = tex2D( ShadowSamp, Tex.xy ).r;	//	シャドウマップから深度を取り出す
		float	l = ( d < Tex.z + adjustValue ) ? Shadow : 1;

		return	l;
	}

//************************************************************************
//
//	シャドウマップ
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		VS_SHADOW	VS_ShadowBuf( float4 Pos	: POSITION )
		{
			VS_SHADOW	Out;

			//	座標変換
			float4x4		mat = mul( TransMatrix, ShadowProjection );
			Out.Pos = mul( Pos, mat );
			Out.Color = Out.Pos.z;

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		float4	PS_ShadowBuf( VS_SHADOW In )	:	COLOR
		{			
			return	In.Color;
		}

//**************************************************************************************************
//
//		シャドウマップ採用
//
//**************************************************************************************************

	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		V_FULL_S	VS_Full_S( VS_INPUT	In )
		{
			V_FULL_S	Out = ( V_FULL_S )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Color = 1;
			Out.Tex = In.Tex;

			float4	P = mul( In.Pos, TransMatrix );
			float3x3	mat = TransMatrix;
			float3 N = mul( In.Normal, mat );
			N = normalize( N );

			//	半球ライティング
			Out.Ambient.rgb = HemiSphereLight( N );

			//	頂点ローカル座標系算出
			float3	vx;
			float3	vy = { 0.0f, 1.0f, 0.001f };
			vx = cross( vy, N );
			vx = normalize( vx );
			vy = cross( vx, N );
			vy = normalize( vy );

			//	ライトベクトル補正
			Out.vLight.x = dot( vx, DirLightVec );
			Out.vLight.y = dot( vy, DirLightVec );
			Out.vLight.z = dot( N, DirLightVec );

			//	点光源ベクトル補正
			float3	pvec = ( P - plight_pos ) / plight_range;

			//	ライトベクトル補正
			Out.pLight.x = dot( vx, pvec );
			Out.pLight.y = dot( vy, pvec );
			Out.pLight.z = dot( N, pvec );

			//	視線ベクトル補正
			float3	E = P - ViewPos;	//	視線ベクトル
			Out.vE.x = dot( vx, E );
			Out.vE.y = dot( vy, E );
			Out.vE.z = dot( N, E );

			//	シャドウマップ
			Out.vShadow = GetShadowTex( P );
			return	Out;
		}

		//-----------------------------------------------------------------------------------
		//	ピクセルシェーダー
		//-----------------------------------------------------------------------------------
		float4	PS_Full_S( V_FULL_S In ) : COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	パララックスディスプレースメント
			float	h = tex2D( HeightSamp, Tex ).r - 0.5f;
			float3	E = normalize( In.vE );
			Tex.x += 0.04f * h * E.x;
			Tex.y -= 0.04f * h * E.y;

			//	法線取得
			float3	N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

			//	ライト計算
			In.vLight = normalize( In.vLight );
			float3	light = DirLight2( In.vLight, N );

			//	点光源
			float	plight = max( 0, 1 - length( In.pLight ) );
			In.pLight = normalize( In.pLight );
			plight *= max( 0, dot( -In.pLight, N ) );

			light += plight_color * plight;

			//	視線反射ベクトル
			float3	R = reflect( -E, N );

			//	スペキュラマップ取得
			float4	sp_tex = tex2D( SpecularSamp, Tex );

			//	環境マップ
			float3	env = Environment( R ) * ( 1 - sp_tex.a );

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb = ( OUT.rgb * ( light + In.Ambient.rgb ) );
			OUT.rgb = OUT.rgb * env + OUT.rgb;

			//	スペキュラ
			OUT.rgb += pow( max( 0, dot( R, In.vLight ) ), 10 ) * sp_tex;

			//	シャドウマップ適用
			OUT.rgb *= GetShadow( In.vShadow );

			OUT.r += flashParam;
			OUT.g += flashParam;
			OUT.b += flashParam;

			return	OUT;
		}

		//-----------------------------------------------------------------------------------
		//	ピクセルシェーダー
		//-----------------------------------------------------------------------------------
		float4	PS_NoLight_S( V_FULL_S In ) : COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	パララックスディスプレースメント
			float	h = tex2D(HeightSamp, Tex).r - 0.5f;
			float3	E = normalize(In.vE);
			Tex.x += 0.04f * h * E.x;
			Tex.y -= 0.04f * h * E.y;

			//	法線取得
			float3	N = tex2D(NormalSamp, Tex).xyz * 2.0f - 1.0f;

			//	ピクセル色決定
			OUT = In.Color * tex2D(DecaleSamp, Tex);

			//	シャドウマップ適用
			OUT.rgb *= GetShadow(In.vShadow);

			return	OUT;
		}

//************************************************************************
//
//	テクニック
//
//************************************************************************

	//	シャドウマップ
	technique	ShadowBuf
	{
		pass	Pass0
		{
			AlphaBlendEnable		=		false;
			ZWriteEnable				=		true;
			CullMode					=		none;

			VertexShader		=		compile vs_2_0	VS_ShadowBuf();
			PixelShader		=		compile ps_2_0 PS_ShadowBuf();
		}
	}


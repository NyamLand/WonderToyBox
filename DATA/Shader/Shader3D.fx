
//-----------------------------------------------------------------------------------
//	環境関連
//-----------------------------------------------------------------------------------

	float4x4		Projection;	//	投影変換行列
	float4x4		TransMatrix;	//	ワールド変換行列
	float4x4		matView;
	float3		ViewPos = { 0.0f, 0.0f, 0.0f };	//	視線変数

//-----------------------------------------------------------------------------------
//	テクスチャサンプラー
//-----------------------------------------------------------------------------------

	//	デカールサンプラー
	texture	Texture;
	sampler	DecaleSamp = sampler_state
	{
		Texture		=	<Texture>;
		MinFilter	=	LINEAR;
		MagFilter	=	LINEAR;
		MipFilter	=	NONE;

		AddressU	=	Wrap;
		AddressV	=	Wrap;
	};

	//	環境用テクスチャサンプラー
	texture	EnvTex;
	sampler	EnvironmentSamp = sampler_state
	{
		Texture = <EnvTex>;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

	//	法線テクスチャサンプラー
	texture	NormalMap;
	sampler	NormalSamp = sampler_state
	{
		Texture = <NormalMap>;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

	//	高さテクスチャサンプラー
	texture	HeightMap;
	sampler	HeightSamp = sampler_state
	{
		Texture = <HeightMap>;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

	//	スペキュラサンプラー
	texture SpecularMap;
	sampler SpecularSamp = sampler_state
	{
		Texture = <SpecularMap>;

		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

	//	反射マップ
	texture	RefMap;
	sampler	RefSamp = sampler_state
	{
		Texture = <RefMap>;
		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

	//	フェイスサンプラー
	texture SSSMap;
	sampler FaceSamp = sampler_state
	{
		Texture = <SSSMap>;

		MinFilter = LINEAR;
		MagFilter = LINEAR;
		MipFilter = NONE;

		AddressU = Wrap;
		AddressV = Wrap;
	};

//-----------------------------------------------------------------------------------
//	頂点フォーマット
//-----------------------------------------------------------------------------------
	
	struct VS_OUTPUT
	{
		float4	Pos	:	POSITION;
		float4	Color	:	COLOR0;
		float2	Tex	:	TEXCOORD0;

		float4	Ambient	:	COLOR1;
		//float4	light			:	COLOR2;
		float3	vLight	:	TEXCOORD1;		//	ライトベクトル(頂点空間)
		float3	vE			:	TEXCOORD2;		//	視線ベクトル(頂点空間)

		float3	pLight	:	TEXCOORD3;	//	点光源パラメータ
	};

	struct VS_INPUT
	{
		float4	Pos		:	POSITION;
		float3	Normal	:	NORMAL;
		float2	Tex		:	TEXCOORD0;

		float4	Color		:	COLOR0;	//	頂点カラー
	};

	struct VS_OUTPUT_FOG
	{
		float4	Pos	:	POSITION;
		float4	Color	:	COLOR0;
		float2	Tex	:	TEXCOORD0;

		float		Fog	:	TEXCOORD1;

		float4	worldPos		:	TEXCOORD2;		//	ワールドの座標
		float3	worldNormal	:	TEXCOORD3;		//	ワールドの法線
	};

	struct VS_OUTPUT_TOON
	{
		float4	Pos		:	POSITION;
		float4	Color		:	COLOR0;
		float2	Tex		:	TEXCOORD0;

		float		Level		:	TEXCOORD1;
	};

	struct VS_HDR
	{
		float4	Pos		:	POSITION;
		float3	wPos		:	TEXCOORD2;
		float3	Normal	:	TEXCOORD1;
		float2	Tex		:	TEXCOORD0;
	};

	struct V_FULL
	{
		float4	Pos		:	POSITION;
		float4	Color		:	COLOR0;
		float2	Tex		:	TEXCOORD0;

		float4	Ambient	:	COLOR1;
		float4	light			:	COLOR2;
		float3	vLight		:	TEXCOORD1;
		float3	vE				:	TEXCOORD2;

		float3	pLight		:	TEXCOORD3;	//	点光源パラメータ
	};

	struct V_EFFECT
	{
		float4	Pos	:	POSITION;
		float4	Color	:	COLOR0;
		float2	Tex	:	TEXCOORD0;
	};

	struct VS_DEPTH
	{
		float4	Pos		:	POSITION;
		float		Depth	:	TEXCOORD0;
	};

	struct V_FULL_SR
	{
		float4	Pos			:	POSITION;
		float4	Color			:	COLOR0;
		float2	Tex			:	TEXCOORD0;
		float4	Ambient	:	COLOR1;
		float4	light			:	COLOR2;
		float3	vLight		:	TEXCOORD1;
		float3	vE				:	TEXCOORD2;
		float3	pLight		:	TEXCOORD3;
		
		float3	vShadow	:	TEXCOORD4;
		float4	wPos			:	TEXCOORD5;
	};
	
//-----------------------------------------------------------------------------------
//	環境光
//-----------------------------------------------------------------------------------
	
	//	色
	float3	AmbientColor = { 0.9f, 0.9f, 0.9f };

//-----------------------------------------------------------------------------------
//	平行光
//-----------------------------------------------------------------------------------

	//	パラメータ
	float3	DirLightVec = { 1.0f, 0.0f, 1.0f };
	float3	DirLightColor = { 1.0f, 1.0f, 1.0f };

	//	平行光
	float3	DirLight( float3 normal )
	{
		float3	vec = normalize( DirLightVec );
		float		intensity = max( 0, dot( vec, -normal ) );

		return	DirLightColor * intensity;
	}

	//	平行光２
	float3	DirLight2( float3 light, float3 normal )
	{
		float3	vec = normalize( light );
		float	intensity = max( 0, dot( vec, -normal ) );
		
		return	DirLightColor * intensity;
	}

//-----------------------------------------------------------------------------------
//	点光源
//-----------------------------------------------------------------------------------

	//	パラメータ
	float3	plight_color = { 1.0f, 0.0f, 0.5f };
	float3	plight_pos = { 0.0f, 0.5f, 0.0f };
	float		plight_range = 5.0f;

//-----------------------------------------------------------------------------------
//	半球ライティング
//-----------------------------------------------------------------------------------

	//	パラメータ
	float3	SkyColor = { 0.3f, 0.4f, 0.5f };
	float3	GroundColor = { 0.4f, 0.3f, 0.1f };
	float3	BulletColor = { 0.0f, 0.0f, 0.0f };

	//	半球ライティング
	float3	HemiSphereLight( float3 normal )
	{
		float	rate = normal.y * 0.5f + 0.5f;
		float3 color = GroundColor * (1 - rate) + SkyColor * rate;

		return	color;
	}

//-----------------------------------------------------------------------------------
//	スキンシェーダー
//-----------------------------------------------------------------------------------

	//	反射色
	float3	epiScatter = { 1.0f, 0.85f, 0.8f };	//	1次反射色
	float3	subScatter = { 0.95f, 0.5f, 0.4f };	//	２次反射色

	//	テカリ色
	float3	sp_primary = { 0.75f, 0.9f, 1.0f };
	float3	sp_secondary = { 0.9f, 0.95f, 1.0f };

	//	透過色
	float3	backScatter = { 0.7f, 0.1f, 0.1f };

//-----------------------------------------------------------------------------------
//	フォグ
//-----------------------------------------------------------------------------------

	//	パラメータ
	float		FogNear		=		10.0f;
	float		FogFar			=		25.0f;
	float3	FogColor		=		{ 0.9f, 0.95f, 1.0f };

	//	フォグ
	float	Fog( float3 pos )
	{
		float rate;
		rate = ( FogFar - pos.z ) / ( FogFar - FogNear );
		rate = saturate( rate );

		return	rate;
	}

//-----------------------------------------------------------------------------------
//	スペキュラ描画
//-----------------------------------------------------------------------------------

	//	スペキュラ
	float	Specular( float3 pos, float3 N )
	{
		float	rate;

		float3	vE = normalize( pos - ViewPos );	//	視線ベクトル
		float3	vL = normalize( DirLightVec );		//	ライトベクトル
		float3	H = -normalize( vE + vL );				//	視線とライトのハーフ

		rate = max( 0, dot( H, N ) );	//	明るさ算出
		rate = pow( rate, 30 );			//	調整(乗)

		return	rate;
	}

	//	スペキュラ計算( 通常よりキツイ )
	float	HDRSpecular( float3 pos, float3 normal )
	{
		float	sp;
		float3 H = normalize( ViewPos - pos );
		H = normalize( H - DirLightVec );

		sp = dot( normal, H );
		sp = max( 0, sp );
		sp = pow( sp, 50 );

		return	sp;
	}

//-----------------------------------------------------------------------------------
//	環境マップ
//-----------------------------------------------------------------------------------

	//	映り込みの具合
	float	EnvParam = 0.5f;

	//	環境マップ
	float4	Environment( float3 normal )
	{
		float4	color;

		float2	uv;
		uv = normal.xy * 0.5f + 0.5f;
		uv.y = 1 - uv.y;
		color = tex2D( EnvironmentSamp, uv );
		
		return	color;
	}

//-----------------------------------------------------------------------------------
//	トゥーン
//-----------------------------------------------------------------------------------

	//	輪郭線の太さ
	float	OutlineSize = 1.5f;	

	//	輪郭線色
	float3	OutlineColor = float3( 0.0f, 0.0f, 0.0f );

	//	影用
	float	ToonLevel = 0.3f;		//	影のレベル
	float	ToonShadow = 0.4f;	//	影の濃さ

//-----------------------------------------------------------------------------------
//	エフェクト用
//-----------------------------------------------------------------------------------

	//	パラメータ
	float	adjustU = 0;
	float	adjustV = 0;

	//	その他
	float3		colorParam = { 0.0f, 0.0f, 0.0f };

//************************************************************************
//
//	通常描画
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		VS_OUTPUT	VS_Basic( VS_INPUT	In )
		{
			VS_OUTPUT	Out = ( VS_OUTPUT )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;
			Out.Color = 1.0f;

			//	環境光適用
			Out.Color.rgb = AmbientColor;

			//	法線変換
			float3x3		mat = ( float3x3 )TransMatrix;
			float3		N = mul( In.Normal, mat );

			//	平行光適用
			Out.Color.rgb += DirLight( N );
			
			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		float4	PS_Basic( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			return	OUT;
		}

//************************************************************************
//
//	半球ライティングとフォグ
//
//************************************************************************
	
	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		VS_OUTPUT_FOG	VS_Lighting( VS_INPUT In )
		{
			VS_OUTPUT_FOG	Out = ( VS_OUTPUT_FOG )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;
			Out.Color.a = 1;

			//	法線変換
			float3x3	 mat = ( float3x3 )TransMatrix;
			float3	N = mul( In.Normal, mat );
			N = normalize( N );

			//	平行光適用
			Out.Color.rgb += DirLight( N );

			//	半球ライティング適用
			Out.Color.rgb += HemiSphereLight( N );

			//	フォグ
			Out.Fog = Fog( Out.Pos );

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		float4	PS_Lighting( VS_OUTPUT_FOG In )	:	COLOR
		{
			float4	OUT;

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			//	フォグ採用
			OUT.rgb = ( OUT.rgb * In.Fog ) + ( FogColor * ( 1 - In.Fog ) );

			return	OUT;
		}

//************************************************************************
//
//	スペキュラ
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		VS_OUTPUT_FOG	VS_Lighting2( VS_INPUT In )
		{
			VS_OUTPUT_FOG	Out = ( VS_OUTPUT_FOG )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;
			Out.Color.a = 1;

			//	法線変換
			float3x3	 mat = ( float3x3 )TransMatrix;
			float3	N = mul( In.Normal, mat );
			N = normalize( N );

			//	平行光適用
			Out.Color.rgb += DirLight( N );

			//	半球ライティング適用
			Out.Color.rgb += HemiSphereLight( N );

			//	フォグ
			Out.Fog = Fog( Out.Pos );

			//	グローバルポジション＆法線
			Out.worldPos = mul( In.Pos, TransMatrix );
			float3x3		m = ( float3x3 )TransMatrix;
			Out.worldNormal = mul( In.Normal, m );

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		float4	PS_Lighting2( VS_OUTPUT_FOG In ) : COLOR
		{
			float4	OUT;

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			In.worldNormal = normalize( In.worldNormal );

			//	スペキュラ
			OUT.rgb += Specular( In.worldPos, In.worldNormal );

			//	フォグ採用
			OUT.rgb = ( OUT.rgb * In.Fog ) + ( FogColor * ( 1 - In.Fog ) );

			return	OUT;
		}

//************************************************************************
//
//	環境マップ
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		float4	PS_Env( VS_OUTPUT_FOG In )	:	COLOR
		{
			float4	OUT;

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );
			In.worldNormal = normalize( In.worldNormal );

			//	環境マップUV計算
			float2	euv = ( In.worldNormal.xy * 0.5f ) + 0.5f;
			euv.y = 1 - euv;

			//	環境マップ
			float4	Env = tex2D( EnvironmentSamp, euv );

			//	環境マップブレンド
			OUT = ( OUT * ( 1 - EnvParam ) ) + ( Env * EnvParam );

			//	スペキュラ
			OUT.rgb += Specular( In.worldPos, In.worldNormal );

			//	フォグ採用
			OUT.rgb = ( OUT.rgb * In.Fog ) + ( FogColor * ( 1 - In.Fog ) );

			return	OUT;
		}

//************************************************************************
//
//	トゥーンシェーダー（高鳥義行担当）
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	アウトライン頂点シェーダー
	//-----------------------------------------------------------------------------------
		VS_OUTPUT	VS_Outline( VS_INPUT In )
		{
			VS_OUTPUT Out = ( VS_OUTPUT )0;

			//	法線方向に拡大
			In.Normal = normalize( In.Normal );
			In.Pos.xyz += In.Normal * OutlineSize;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;
			Out.Color.rgb = OutlineColor;
			Out.Color.a = 1;

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	トゥーン影シェーダー
	//-----------------------------------------------------------------------------------
		VS_OUTPUT_TOON	VS_Toon( VS_INPUT In )
		{
			VS_OUTPUT_TOON	Out = ( VS_OUTPUT_TOON )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;
			Out.Color = 1;

			//	法線変換
			float3x3		mat = TransMatrix;
			float3	N = mul( In.Normal, mat );
			N = normalize( N );

			//	ライト計算
			float3	vec = normalize( DirLightVec );
			Out.Level = dot( vec, -N );

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		
		//	輪郭線
		float4	PS_Outline( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;

			//	ピクセル色決定
			OUT = In.Color;

			OUT.r += colorParam.r;
			OUT.g += colorParam.g;
			OUT.b += colorParam.b;

			return	OUT;
		}

		//	トゥーン影シェーダー
		float4	PS_Toon( VS_OUTPUT_TOON In )	:	COLOR
		{
			float4	OUT;

			//	ピクセル色決定
			OUT = tex2D( DecaleSamp, In.Tex );

			//	レベル未満を影にする
			if ( In.Level < ToonLevel )	OUT.rgb *= ToonShadow;
			
			OUT.r += colorParam.r;
			OUT.g += colorParam.g;
			OUT.b += colorParam.b;

			return	OUT;
		}

//************************************************************************
//
//	法線マップ,視差マップ
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		VS_OUTPUT	VS_FullFX( VS_INPUT In )
		{
			VS_OUTPUT	Out = ( VS_OUTPUT )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Color = In.Color;
			Out.Tex = In.Tex;

			float4		P			=	mul(In.Pos, TransMatrix);
			float3x3		mat		=	TransMatrix;
			float3		N			=	mul( In.Normal, mat );

			N = normalize( N );

			Out.Color.rgb = AmbientColor;
			Out.Color.rgb += HemiSphereLight( N );

			//	頂点ローカル座標系算出
			float3	vx;
			float3	vy = { 0.0f, 1.0f, 0.001f };
			vx = cross( vy, N );
			vx = -normalize( vx );
			vy = cross( N, vx );
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

			//	視差ベクトル補正
			float3	E = P - ViewPos;	//	視線ベクトル
			Out.vE.x = dot( vx, E );
			Out.vE.y = dot( vy, E );
			Out.vE.z = dot( N, E );

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー(法線マップ)
	//-----------------------------------------------------------------------------------
		float4	PS_Level1( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	法線取得
			float3	N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

			//	ライト計算
			In.vLight = normalize( In.vLight );
			float3	light = DirLight2( In.vLight, N );

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb = ( OUT.rgb + light );

			return	OUT;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー(視差マップ適用)
	//-----------------------------------------------------------------------------------
		float4	PS_Level2( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	パララックスディスプレースメント
			float		h = tex2D( HeightSamp, Tex ).r - 0.5f;
			float3	E = normalize(In.vE);
			Tex.x -= 0.04f * h * E.x;
			Tex.y += 0.04f * h * E.y;

			//	法線取得
			float3	N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

			//	ライト計算
			In.vLight = normalize( In.vLight );
			float3	light = DirLight2( In.vLight, N );

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb = ( OUT.rgb * light );

			return	OUT;
		}

//************************************************************************
//
//	スペキュラマップ,環境マップ
//
//************************************************************************
	
	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー(スペキュラマップ適用)
	//-----------------------------------------------------------------------------------
		float4	PS_Level3( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	パララックスディスプレースメント
			float	h = tex2D( HeightSamp, Tex ).r - 0.5f;
			float3	E = normalize( In.vE );
			Tex.x -= 0.04f * h * E.x;
			Tex.y += 0.04f * h * E.y;

			//	法線取得
			float3	N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

			//	ライト計算
			In.vLight = normalize( In.vLight );
			float3	light = DirLight2( In.vLight, N );

			//	視線反射ベクトル
			float3	R = reflect( -E, N );

			//	スペキュラマップ取得
			float4	sp_tex = tex2D( SpecularSamp, Tex );

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb = ( OUT.rgb * light );

			//	スペキュラ
			OUT.rgb += pow( max( 0, dot( R, In.vLight ) ), 10 ) * sp_tex;

			return	OUT;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー(環境マップ適用)
	//-----------------------------------------------------------------------------------
		float4	PS_Level4( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	パララックスディスプレースメント
			float	h = tex2D( HeightSamp, Tex ).r - 0.5f;
			float3	E = normalize(In.vE);
			Tex.x -= 0.04f * h * E.x;
			Tex.y += 0.04f * h * E.y;

			//	法線取得
			float3	N = tex2D(NormalSamp, Tex).xyz * 2.0f - 1.0f;

			//	ライト計算
			In.vLight = normalize( In.vLight );
			float3	light = DirLight2( In.vLight, N );

			//	視線反射ベクトル
			float3	R = reflect( -E, N );

			//	スペキュラマップ取得
			float4	sp_tex = tex2D( SpecularSamp, Tex );

			//	環境マップ
			float3	env = ( 1 - sp_tex.a ) * Environment( R );

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb = ( OUT.rgb + light + env );

			//	スペキュラ
			OUT.rgb += pow( max( 0, dot( R, In.vLight ) ), 10 ) * sp_tex;

			return	OUT;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー(点光源適用)
	//-----------------------------------------------------------------------------------
		float4	PS_Level5( VS_OUTPUT In ) : COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	パララックスディスプレースメント
			float	h = tex2D( HeightSamp, Tex ).r - 0.5f;
			float3	E = normalize( In.vE );
			Tex.x -= 0.04f * h * E.x;
			Tex.y += 0.04f * h * E.y;

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
			float3	env = ( 1 - sp_tex.a ) * Environment( R );

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb = ( OUT.rgb + light + env );

			//	スペキュラ
			OUT.rgb += pow( max( 0, dot( R, In.vLight ) ), 10 ) * sp_tex;

			return	OUT;
		}

//************************************************************************
//
//	擬似HDR表現
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		VS_HDR	VS_Specular( VS_INPUT In )
		{
			VS_HDR	Out = ( VS_HDR )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;

			//	スペキュラ計算用座標
			Out.wPos = mul( In.Pos, TransMatrix );

			//	スペキュラ計算用法線
			float3x3		mat		=		TransMatrix;
			float3		N			=		mul( In.Normal, mat );
			Out.Normal			=		normalize( N );

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		float4	PS_Specular( VS_HDR In )	:	COLOR
		{
			float4	OUT;

			//	スペキュラ計算
			float	s = HDRSpecular( In.wPos, In.Normal );
			float4	sp = tex2D( SpecularSamp, In.Tex ) * s;

			//	ピクセル色決定
			OUT.rgb = sp * 0.5f;
			OUT.a = 1;
			
			return	OUT;
		}

//************************************************************************
//
//	エフェクト用シェーダー
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		V_EFFECT	VS_Effect( VS_INPUT In )
		{
			V_EFFECT	Out = ( V_EFFECT )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Color = In.Color;
			Out.Tex = In.Tex;

			Out.Tex.x += adjustU;
			Out.Tex.y += adjustV;

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		float4	PS_Effect( VS_INPUT In ) : COLOR
		{
			float4	OUT;

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, In.Tex );

			return	OUT;
		}

//**************************************************************************************************
//
//		点光源付きフルエフェクト
//
//**************************************************************************************************

	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		V_FULL	VS_Full( VS_INPUT	In )
		{
			V_FULL	Out = ( V_FULL )0;

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

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		float4	PS_Full( V_FULL In )	:	COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	パララックスディスプレースメント
			float	h = tex2D( HeightSamp, Tex ).r - 0.5f;
			float3	E = normalize(In.vE);
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

			return	OUT;
		}

//************************************************************************
//
//	深度描画
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		VS_DEPTH		VS_Depth( float4 Pos : POSITION )
		{
			VS_DEPTH		Out;

			Out.Pos = mul( Pos, Projection );
			Out.Depth = Out.Pos.z;		//	深度

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		float4	PS_Depth( VS_DEPTH In )	:	COLOR
		{
			float4	OUT;
			OUT.rgb = In.Depth;	//	深度を色として描画
			OUT.a = 1;
			return	OUT;
		}

//	shadow.fxのインクルード
#include	"shadow.fx"

//************************************************************************
//
//	反射マップ作成
//
//************************************************************************
	
	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		V_FULL	VS_Reflect( VS_INPUT In )
		{
			V_FULL	Out = ( V_FULL )0;

			In.Pos.y = -In.Pos.y;

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

			return	Out;
		}

//************************************************************************
//
//	反射マップ付き
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		V_FULL_SR	VS_FullFX_SR( VS_INPUT	In )
		{
			V_FULL_SR	Out = ( V_FULL_SR )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Color = 1;
			Out.Tex = In.Tex;

			Out.wPos = mul( In.Pos, Projection );

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
		float4	PS_FullFX_SR( V_FULL_SR In ) : COLOR
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

			float2	ref = ( In.wPos.xy / In.wPos.w ) * 0.5f + 0.5f;
			ref.y = -ref.y;
			float3	Env = tex2D( RefSamp, ref ) * ( OUT.rgb + 0.4f );
			OUT.rgb = lerp( Env, OUT.rgb, 0.3f );

			return	OUT;
		}

//************************************************************************
//
//	スキンシェーダー
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		VS_OUTPUT	VS_FullVX( VS_INPUT In )
		{
			VS_OUTPUT	Out = ( VS_OUTPUT )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Color = In.Color;
			Out.Tex = In.Tex;

			float4	P = mul( In.Pos, TransMatrix );

				float3x3	mat = ( float3x3 )TransMatrix;
				float3	N = mul( In.Normal, mat );
				N = normalize( N );

			//	半球ライティング
			Out.Ambient.rgb = HemiSphereLight( N ).rgb;

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

			//	視線ベクトル補正
			float3	E = P.xyz - ViewPos;	//	視線ベクトル
			Out.vE.x = dot( vx, E );
			Out.vE.y = dot( vy, E );
			Out.vE.z = dot( N, E );

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	バレット用シェーダー
	//-----------------------------------------------------------------------------------
	V_FULL	VS_Bullet(VS_INPUT	In)
	{
			V_FULL	Out = (V_FULL)0;

			Out.Pos = mul(In.Pos, Projection);
			Out.Color = 1;
			Out.Tex = In.Tex;

			float4	P = mul(In.Pos, TransMatrix);
				float3x3	mat = TransMatrix;
				float3 N = mul(In.Normal, mat);
				N = normalize(N);

			float	rate = N * 0.5f + 0.5f;
			float3 color = BulletColor * rate;
			Out.Ambient.rgb += color;

			//	頂点ローカル座標系算出
			float3	vx;
			float3	vy = { 0.0f, 1.0f, 0.001f };
				vx = cross(vy, N);
			vx = normalize(vx);
			vy = cross(vx, N);
			vy = normalize(vy);

			//	ライトベクトル補正
			Out.vLight.x = dot(vx, DirLightVec);
			Out.vLight.y = dot(vy, DirLightVec);
			Out.vLight.z = dot(N, DirLightVec);

			//	点光源ベクトル補正
			float3	pvec = (P - plight_pos) / plight_range;

				//	ライトベクトル補正
				Out.pLight.x = dot(vx, pvec);
			Out.pLight.y = dot(vy, pvec);
			Out.pLight.z = dot(N, pvec);

			//	視線ベクトル補正
			float3	E = P - ViewPos;	//	視線ベクトル
				Out.vE.x = dot(vx, E);
			Out.vE.y = dot(vy, E);
			Out.vE.z = dot(N, E);

			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー(有り)
	//-----------------------------------------------------------------------------------
		float4	PS_Skin( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT = (float4)0;
			float2	Tex = In.Tex;
			float4	sss_tex = tex2D(FaceSamp, Tex);

			//	法線取得
			float3	N = tex2D(NormalSamp, Tex).xyz * 2.0f - 1.0f;

			//	ライトの強度
			float	l = dot( In.vLight, -N );
			float3	l1 = (l + 1.0f) * 0.5f;
			float3	l2 = max(0, l);
			float3	l3 = dot(In.vLight, N);

			//	表皮カラー計算
			float3	dif = l1 * 0.3f;
			float3	eps = epiScatter * l1 * sss_tex.r;
			float3	sps = subScatter * l2 * sss_tex.g;
			float3	back = backScatter * l3 * sss_tex.b;

			//	カラー合成
			float3	col = dif * eps * sps * back;

			//	ピクセル色決定
			OUT = tex2D( DecaleSamp, Tex );
			OUT.rgb *= col * DirLightColor + In.Ambient.rgb;

			//	視線反射ベクトル
			float3	E = normalize(In.vE);
			float3	R = reflect(-E, N);

			//	スペキュラ
			float	ssss = ( 1 - sss_tex.a ) * DirLightColor;
			OUT.rgb += pow( max( 0, dot( R, In.vLight ) ), 5 ) * sp_primary * ssss;
			OUT.rgb += pow( max( 0, dot( R, In.vLight ) ), 35 ) * sp_secondary * ssss;

			return	OUT;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー(なし)
	//-----------------------------------------------------------------------------------
		float4	PS_NoSkin( VS_OUTPUT In )	:	COLOR
		{
			float4	OUT;
			float2	Tex = In.Tex;

			//	法線取得
			float3	N = tex2D(NormalSamp, Tex).xyz * 2.0f - 1.0f;

			//	ライト計算
			In.vLight = normalize( In.vLight );
			float3	light = DirLight2(In.vLight, N) + In.Ambient.rgb;

			//	視線反射ベクトル
			float3	E = normalize(In.vE);
			float3	R = reflect(-E, N);

			//	ピクセル色決定
			OUT = In.Color * tex2D( DecaleSamp, Tex );
			OUT.rgb *= light;


			return	OUT;
		}

//************************************************************************
//
//	テクニック
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	通常
	//-----------------------------------------------------------------------------------
		
		//	通常
		technique	copy
		{
			pass	P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				CullMode				=	CCW;
				ZEnable					=	true;
				ZWriteEnable			=	true;

				VertexShader		=		compile vs_2_0	VS_Basic();
				PixelShader		=		compile ps_2_0 PS_Basic();
			}
		}

		//	通常(透明部分くり抜き)
		technique	alpha
		{
			pass	P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				CullMode = CCW;
				ZEnable = true;
				ZWriteEnable = false;

				VertexShader = compile vs_2_0	VS_Basic();
				PixelShader = compile ps_2_0 PS_Basic();
			}
		}

	//-----------------------------------------------------------------------------------
	//	半球ライティング＋フォグ+スペキュラ
	//-----------------------------------------------------------------------------------

		//	半球ライティング+フォグ
		technique	lighting
		{
			pass	P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				CullMode				=	CCW;
				ZEnable					=	true;
				ZWriteEnable			=	true;

				VertexShader		=		compile	 vs_2_0 VS_Lighting();
				PixelShader		=		compile ps_2_0 PS_Lighting();
			}
		}

		//	半球ライティング＋フォグ＋スペキュラ
		technique	lighting2
		{
			pass	P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				CullMode				=		CCW;
				ZEnable					=		true;
				ZWriteEnable			=		true;

				VertexShader		=		compile		vs_2_0 VS_Lighting2();
				PixelShader		=		compile		ps_2_0 PS_Lighting2();
			}
		}

	//-----------------------------------------------------------------------------------
	//	環境マップ
	//-----------------------------------------------------------------------------------

		//	環境マップ
		technique	environment
		{
			pass P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				CullMode				=	CCW;
				ZEnable					=	true;
				ZWriteEnable			=	true;

				VertexShader		=		compile vs_2_0		VS_Lighting2();
				PixelShader		=		compile ps_2_0		PS_Env();
			}
		}

	//-----------------------------------------------------------------------------------
	//	トゥーン
	//-----------------------------------------------------------------------------------

		//	トゥーン
		technique	toon
		{
			//	輪郭線
			pass	Outline
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				CullMode				=		CW;
				ZEnable					=		true;
				ZWriteEnable			=		true;

				VertexShader		=	compile vs_2_0 VS_Outline();
				PixelShader		=	compile ps_2_0 PS_Outline();
			}

			//	トゥーン
			pass	Toon
			{
				CullMode			=		CCW;
				ZWriteEnable		=		true;

				VertexShader		=		compile vs_2_0 VS_Toon();
				PixelShader		=		compile ps_2_0 PS_Toon();
			}
		}

	//-----------------------------------------------------------------------------------
	//	法線マップ、視差マップ、スペキュラマップ、環境マップ
	//-----------------------------------------------------------------------------------

		//	法線マップ適用
		technique	level1
		{
			pass P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				CullMode				=	CCW;
				ZEnable					=	true;
				ZWriteEnable			=	true;

				VertexShader		=		compile vs_2_0		VS_FullFX();
				PixelShader		=		compile ps_2_0		PS_Level1();
			}
		};

		//	視差マップ適用
		technique	level2
		{
			pass P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				CullMode				=	CCW;
				ZEnable					=	true;
				ZWriteEnable			=	true;

				VertexShader			=		compile		vs_2_0		VS_FullFX();
				PixelShader			=		compile		ps_2_0		PS_Level2();
			}
		};

		//	スペキュラマップ適用
		technique	level3
		{
			pass P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				CullMode				=		CCW;
				ZEnable					=		true;
				ZWriteEnable			=		true;

				VertexShader			= compile		vs_2_0		VS_FullFX();
				PixelShader			= compile		ps_2_0		PS_Level3();
			}
		};

		//	環境マップ適用
		technique	level4
		{
			pass P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				CullMode				=		CCW;
				ZEnable					=		true;
				ZWriteEnable			=		true;

				VertexShader			=		compile		vs_2_0		VS_FullFX();
				PixelShader			=		compile		ps_2_0		PS_Level4();
			}
		};

		//	点光源適用
		technique	level5
		{
			pass P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				CullMode				=		CCW;
				ZWriteEnable			=		true;

				VertexShader		=		compile		vs_2_0		VS_FullFX();
				PixelShader		=		compile		ps_2_0		PS_Level5();
			}
		};

	//-----------------------------------------------------------------------------------
	//	擬似HDR表現
	//-----------------------------------------------------------------------------------

		//	スペキュラ
		technique	specular
		{
			pass	P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				ZEnable					=	true;
				ZWriteEnable			=	true;

				VertexShader		=		compile	vs_2_0	VS_Specular();
				PixelShader		=		compile ps_2_0	PS_Specular();
			}
		}

	//-----------------------------------------------------------------------------------
	//	エフェクト用
	//-----------------------------------------------------------------------------------
		
		//	通常
		technique	effect_copy
		{
			pass	P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				ZEnable					=		true;
				ZWriteEnable			=		true;

				VertexShader		=		compile	vs_2_0 VS_Effect();
				PixelShader		=		compile ps_2_0 PS_Effect();
			}
		}

		//	加算
		technique	effect_add
		{
			pass	P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				srcBlend				=		SrcAlpha;
				DestBlend				=		One;
				CullMode				=		None;
				ZEnable					=		false;

				VertexShader		=		compile	vs_2_0 VS_Effect();
				PixelShader		=		compile ps_2_0 PS_Effect();
			}
		}

	//-----------------------------------------------------------------------------------
	//	点光源付きフルエフェクト
	//-----------------------------------------------------------------------------------

		//	合成なし
		technique full
		{
			pass P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				ZWriteEnable			=	true;

				VertexShader			=		compile		vs_3_0 VS_Full();
				PixelShader			=		compile		ps_3_0 PS_Full();
			}
		}

		//	シャドウマップ採用
		technique full_s
		{
			pass P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		InvSrcAlpha;
				ZWriteEnable		=		true;

				VertexShader		=		compile		vs_3_0 VS_Full_S();
				PixelShader			=		compile		ps_3_0 PS_Full_S();
			}
		}

		//	シャドウマップ採用(ライティングなし)
		technique nolight_s
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;

				VertexShader = compile		vs_3_0 VS_Full_S();
				PixelShader = compile		ps_3_0 PS_NoLight_S();
			}
		}
	//-----------------------------------------------------------------------------------
	//	被写界深度
	//-----------------------------------------------------------------------------------
		
		//	深度描画
		technique	depth
		{
			pass	P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				ZWriteEnable			=	true;

				VertexShader		=		compile vs_2_0 VS_Depth();
				PixelShader		=		compile ps_2_0 PS_Depth();
			}
		}

	//-----------------------------------------------------------------------------------
	//	反射マップ
	//-----------------------------------------------------------------------------------

		//	反射マップ作成
		technique	Reflect
		{
			pass	P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				ZWriteEnable			=	true;
				CullMode				=	cw;
			
				VertexShader		=		compile vs_3_0 VS_Reflect();
				PixelShader		=		compile ps_3_0 PS_Full();
			}

		}

		//	反射マップ付きフルエフェクト
		technique	full_sr
		{
			pass	P0
			{
				AlphaBlendEnable	= true;
				BlendOp				= Add;
				SrcBlend				= SrcAlpha;
				DestBlend				= InvSrcAlpha;
				ZWriteEnable			= true;

				VertexShader		=		compile vs_3_0 VS_FullFX_SR();
				PixelShader		=		compile ps_3_0 PS_FullFX_SR();
			}
		}

	//------------------------------------------------------
	//	スキンシェーダー	
	//------------------------------------------------------
		
		//	有り
		technique skin
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;
				CullMode = none;
				VertexShader = compile vs_3_0 VS_FullFX();
				PixelShader = compile ps_3_0 PS_Skin();
			}
		}

		//	なし
		technique no_skin
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;
				CullMode = none;
				VertexShader = compile vs_3_0 VS_FullFX();
				PixelShader = compile ps_3_0 PS_NoSkin();
			}
		}

	//------------------------------------------------------
	//	スクリーン用	（高鳥担当）
	//------------------------------------------------------

		float effect_size = 0.0f;
		float screen_width = 1280.0f;
		float screen_height = 720.0f;

		float4 PS_WipeEffect( float2 ScreenPos : VPOS ) : COLOR
		{
			float p1 = ScreenPos.x - screen_width  * 0.5f;
			float p2 = ScreenPos.y - screen_height * 0.5f;

			return ( p1*p1 + p2*p2 >= effect_size*effect_size ) ? float4( 0.0f, 0.0f, 0.0f, 1.0f ) : float4( 0.0f, 0.0f, 0.0f, 0.0f );
		}

		technique WipeEffect
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;

				PixelShader = compile ps_3_0 PS_WipeEffect();
			}
		}


		//-----------------------------------------------------------------------------------
		//	バレット用
		//-----------------------------------------------------------------------------------
				//	合成なし
		technique bullet
		{
			pass P0
			{
				AlphaBlendEnable	=	true;
				BlendOp				=	Add;
				SrcBlend				=	SrcAlpha;
				DestBlend				=	InvSrcAlpha;
				ZWriteEnable			=	true;

				VertexShader = compile		vs_3_0 VS_Bullet();
				PixelShader			=		compile		ps_3_0 PS_Full();
			}
		}

//-----------------------------------------------------------------------------------
//	環境関連
//-----------------------------------------------------------------------------------

	float4x4		Projection;	//	投影変換行列
	float4x4		TransMatrix;	//	ワールド変換行列
	float4x4		matView;
	float3		ViewPos = { 0.0f, 0.0f, 0.0f };	//	視線変数
	float4x4		InvProj;	//	逆変換行列

//-----------------------------------------------------------------------------------
//	テクスチャサンプラー
//-----------------------------------------------------------------------------------

	//	デカールサンプラー
	texture	Texture;
	sampler	DecaleSamp = sampler_state
	{
		Texture = <Texture>;
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

	//	スクリーンスペース深度
	texture	DepthBuf;	//	カメラ空間深度
	sampler DepthBufSamp = sampler_state
	{
		Texture = <DepthBuf>;
		MinFilter = POINT;
		MagFilter = POINT;
		MipFilter = NONE;

		AddressU = CLAMP;
		AddressV = CLAMP;
	};

	//	スペキュラバッファ
	texture	SpecularBuf;	//	スペキュラ
	sampler SpecularBufSamp = sampler_state
	{
		Texture = <SpecularBuf>;
		MinFilter = POINT;
		MagFilter = POINT;
		MipFilter = NONE;

		AddressU = CLAMP;
		AddressV = CLAMP;
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
		float4	wPos	: TEXCOORD4;

		float3	N	:	TEXCOORD5;
		float3	T	:	TEXCOORD6;
		float3	B	:	TEXCOORD7;
	};

	struct VS_INPUT
	{
		float4	Pos		:	POSITION;
		float3	Normal	:	NORMAL;
		float2	Tex		:	TEXCOORD0;

		float4	Color		:	COLOR0;	//	頂点カラー
	};

	struct PS_OUTPUT
	{
		float4	color	:	COLOR0;
		float4	spec	:	COLOR1;
		float4	depth	:	COLOR2;
		float4	normal	:	COLOR3;
	};

	struct POUT_LIGHT
	{
		float4	color	:	COLOR0;
		float4	spec	:	COLOR1;
	};

//-----------------------------------------------------------------------------------
//	平行光
//-----------------------------------------------------------------------------------

	//	パラメータ
	float3	LightVec = { 0.7f, -0.7f, 0.0f };
	float3	LightColor = { 1.0f, 1.0f, 1.0f };

//-----------------------------------------------------------------------------------
//	スポットライト
//-----------------------------------------------------------------------------------

	//	パラメータ
	float3	sLightPos = { 0.0f, 3.0f, 0.0f };
	float3	sLightDir = { 0.0f, -1.0f, 0.0f };
	float3	sLightColor = { 1.0f, 1.0f, 1.0f };
	float		inner = 0.7f;
	float		outer = 0.6f;

//-----------------------------------------------------------------------------------
//	点光源
//-----------------------------------------------------------------------------------

	//	パラメータ
	float3	pLightPos = { 0.0f, 3.0f, 0.0f };
	float3	pLightColor = { 1.0f, 1.0f, 1.0f };
	float		pLightRange = 5.0f;

//************************************************************************
//
//	ディファード①②
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	頂点シェーダー
	//-----------------------------------------------------------------------------------
		VS_OUTPUT	VS_Differed( VS_INPUT In )
		{
			VS_OUTPUT	Out = ( VS_OUTPUT )0;

			Out.Pos = mul( In.Pos, Projection );
			Out.Tex = In.Tex;
			Out.Color = 1.0f;

			Out.wPos = Out.Pos;

			float3x3		mat = (float3x3)matView;

			//	カメラ空間座標系
			Out.N = mul( In.Normal, mat );
			Out.N = normalize( Out.N );
			float3	vy = { 0.0f, 1.0f, 0.001f };
			Out.T = cross( vy, Out.N );
			Out.T = normalize( Out.T );
			Out.B = cross( Out.N, Out.T );
			Out.B = normalize( Out.B );
			return	Out;
		}

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		PS_OUTPUT	PS_Differed( VS_OUTPUT In )
		{
			PS_OUTPUT	OUT;

			float2	Tex = In.Tex;

			//	ディフューズ色
			OUT.color = In.Color * tex2D( DecaleSamp, Tex );

			//	スペキュラ
			OUT.spec = tex2D( SpecularSamp, Tex );
			OUT.spec.a = 1;

			//	深度
			OUT.depth.rgb = In.wPos.z / In.wPos.w;
			OUT.depth.a = 1;

			//	頂点空間→カメラ変換行列
			float3x3		ToView;
			ToView[0] = normalize( In.T );
			ToView[1] = normalize( In.B );
			ToView[2] = normalize( In.N );

			//	法線変換
			float3	N = tex2D( NormalSamp, Tex ).xyz - 0.5f;
			float3	normal = normalize( mul( N, ToView ) );
			normal = normal * 0.5f + 0.5f;

			//	カメラ空間法線
			OUT.normal.rgb = normal;
			OUT.normal.a = 1;

			return	OUT;
		}

//************************************************************************
//
//	平行光
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		float4	PS_DirLight( float2 Tex : TEXCOORD0 )	:	COLOR
		{
			float4	OUT;
			float4	n = tex2D(DecaleSamp, Tex);
			float3	normal = n.rgb * 2.0f - 1.0f;
			normal = normalize( normal );

			//	ライト率
			float	r = max( 0, dot( normal, -LightVec ) );

			//	ピクセル色決定
			OUT.rgb = r * LightColor;
			OUT.a = 1;

			return	OUT;
		}

//************************************************************************
//
//	スペキュラ
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		POUT_LIGHT	PS_DirLight2( float2 Tex : TEXCOORD0 )
		{
			POUT_LIGHT	OUT;
			
			float4	n = tex2D( DecaleSamp, Tex );
			float3	normal = n.rgb * 2.0f - 1.0f;
			normal = normalize( normal );

			//	ライト率
			float r = max( 0, dot( normal, -LightVec ) );

			//	ピクセル色決定
			OUT.color.rgb = r * LightColor;
			OUT.color.a = 1;

			//	カメラ空間変換
			float	depth = tex2D( DepthBufSamp, Tex ).r;
			float4 screen;
			screen.xy = Tex * 2.0f - 1.0f;
			screen.y = -screen.y;
			screen.z = depth;
			screen.w = 1;
			float4	pos = mul( screen, InvProj );
			pos.xyz /= pos.w;

			//	スペキュラ
			float3	E = pos.xyz;

			E = normalize( E );
			float3	R = normalize( -LightVec - E );

			float	sp = pow( max( 0, dot( R, normal ) ), 10 );
			float4	sp_tex = tex2D( SpecularBufSamp, Tex );
			OUT.spec.rgb = sp_tex.rgb * sp * LightColor;
			OUT.spec.a = 1;

			return	OUT;
		}

//************************************************************************
//
//	点光源
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		POUT_LIGHT	PS_PointLight( float2 Tex : TEXCOORD0 )
		{
			POUT_LIGHT	OUT;

			//	カメラ空間変換
			float		depth = tex2D( DepthBufSamp, Tex  ).r;
			float4	screen;
			screen.xy = Tex * 2 - 1;
			screen.y = -screen.y;
			screen.z = depth;
			screen.w = 1;
			float4	pos = mul( screen, InvProj );
			pos.xyz /= pos.w;

			//	ライトベクトル
			float3	LightVec = pos - pLightPos;
			float	dist = length( LightVec );
			LightVec = normalize( LightVec );

			//	法線取得
			float4	n = tex2D(DecaleSamp, Tex);
			float3	normal = n.rgb * 2 - 1;
			normal = normalize( normal );

			//	減衰量
			float	intensity = max( 0, 1.0f - ( dist / pLightRange ) );

			//	ライト計算
			float	r = max( 0, dot( normal, - LightVec ) );

			//	ピクセル色決定
			OUT.color.rgb = r * pLightColor * intensity;
			OUT.color.a = 1;

			//	スペキュラ
			float3	E = pos.xyz;
			float3	R = normalize( -LightVec - E );

			float	sp = pow( max( 0, dot( R, normal ) ), 10 );
			float4	sp_tex = tex2D( SpecularSamp, Tex );
			OUT.spec.rgb = sp_tex.rgb * sp * pLightColor * intensity;
			OUT.spec.a = 1;
			
			return	OUT;
		}

//************************************************************************
//
//	スポットライト
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	ピクセルシェーダー
	//-----------------------------------------------------------------------------------
		POUT_LIGHT	PS_SpotLight( float2	Tex : TEXCOORD0 )
		{
			POUT_LIGHT		OUT;

			float4	n = tex2D( DecaleSamp, Tex );
			float3	normal = n.rgb * 2.0f - 1.0f;
			normal = normalize( normal );

			//	カメラ空間変換（これまでと同様）
			float		depth = tex2D( DepthBufSamp, Tex ).r;
			float4	screen;
			screen.xy = Tex * 2 - 1;
			screen.y = -screen.y;
			screen.z = depth;
			screen.w = 1;
			float4	pos = mul( screen, InvProj );
			pos.xyz /= pos.w;

			//	ライトベクトル
			float3	LightVec = ( float3 )pos - sLightPos;
			float3	dist = length( LightVec );
			LightVec = normalize( LightVec );

			//	減衰量
			float	intensity = dot( LightVec, sLightDir );
			intensity = ( inner - intensity ) / ( inner - outer );
			intensity = saturate( 1 - intensity );

			//	ライト計算
			float	r = max( 0, dot( normal, -LightVec ) );

			//	ピクセル色決定
			OUT.color.rgb = r * sLightColor * intensity;
			OUT.color.a = 1;

			//	スペキュラ
			float3	E = pos.xyz;
			float3	R = normalize(-LightVec - E);

			float	sp = pow( max( 0, dot( R, normal ) ), 10 );
			
			float4	sp_tex = tex2D( SpecularBufSamp, Tex );
			OUT.spec.rgb = sp_tex.rgb * sp * LightColor * intensity;
			OUT.spec.a = 1;

			return	OUT;
		}

//************************************************************************
//
//	テクニック
//
//************************************************************************

	//-----------------------------------------------------------------------------------
	//	ディファード①②
	//-----------------------------------------------------------------------------------

		//	MRT
		technique	differed
		{
			pass P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = InvSrcAlpha;
				ZWriteEnable = true;

				VertexShader = compile vs_3_0 VS_Differed();
				PixelShader = compile ps_3_0 PS_Differed();
			}
		}

	//-----------------------------------------------------------------------------------
	//	ディファード③
	//-----------------------------------------------------------------------------------

		//	平行光
		technique	dirlight
		{
			pass	P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = One;
				CullMode = None;
				ZEnable = false;
				PixelShader = compile ps_2_0 PS_DirLight();
			}
		}

	//-----------------------------------------------------------------------------------
	//	ディファード④
	//-----------------------------------------------------------------------------------

		//	平行光
		technique	dirlight2
		{
			pass	P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = One;
				CullMode = None;
				ZEnable = false;
				PixelShader = compile ps_2_0 PS_DirLight2();
			}
		}

	//-----------------------------------------------------------------------------------
	//	ディファード⑤
	//-----------------------------------------------------------------------------------

		//	点光源
		technique	pointlight
		{
			pass	P0
			{
				AlphaBlendEnable = true;
				BlendOp = Add;
				SrcBlend = SrcAlpha;
				DestBlend = One;
				CullMode = None;
				ZEnable = false;
				PixelShader = compile ps_2_0 PS_PointLight();
			}
		}

	//-----------------------------------------------------------------------------------
	//	ディファード⑤
	//-----------------------------------------------------------------------------------

		//	スポットライト
		technique	spotlight
		{
			pass	P0
			{
				AlphaBlendEnable	=		true;
				BlendOp				=		Add;
				SrcBlend				=		SrcAlpha;
				DestBlend				=		One;
				CullMode				=		None;
				ZEnable					=		false;
				PixelShader			=		compile		ps_2_0		PS_SpotLight();
			}
		}
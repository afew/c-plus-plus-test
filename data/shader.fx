//
// Shader Example.
// Copyleft (c) 2007 Heesung Oh(Galic Studio). All rights Not reserved.
//
////////////////////////////////////////////////////////////////////////////////


float4x4	m_mtWVP;	// World * View * Projection

float4		m_color;	// color

// Diffuse Map
texture	m_tex_dif;
sampler	SampDif = sampler_state
{
	Texture = <m_tex_dif>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};


// Output Vertex Processing Structure
struct SVsOut
{
	float4 Pos	: POSITION;
	float2 Tx0	: TEXCOORD0;
};


// Vertex Shader Processing
SVsOut VtxPrc(	float4 Pos : POSITION	// Position Vector
			  ,	float2 Tx0 : TEXCOORD0	// Diffuse Map Coordiante
)
{
	SVsOut Out = (SVsOut)0;						// Initialize to Zero
	Out.Pos	= mul(Pos, m_mtWVP);				// Transform Rotation, Scaling, Translation of Vertex Position
	Out.Tx0 = Tx0;								// Diffuse Map Coordinate

	return Out;
}

// Pixel Shader Processing
float4 PxlPrcNormal(SVsOut In) : COLOR
{
	float4 Out;
	float4 tex = tex2D( SampDif, In.Tx0 );		// Diffuse Map

	Out = tex;
	return Out;
}

// Pixel Shader Processing
float4 PxlPrcMulti(SVsOut In) : COLOR
{
	float4 Out;
	float4 tex = tex2D( SampDif, In.Tx0 );		// Diffuse Map

	Out = tex;
	Out.rgb *= m_color.rgb;
	return Out;
}

// Pixel Shader Processing
float4 PxlPrcOverlay(SVsOut In) : COLOR
{
	float4 u_effect_color=float4(214.0/255.0, 52.0/255.0, 41.0/255.0, 1.0);

	float4 color = tex2D( SampDif, In.Tx0 );		// Diffuse Map
	float4 Out= color * u_effect_color;
	Out.rgb *= 2.0;
	if(color.r>0.5)	Out.r = 2.0 * (color.r + u_effect_color.r) - Out.r -1.0;
	if(color.g>0.5)	Out.g = 2.0 * (color.g + u_effect_color.g) - Out.g -1.0;
	if(color.b>0.5)	Out.b = 2.0 * (color.b + u_effect_color.b) - Out.b -1.0;
	return Out;
}



technique Tech0
{
	pass P0
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader  = compile ps_3_0 PxlPrcNormal();
	}

	pass P1
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader  = compile ps_3_0 PxlPrcMulti();
	}

	pass P2
	{
		VertexShader = compile vs_3_0 VtxPrc();
		PixelShader  = compile ps_3_0 PxlPrcOverlay();
	}
}

